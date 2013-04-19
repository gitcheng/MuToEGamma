//--------------------------------------------------------------------------
// File and Version Information:
// $Id: GfiSingleParticleOnCone.cc 900 2011-06-17 12:45:34Z stroili $
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
#include "MuToEGamma/GfiSingleParticleOnCone.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/Constants.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "CLHEP/Geometry/HepPoint.h"
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


GfiSingleParticleOnCone::GfiSingleParticleOnCone(const char* const theName,
						 const char* const theDescription) :
  GfiGenerator(theName, theDescription, new GfiContCollisionGenerator)
  , _mass(0.0), _massSquared(0.0)
  , _pmin("Pmin", this, 0.1), _pmax("Pmax", this, 5.0), _usePt("usePt", this, false)
  , _costhetamin("CosThetamin", this, -1.0)
  , _costhetamax("CosThetamax", this, 1.0)
  , _phimin("Phimin", this, 0.0)
  , _phimax("Phimax", this, Constants::twoPi)
  , _cone1z("cone1z", this, -1)  // in cm
  , _cone2z("cone2z", this, 1)  // in cm
  , _conez0("conez0", this, 0)  // in cm
  , _coneR("coneR", this, 1)  // in cm
  , _ipT("ipT", this, 0.0) // vertex time in sec
  , _generate("GENERATE", this, "gamma")
{
  commands()->append(&_generate);
  commands()->append(&_pmin);
  commands()->append(&_pmax);
  commands()->append(&_usePt);
  commands()->append(&_costhetamin);
  commands()->append(&_costhetamax);
  commands()->append(&_phimin);
  commands()->append(&_phimax);
  commands()->append(&_cone1z);
  commands()->append(&_cone2z);
  commands()->append(&_conez0);
  commands()->append(&_coneR);
  commands()->append(&_ipT);
}

//--------------
// Destructor --
//--------------

GfiSingleParticleOnCone::~GfiSingleParticleOnCone() {
}

//--------------
// Operations --
//--------------

bool GfiSingleParticleOnCone::generateEvent(AbsEvent* anEvent,
					    const PepCollision & collision) {

  // determine the particle to generate here to allow the user to change it on
  // the fly:
  double mass;
  int lundId;

  const PdtEntry * partEntry = Pdt::lookup(_generate.value());
  if (0 != partEntry) {
    lundId = partEntry->lundId();
    mass = partEntry->mass();
  } else {
    ErrMsg(fatal) << "ERROR: " << name()
        << ", GfiSingleParticleOnCone::event(AbsEvent*, int):" << endl
        << "       Invalid argument \"" << _generate.value()
        << "\" given to Generate command." << endl
        << "       Argument is not in pdt.table or PdtInit::beginJob was"
        << " not called." << endmsg;
    assert(0);
  }

  double pmin = _pmin.value();
  double pmax = _pmax.value();
  double costhetamin = _costhetamin.value();
  double costhetamax = _costhetamax.value();
  double phimin = _phimin.value();
  double phimax = _phimax.value();

  double p = pmin + (pmax - pmin) * generateDouble();
  double costheta = costhetamin + (costhetamax - costhetamin)
      * generateDouble();
  double phi = phimin + (phimax - phimin) * generateDouble();

  double sintheta = sqrt(1.0 - costheta * costheta);

  double px;
  double py;
  double pz;

  if (_usePt.value()) {
    px = p * cos(phi);
    py = p * sin(phi);
    pz = p * costheta / sintheta;
  } else {
    px = p * sintheta * cos(phi);
    py = p * sintheta * sin(phi);
    pz = p * costheta;
  }

  int mothers[2] = { 0, 0 };
  int children[2] = { 0, 0 };

  double p4[5] = { px, py, pz, sqrt(mass * mass + px * px + py * py + pz * pz),
      mass };
  //  double vertex[4] = {0.0, 0.0, 0.0, 0.0};
  double vertex[4];


  // Generate a random (x,y) that is uniform on a circle
  double theta= Constants::twoPi * generateDouble();
  double r= sqrt( generateDouble() * _coneR.value() );



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

  static const int oneParticle = 1; // # of particles produced
  static const int particleNum = 1; // fortran index

  // Reset stdhep common block for the new event and store the track:
  std_new_event_();
  std_set_num_entries_(oneParticle);

  std_set_entry_(particleNum, STDHEP_STABLE, lundId, mothers, children, p4,
      vertex);

  return true;
}

//-----------------------------------------------------------------------
void GfiSingleParticleOnCone::finalize() {
}

