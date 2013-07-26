//------------------------------------------------------------------------
// File and Version Information:
//      $Id: MuToEGammaApp.cc $
//
// Description:
//	Class AppUserBuild. This class must be provided by the user of
//	the framework in order to build an application. It must define
//	the modules that are to form the basis of the application.
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-------------
// C++ Headers:
//-------------
#include <signal.h>

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppUserBuild.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"

// actions
#include "PacMC/PacActions.hh"
// Pac
#include "PacMC/PmcSimulationSequence.hh"
#include "PacMC/PmcReconstructionSequence.hh"
#include "PacMC/PmcSimulationSequence.hh"
#include "PacMC/PmcPhysicsSequence.hh"
#include "PacDisplay/PacDisplay.hh"
// tests
//#include "KalTests/KalTest.hh"
#include "BtaTupleMaker/BtuTupleMaker.hh"
#include "MuToEGamma/MuToEGammaAnalysis.hh"
#include "MuToEGamma/GfiSingleParticleOnCone.hh"
#include "MuToEGamma/GfiRandomEGammaOnCone.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

AppUserBuild::AppUserBuild( AppFramework* theFramework )
    : AppBuild( theFramework )
{
  //  main simulation sequence
  PmcSimulationSequence(this);
  PmcReconstructionSequence(this);
  // minimal physics sequence
  PmcPhysicsSequence(this);
  // actions
  PacActions(theFramework);
  // tracking tests
  //  add( new KalTest( "KalTest", "Test the Kalman Filter track fit" ) );

  //Display
  add(new PacDisplay("PacDisplay","Store Event Display Object" ));

  // Analysis
  add(new MuToEGammaAnalysis("MuToEGammaAnalysis", "mu to e gamma analysis module")); 
  add(new BtuTupleMaker( "BtuTupleMaker","Tuple maker module for users"));
  add(new GfiSingleParticleOnCone("GfiSingleParticleOnCone", "Single particle generator originating from a double-cone") );
  add(new GfiRandomEGammaOnCone("GfiRandomEGammaOnCone", "Two random positron and gamma originating from a double-cone") );
}

//--------------
// Destructor --
//--------------

AppUserBuild::~AppUserBuild( )
{
}
