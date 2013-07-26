//--------------------------------------------------------------------------
// File and Version Information:
// $Id: GfiRandomEGammaOnCone.cc 900 2011-06-17 12:45:34Z stroili $
//
// Description:
//      Implements single particle generator for framework based simulation.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Anders Ryd       -craeted Oct 15 1997
//      Stefan Christ    added vertex smearing Nov 07 2000
//
//------------------------------------------------------------------------

//----------------
// BaBar Header --
//----------------
#include "BaBar/BaBar.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------

#include <iostream>

//-----------------------
// This Class's Header --
//-----------------------
#include "MuToEGamma/GfiRandomEGammaOnCone.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/Constants.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "BbrGeom/HepPoint.hh"
#include "GfiBase/GfiContCollisionGenerator.hh"
#include "PepData/PepCollision.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtLund.hh"
#include "ErrLogger/ErrLog.hh"

#include "stdhep/StdHepUtil.hh"
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------


GfiRandomEGammaOnCone::GfiRandomEGammaOnCone(const char* const theName,
					     const char* const theDescription) :
  GfiGenerator(theName, theDescription, new GfiContCollisionGenerator)
  , _p1name("p1name", this, "e+")
  , _p2name("p2name", this, "gamma")
  , _pmin("Pmin", this, 0.1), _pmax("Pmax", this, 5.0)
  //, _usePt("usePt", this, false)
  , _costhetamin("CosThetamin", this, -1.0)
  , _costhetamax("CosThetamax", this, 1.0)
  , _phimin("Phimin", this, 0.0)
  , _phimax("Phimax", this, Constants::twoPi)
  , _cone1z("cone1z", this, -1)  // in cm
  , _cone2z("cone2z", this, 1)  // in cm
  , _conez0("conez0", this, 0)  // in cm
  , _coneR("coneR", this, 1)  // in cm
  , _ipT("ipT", this, 0.0) // vertex time in sec
  , _backtoback("backtoback", this, true)
{
  commands()->append(&_p1name);
  commands()->append(&_p2name);
  commands()->append(&_pmin);
  commands()->append(&_pmax);
  //commands()->append(&_usePt);
  commands()->append(&_costhetamin);
  commands()->append(&_costhetamax);
  commands()->append(&_phimin);
  commands()->append(&_phimax);
  commands()->append(&_cone1z);
  commands()->append(&_cone2z);
  commands()->append(&_conez0);
  commands()->append(&_coneR);
  commands()->append(&_ipT);
  commands()->append(&_backtoback);
}

//--------------
// Destructor --
//--------------

GfiRandomEGammaOnCone::~GfiRandomEGammaOnCone() {
}

//--------------
// Operations --
//--------------

bool GfiRandomEGammaOnCone::generateEvent(AbsEvent* anEvent,
					    const PepCollision & collision) {

  // determine the particle to generate here to allow the user to change it on
  // the fly:
  static const PdtEntry *p1pdt = Pdt::lookup(_p1name.value());
  static const PdtEntry *p2pdt = Pdt::lookup(_p2name.value());

  if ( 0==p1pdt || 0==p2pdt ) {
    ErrMsg(fatal) << "Invalid argument " << _p1name.value() 
		  << " of p1name, or " << _p2name.value()
		  << " of p2name. Not a valid pdt particle name." << endmsg;
  }

  static const double p1mass(p1pdt->mass());
  static const double p2mass(p2pdt->mass());
  static const int p1lundId(p1pdt->lundId());
  static const int p2lundId(p2pdt->lundId());

  double pmin = _pmin.value();
  double pmax = _pmax.value();
  double costhetamin = _costhetamin.value();
  double costhetamax = _costhetamax.value();
  double phimin = _phimin.value();
  double phimax = _phimax.value();

  double p1 = pmin + (pmax - pmin) * generateDouble();
  double costheta1 = costhetamin + (costhetamax - costhetamin)
      * generateDouble();
  double phi1 = phimin + (phimax - phimin) * generateDouble();
  double sintheta1 = sqrt(1.0 - costheta1 * costheta1);

  double p1x= p1 * sintheta1 * cos(phi1);;
  double p1y= p1 * sintheta1 * sin(phi1);;
  double p1z= p1 * costheta1;

  double costheta2, phi2;
  double p2 = pmin + (pmax - pmin) * generateDouble();
  if ( _backtoback.value() ) {
    costheta2= -costheta1;
    phi2= phi1 + Constants::pi;
  } else {
    costheta2 = costhetamin + (costhetamax - costhetamin)* generateDouble();
    phi2= phimin + (phimax - phimin) * generateDouble();
  }
  double sintheta2 = sqrt(1.0 - costheta2 * costheta2);
  double p2x= p2 * sintheta2 * cos(phi2);;
  double p2y= p2 * sintheta2 * sin(phi2);;
  double p2z= p2 * costheta2;


  int mothers[2] = { 0, 0 };
  int children[2] = { 0, 0 };

  double p14[5] = {p1x,p1y,p1z,sqrt(p1mass*p1mass+p1x*p1x+p1y*p1y+p1z*p1z), p1mass };
  double p24[5] = {p2x,p2y,p2z,sqrt(p2mass*p2mass+p2x*p2x+p2y*p2y+p2z*p2z), p2mass };

  // vertex
  double p1vertex[4];
  double p2vertex[4];

  genVertex(p1vertex);
  genVertex(p2vertex);

  static const int twoParticle = 2; // # of particles produced
  static const int particleNum1 = 1; // fortran index
  static const int particleNum2 = 2; // fortran index

  // Reset stdhep common block for the new event and store the track:
  std_new_event_();
  std_set_num_entries_(twoParticle);

  std_set_entry_(particleNum1, STDHEP_STABLE, p1lundId, mothers, children, p14,
      p1vertex);
  std_set_entry_(particleNum2, STDHEP_STABLE, p2lundId, mothers, children, p24,
      p2vertex);

  return true;
}

//-----------------------------------------------------------------------
void GfiRandomEGammaOnCone::finalize() {
}

void GfiRandomEGammaOnCone::genVertex(double *vertex) {

  // Generate a random (x,y) that is uniform on a circle
  double theta= Constants::twoPi * generateDouble();
  double r= sqrt( generateDouble() ) * _coneR.value() ;

  vertex[0] = r*cos(theta) * 10; // convert from cm to mm
  vertex[1] = r*sin(theta) * 10; // convert from cm to mm
  vertex[3] = _ipT.value() ; //

  bool zsign= (generateDouble() > 0.5);
  if ( zsign ) {
    vertex[2]= _cone2z.value() - r/_coneR.value() * (_cone2z.value()-_conez0.value());
  } else {
    vertex[2]= _cone1z.value() + r/_coneR.value() * (_conez0.value()-_cone1z.value());
  }
  vertex[2] *= 10.; // convert from cm to mm
}
