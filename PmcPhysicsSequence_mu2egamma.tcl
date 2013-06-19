sequence create PmcPhysicsSequence

# Event Tags
sequence append PmcPhysicsSequence  TagCreateModule
module enable TagCreateModule

## Track selection: use post-TrkFixup cuts, modified for Pac
sourceFoundFile PacMC/PacGoodTrackSequence.tcl
sequence append PmcPhysicsSequence PacGoodTrackSequence
# neutrals
sourceFoundFile BetaMicro/BtaGoodTrackSequence.tcl
sequence append PmcPhysicsSequence BtaGoodTrackSequence

sequence append PmcPhysicsSequence GoodNeutralLooseAccSelection
catch { setProduction GoodNeutralLooseAccSelection }
talkto GoodNeutralLooseAccSelection {
    inputBtaCandidateList  set CalorNeutral
    outputBtaCandidateList set GoodNeutralLooseAcc
    outputMap              set GoodNeutralLooseAcc
    eRawCut                set 0.030
    nCrys                  set 0
    latShapeCut            set 1.1
    thetaMin               set 0.410
    thetaMax               set 2.409
# activate histogramming for this module
    histoFlag              set f
# specify the number of entries in the ntuple (-1 means no limit)
    maxEventTuple          set 0
#activate debugging histograms
    debugHistograms        set f
}

# Ks and Lambdas are used by the tag vertex algorithm
sourceFoundFile SimpleComposition/SmpProcs.tcl
#sourceFoundFile SimpleComposition/SmpKsProdSequence.tcl
#sequence append PmcPhysicsSequence SmpKsProdSequence
#sourceFoundFile CompositionSequences/CompV0Sequence.tcl
#sequence append PmcPhysicsSequence CompV0Sequence

# Pid sequence

#sourceFoundFile PacPid/PacPidSequence.tcl
#sequence append PmcPhysicsSequence PacPidSequence

# event tag
sequence create PacEventTagSequence
sequence append PmcPhysicsSequence PacEventTagSequence
sequence append PacEventTagSequence PmcEmulateBGF
mod enable PmcEmulateBGF
talkto PmcEmulateBGF {
    doHisto set false
}
sequence append PacEventTagSequence TagStoreEventShape
sequence append PacEventTagSequence TagStoreEventShapeAll
sequence append PacEventTagSequence TagStorePrimaryVtx
sequence append PacEventTagSequence TagStoreTrackInfo

talkto TagStoreEventShape {
  trkList set ChargedTracks
  chargedOnly set true
  sphericity set false
  suffix set ""
}

talkto TagStoreEventShapeAll {
  trkList  set ChargedTracksAcc
  neutList set GoodNeutralLooseAcc
  chargedOnly set false
  sphericity set true
  suffix set "All"
}

talkto TagStoreTrackInfo {
  trkList set ChargedTracks
}


