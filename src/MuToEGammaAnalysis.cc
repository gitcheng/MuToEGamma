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
#include "BaBar/BaBar.hh"
#include "MuToEGamma/MuToEGammaAnalysis.hh"

//---------------
// C++ Headers --
//---------------
#include <math.h>

#include <vector>
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "GenEnv/GenEnv.hh"

#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"
#include "ProxyDict/Ifd.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtLund.hh"
#include "BaBar/Constants.hh"
#include "BbrGeom/BbrPointErr.hh"

#include "DetectorModel/DetSet.hh"
#include "DetectorModel/DetElem.hh"
#include "DetectorModel/DetElemList.hh"
#include "DetectorModel/DetIntersection.hh"
#include "PacGeom/PacDetector.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacMeasurement.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkPocaXY.hh"
#include "TrkBase/TrkDifTraj.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
using namespace std;

//----------------
// Constructors --
//----------------
MuToEGammaAnalysis::MuToEGammaAnalysis(const char* const theName,
				       const char* const theDescription )
  : AppModule( theName, theDescription )
  , _eventInfoList("eventInfoList", this, "Default" )
  , _muInputListName("muInputListName", this, "")
  , _muOutputListName("muOutputListName", this, "")
  , _eid(0), _manager(0), _detector(0)
{
  commands()->append( &_eventInfoList );
  commands()->append( &_muInputListName);
  commands()->append( &_muOutputListName);
}


//--------------
// Destructor --
//--------------
MuToEGammaAnalysis::~MuToEGammaAnalysis()
{
}

//--------------
// Operations --
//--------------
AppResult
MuToEGammaAnalysis::beginJob( AbsEvent *anEvent)
{
    ErrMsg(debugging) << name() << ": beginJob" << endmsg;

    HepTupleManager* _manager = gblEnv->getGen()->ntupleManager();
    assert(_manager != 0);
    _hNinputmuons= _manager->histogram("nMus",5, -0.5, 4.5);
    _hNintersection= _manager->histogram("nInts", 5, -0.5, 4.5);

    return AppResult::OK;
}

AppResult
MuToEGammaAnalysis::endJob( AbsEvent *anEvent)
{
    ErrMsg(debugging) << name() << ": endJob" << endmsg;
    return AppResult::OK;
}

AppResult
MuToEGammaAnalysis::event( AbsEvent *anEvent )
{
  _eid++;
  
  // Search for Target detector element
  if ( 0== _detector ) {
    _detector=Ifd<PacDetector>::get(gblPEnv, "Tracking Det" );
    if (! _detector) {
      ErrMsg(fatal) << "Can't get the detector model" << endmsg;
    }

    DetElemList elements; // list of DetectorElements
    _detector->detectorModel()->listAllElements(elements);
    
    DetElemList::const_iterator ielem = elements.begin();

    for (; ielem != elements.end(); ielem++ ) {
      int elemid= (*ielem)->elementNumber() ;
      if ( elemid % 100 == 0 ) { // Only pick the element id that ends with 00
        std::string ename= (*ielem)->elementName();
        if ( 0!= ename.compare(0,6,"Target") ) continue; // name should start with "Target"
        _target_elems.push_back( *ielem );
        ErrMsg(warning) << "Found " << ename << endmsg;
      }
    }

    assert(_target_elems.size() >= 1);
  }

  assert(_detector);

  // Get the muon list
  vector<BtaCandidate*>* mulist = 
    Ifd<vector< BtaCandidate*> >::get(anEvent, _muInputListName.value() );
  if (mulist == 0 ) {
    ErrMsg(fatal) << "Could not locate list " <<  _muInputListName.value() 
		  << ". This is likely a configuration error." << endmsg;
  }
  _hNinputmuons->accumulate( mulist->size() );
    
  // Output list
  vector<BtaCandidate*>* outputMuons = new vector<BtaCandidate*> ;
  if ( !Ifd<vector<BtaCandidate*> >::put(anEvent, outputMuons, _muOutputListName.value() ) ) {
    ErrMsg(fatal) << "Could not put list " <<  _muOutputListName.value() 
		  << ". This is likely a configuration error." << endmsg;
  }
  
  for (vector<BtaCandidate*>::const_iterator i = mulist->begin(), e = mulist->end(); i != e; ++i) {
    BtaCandidate* mu= *i;

    const BtaCandidate* positron = mu->getDaughter(PdtLund::e_plus);
    const BtaCandidate* gamma= mu->getDaughter(PdtLund::gamma);
    if ( !positron or !gamma ) continue;

    // positron track
    const TrkRecoTrk *trk= positron->recoTrk();
    const TrkFit* fit= trk->fitResult();
    if (!fit) {
      ErrMsg(warning) << "No fit for this track?" << endmsg;
      continue;
    }

    // Find the intersect between a trajectory and a det element.
    int nint=0;
    double minchi2=1e16;
    BtaCandidate *fittedmu(0);
    for ( unsigned i=0; i<_target_elems.size(); i++) {
      DetIntersection dinter(_target_elems[i], &fit->traj(), -20, 20);
      int jflag=  _target_elems[i]->intersect(&fit->traj(), dinter);
      if (! jflag ) continue;
      nint++;
      double pathlen= dinter.pathlen;
      BbrPointErr poserr= fit->positionErr(pathlen);

      BtaOpMakeTree comb;
      BtaCandidate* cand= comb.create(*positron, *gamma);
      cand->setType( mu->pdtEntry() );
      setGeoConstraint(*cand);
      setBeamConstraint(*cand, poserr);

      // Fitter
      VtxAbsAlgorithm::algId algo = VtxAbsAlgorithm::TreeFitter;
      VtxFitterOper fitter(*cand, algo);
      fitter.fit();
      BtaCandidate updmu= fitter.getFittedTree(); // all candidates in tree updated
      double thischi2= updmu.decayVtx()->chiSquared();
      if ( thischi2 < minchi2 ) {
	minchi2= thischi2;
	fittedmu= new BtaCandidate(updmu);
      }
      //delete cand;
    }

    _hNintersection->accumulate(nint);

    if ( fittedmu != 0 ) {
      if ( _verbose.value() ) {
	ErrMsg(debugging) << " pdt=  " << fittedmu->pdtEntry()->name() << endmsg;
      }
      outputMuons->push_back(fittedmu);
    }

    if ( _verbose.value() ) {
      ErrMsg(debugging) << "Found " << nint << " intersections between positron and target" << endmsg;
    }

  }

  return AppResult::OK;
}


