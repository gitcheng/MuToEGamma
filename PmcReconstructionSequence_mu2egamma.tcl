#
# Setup reconstruction sequence for PacMC.  This models all effects of the readout, triggering
# and reconstruction, including detector overlaps, and
# creation of fundamental (reco-based) BtaCandidate lists
#
#
sequence create PmcReconstructionSequence
# Simulate hit merging and after 
# simulated tracks are created but before fitting in reconstruction
sequence append PmcReconstructionSequence PmcMergeHits
#  disable this module because the functionality has been moved to PacReconstructTrk
module disable PmcMergeHits 

# Simulate Reconstruction effects
sequence append PmcReconstructionSequence PmcReconstruct
setProduction PmcReconstruct

# Initialize Beta strucutres
sequence append PmcReconstructionSequence BtaInitEvent
sequence append PmcReconstructionSequence BtaBuildEnv

# Build BtaCandidates for MCTruth after Reconstruction 
# to include also decay products of decays in flight and
# material interactions
mod talk BtaLoadMcCandidates
 requireGTrackList set t
exit

sequence append PmcReconstructionSequence BtaLoadMcCandidates

sequence append PmcReconstructionSequence PmcLoadRecoCandidates


sequence append PmcReconstructionSequence BtaLoadBeamSpot

if [ expr [info exists MCAssociator] && {$MCAssociator=="GHit"} ] {
  echo "PmcReconstructionSequence: using GHit associator"
  sequence append PmcReconstructionSequence BtaLoadGHitAssoc
  talkto BtaLoadGHitAssoc {
    production set true
    verbose set false
    outputMcAssoc set GHit
  }
  set PacPidTruthMap GHit
} else {
    echo "PmcReconstructionSequence: using Chisq associator"
    sequence append PmcReconstructionSequence BtaLoadChiSqAssoc
    mod talk BtaLoadChiSqAssoc
     outputMcAssoc set Chisq
    exit
    set PacPidTruthMap Chisq
}

