FwkCfgVar UDECAYfile MuToEGamma/MuToEGamma.dec
FwkCfgVar UDECAYTcl MuToEGamma/single_part_gen.tcl
FwkCfgVar UGENERATE continuum
FwkCfgVar MEGDEBUGALL
FwkCfgVar MEGDEBUG
FwkCfgVar PacDisplayFileName
FwkCfgVar BeamConstraint

set PmcAnalysis BtuTupleMaker

sourceFoundFile MuToEGamma/PacMC_mu2egamma.tcl

# create converted gamma
#sourceFoundFile SimpleComposition/SmpGammaConvProdSequence.tcl
#seq append PmcPhysicsSequence SmpGammaConvProdSequence

mod clone SmpMakerDefiner GammaConversion
seq append PmcPhysicsSequence GammaConversion
talkto GammaConversion {
    decayMode            set "gamma -> e+ e-"
    daughterListNames    set "ChargedTracks"
    daughterListNames    set "ChargedTracks"
    preFitSelectors      set "Mass 0:0.5"
    postFitSelectors     set "Mass 0:0.03"
    postFitSelectors     set "RhoBabar 0.8:"
    postFitSelectors     set "ChiSq :15849"
    postFitSelectors     set "VtxSuccess"
    fittingAlgorithm     set "GammaParallel"
}

# build mu -> e gamma
mod clone SmpMakerDefiner muToEGamma
seq append PmcPhysicsSequence muToEGamma
talkto muToEGamma {
    decayMode  set "mu+ -> e+ gamma"
    daughterListNames set ChargedTracks
    daughterListNames set GammaConversion
    fittingAlgorithm  set "TreeFitter"
    if { [ info exists BeamConstraint ] && $BeamConstraint=="yes"} {
	fitConstraints    set "Beam"
	fitSettings       set UpdateDaughters
	#fitSettings       set InvalidateFit
    }
    show
}

# Refit with target constraint
seq append PmcPhysicsSequence MuToEGammaAnalysis
talkto MuToEGammaAnalysis {
    muInputListName  set muToEGamma
    muOutputListName set muToEGammaRefit
    if [ info exists MEGDEBUG ] {
	verbose set t
    }
    show
}


# Customize generator
disableGenerators 0
module enable GfiEvtGen

if [ info exists UDECAYfile ] {
    talkto GfiEvtGen {
	GENERATE set $UGENERATE
	DECAY    set PacMC/DECAY.DEC
	UDECAY   set $UDECAYfile
	show
    }
}
if [ info exists UDECAYTcl ] {
    echo "Sourcing $UDECAYTcl"
    sourceFoundFile $UDECAYTcl
}

if [ info exists MEGDEBUGALL ] {
    set verbosemodules { PmcSimulate PmcReconstruct PmcLoadRecoCandidates }
    foreach mm $verbosemodules {
        talkto $mm {
            production set f
            verbose set t
            show
        }
    }
    talkto BtaLoadMcCandidates {
	production set f
	show
    }
}


action disable SumTimeAction

if [ info exists PacDisplayFileName ] {
    mod enable PacDisplay
    seq append MyAnalysisSequence PacDisplay
    talkto PacDisplay {
        saveGen set true
        saveSim set true
        saveRec set true
        saveDet set true
        saveHit set true
        saveEmc set false
        stepsize set 0.2
	fileName set $PacDisplayFileName
    }
}

talkto BtuTupleMaker {
    listToDump  set muToEGammaRefit
    fillMC set true
    mcBlockContents  set "Mass Momentum Vertex"
    ntpBlockConfigs  set "mu+  mu   2  10"
    ntpBlockConfigs  set "gamma gamma 2 20"
    ntpBlockConfigs  set "e+ e 0 20" 
    ntpBlockContents set "mu : MCIdx Mass Vertex VtxChi2 Momentum"
    ntpBlockContents set "e : MCIdx Momentum"
    ntpBlockContents set "gamma : MCIdx Mass Momentum Vertex VtxChi2"

    show
}

path list


if { [ info exists NEVENTS ] && ${NEVENTS} > 0 } {
    ev begin -nev $NEVENTS
    ErrMsg trace "completed OK"
    exit
}
