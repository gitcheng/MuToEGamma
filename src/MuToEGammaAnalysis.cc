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
#include "BetaRecoAdapter/BtaAbsRecoObject.hh"

#include "DetectorModel/DetSet.hh"
#include "DetectorModel/DetElem.hh"
#include "DetectorModel/DetElemList.hh"
#include "DetectorModel/DetIntersection.hh"
#include "PacGeom/PacDetector.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacConeDetElem.hh"
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
      std::string ename= (*ielem)->elementName();
      if ( 0!= ename.compare(0,6,"Target") ) continue; // name should start with "Target"
      _target_elems.push_back( *ielem );
      ErrMsg(warning) << "Found detector element: " << ename << " id " << elemid
		      << endmsg;
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

  // origin
  static const HepPoint pos_origin(0,0,0);
  
  for (vector<BtaCandidate*>::const_iterator i = mulist->begin(), e = mulist->end(); i != e; ++i) {
    BtaCandidate* mu= *i;

    const BtaCandidate* positron = mu->getDaughter(PdtLund::e_plus);
    const BtaCandidate* gamma= mu->getDaughter(PdtLund::gamma);
    if ( !positron or !gamma ) continue;

    // Find the poca of the positron track to the z axis
    BbrPointErr poca= positron->recoObject()->positionErr(pos_origin, BtaAbsRecoObject::XY);

    // positron track
    const TrkRecoTrk *trk= positron->recoTrk();
    const TrkFit* fit= trk->fitResult();
    if (!fit) {
      ErrMsg(warning) << "No fit for this track?" << endmsg;
      continue;
    }

    //cout << "poca= " << poca << endl;

    // for (int i=0; i<50; i++) {
    //   double flt= -1+ i*2/50.0;
    //   HepPoint pos= fit->position(flt);
    //   cout << flt << ", " << pos.x() << ", " << pos.y() << ", " << pos.z() << ", " << endl;
    // }

    // cout << "Trajectory " << endl;
    // fit->traj().printAll(cout);

    // Find the intersect between a trajectory and target element.
    int nint=0;
    double minchi2=1e16;
    BtaCandidate *fittedmu(0);
    vector<BbrPointErr> pnterrs; // possible muon decay constrain points
    for ( unsigned i=0; i<_target_elems.size(); i++) {
      // cout << " element name= " << _target_elems[i]->elementName() 
      // 	   << " type " << _target_elems[i]->detectorType()->typeName() << endl;
      double pathlen= -1.0;
      const double pathlenmax=5;
      const double epsilon= 0.01;
      int jflag=1;
      if ( _verbose.value() ) {
	ErrMsg(debugging) << "target element #" << i << endmsg;
      }
      while ( jflag!=0 && pathlen < pathlenmax ) {
	DetIntersection dinter(_target_elems[i], &fit->traj(), pathlen+epsilon, pathlenmax);
	//const PacDetElem* pelem= dynamic_cast<const PacDetElem*>( _target_elems[i] );
	jflag= _target_elems[i]->intersect(&fit->traj(), dinter);
	//const TrkGeomTraj *tgtrj= dynamic_cast<const TrkGeomTraj*>(&fit->traj());
	//jflag= pelem->intersect(tgtrj, dinter);
	pathlen= dinter.pathlen;
	if ( _verbose.value() ) {
	  ErrMsg(debugging) << " jflag= " << jflag << " pathlen= " << pathlen
			    << endmsg;
	}  
	if ( jflag ) {
	  nint++;
	  BbrPointErr poserr= fit->positionErr(pathlen);
	  pnterrs.push_back( poserr );
	}
      }
    }

    // If no intersections are found, use the point on the cone closest to poca
    if ( pnterrs.size() == 0 ) {
      pnterrs.push_back( poca_on_target(&poca) );
    }
    // Find the best fit
    BtaOpMakeTree comb;
    for (vector<BbrPointErr>::const_iterator i= pnterrs.begin(), e = pnterrs.end(); i != e; ++i) {
      BbrPointErr poserr= *i;
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
	if ( fittedmu != 0 ) {
	  delete fittedmu;
	  fittedmu=0;
	}
	fittedmu= new BtaCandidate(updmu);
      }
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


BbrPointErr MuToEGammaAnalysis::poca_on_target(const BbrPointErr *poca) {
  // FIXME. Serious kludge

  BbrPointErr retval = *poca;

  int i0(-1);
  double zmin(1000), zmax(-1000);
  for ( unsigned i=0; i<_target_elems.size(); i++) {
    const PacDetElem *pelem= dynamic_cast<const PacDetElem*>(_target_elems[i]);
    if (  pelem->surfaceType() != PacDetElem::cone ) {
      ErrMsg(fatal) << "Only knows how to deal with cones" << endmsg;
    }
    const PacConeDetElem* cone= dynamic_cast<const PacConeDetElem*>(_target_elems[i]);
    double r1= cone->coneType()->lowR() * cone->cone()->sinTheta();
    double r2= cone->coneType()->hiR() * cone->cone()->sinTheta();
    double z1= r1/cone->tanTheta()+ cone->zVertex();
    double z2= r2/cone->tanTheta()+ cone->zVertex();

    if ( z1 < zmin ) zmin=z1;
    if ( z2 < zmin ) zmin=z2;
    if ( z1 > zmax ) zmax=z1;
    if ( z2 > zmax ) zmax=z2;

    if ( poca->z() <= std::max(z1,z2) && poca->z() > std::min(z1,z2) ) {
      i0= i;
      break;
    }
  }

  if ( i0 < 0 ) {  // poca z is outsize the cones
    // Set the point to the vertex
    retval.setX(0);
    retval.setY(0);
    if ( poca->z() > zmax ) retval.setZ(zmax);
    else retval.setZ(zmin);
  } else {
    const PacConeDetElem* cone= dynamic_cast<const PacConeDetElem*>(_target_elems[i0]);
    double rc= cone->radius( poca->z() );
    retval.setX( rc * poca->x() / poca->perp() );
    retval.setY( rc * poca->y() / poca->perp() );
  }

  // ErrMsg(warning) << *poca << endmsg;
  // ErrMsg(warning) << retval << endmsg;
  return retval;
}
