FwkCfgVar UDECAYfile MuToEGamma/MuToEGamma.dec
FwkCfgVar UDECAYTcl MuToEGamma/single_part_gen.tcl
FwkCfgVar UGENERATE continuum
FwkCfgVar PACQADEBUG
FwkCfgVar PacDisplayFileName

set PmcAnalysis PacQMonSVT

sourceFoundFile PacMC/PacMC_mu2egamma.tcl

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

if [ info exists PACQADEBUG ] {
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

talkto PacQMonSVT {
    FitList set PmcRecoTracks
    evtSkip set 1
    HyposToTest set electron
    InnerInfo  set true
    show
}


path list


if { [ info exists NEVENTS ] && ${NEVENTS} > 0 } {
    ev begin -nev $NEVENTS
    ErrMsg trace "completed OK"
    exit
}
