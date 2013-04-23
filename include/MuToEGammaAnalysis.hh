//--------------------------------------------------------------------------
// File and Version Information:
//      $Id$
//
// Description:
//      mu->egamma analysis module
//
// Author List:
//        Chih-hsiang Cheng           Caltech
// -------------------------------------------------------------------------
#ifndef MUTOEGAMMAANALYSIS_HH
#define MUTOEGAMMAANALYSIS_HH

#include "Riostream.h"
#include "Rtypes.h"

#include "Framework/AppModule.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "BbrGeom/BbrPointErr.hh"

class HepTupleManager;
class HepTuple;
class HepHistogram;
class BtaCandidate;
class PacDetector;
class DetElem;
class MuToEGammaAnalysis : public AppModule {

public:

  // Constructor
  MuToEGammaAnalysis ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~MuToEGammaAnalysis ();

  // Operations
  
  virtual AppResult            beginJob( AbsEvent* anEvent );
  virtual AppResult            event   ( AbsEvent* anEvent );
  virtual AppResult            endJob  ( AbsEvent* anEvent );

protected:

  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _muInputListName, _muOutputListName;

protected:
  BbrPointErr poca_on_target(const BbrPointErr* poca);

private:

  int _eid;
  HepTupleManager* _manager;
  //HepTuple* _ntuple;
  
  PacDetector *_detector;
  std::vector< DetElem* > _target_elems;
  
  HepHistogram *_hNinputmuons;
  HepHistogram *_hNintersection;

};
#endif
