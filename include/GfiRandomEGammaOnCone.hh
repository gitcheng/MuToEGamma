//--------------------------------------------------------------------------
// File and Version Information:
// $Id: GfiSingleParticle.hh 433 2010-01-14 15:29:24Z stroili $
//
// Description:
//      Framework module to generate single particle events on a double-cone
//      surface.
//
//
//------------------------------------------------------------------------

#ifndef GFIRANDOMEGAMMAONCONE_HH
#define GFIRANDOMEGAMMAONCONE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "GfiBase/GfiGenerator.hh"
#include "Framework/AbsParmString.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class GfiRandomEGammaOnCone : public GfiGenerator {
public:

  // Constructors
  GfiRandomEGammaOnCone( const char* const theName, 
			 const char* const theDescription );
  
  // Destructor
  virtual ~GfiRandomEGammaOnCone( );
  
  // GfiGenerator virtuals:
  virtual void initialize() {}

  virtual bool generateEvent(AbsEvent * theEvent, 
                             const PepCollision & collision);
  
  virtual void finalize();
  virtual bool doTransform() const {return false;}  // don't transformEvent(..)

private:

  int _lundid;
  AbsParmString _p1name;
  AbsParmString _p2name;
  AbsParmDouble _pmin;
  AbsParmDouble _pmax;
  //AbsParmBool   _usePt;
  AbsParmDouble _costhetamin;
  AbsParmDouble _costhetamax;
  AbsParmDouble _phimin;
  AbsParmDouble _phimax;
  AbsParmDouble _cone1z; // cone 1 vertex z
  AbsParmDouble _cone2z; // cone 2 vertex z
  AbsParmDouble _conez0; // z of where the two cones meet at their bases
  AbsParmDouble _coneR;  // the radius of the cone base
  AbsParmDouble _ipT;
  AbsParmBool   _backtoback;

  void genVertex(double *vertex);

};

#endif
