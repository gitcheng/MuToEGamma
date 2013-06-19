FwkCfgVar UDECAYfile ProdDecayFiles/B0B0bar_generic.dec
FwkCfgVar UGENERATE Upsilon(4S)
FwkCfgVar PACQADEBUG

FwkCfgVar DoDisplay no

set PmcAnalysis PacQMonSVT

sourceFoundFile MuToEGamma/PacMC_mu2egamma.tcl

# Customize generator
disableGenerators 0
module enable GfiEvtGen

set ffields [ split $UDECAYfile . ]
set suffix [ lindex $ffields [ expr [ llength $ffields ] - 1 ] ]

if { $suffix == "dec" } {
    talkto GfiEvtGen {
        GENERATE set $UGENERATE
        DECAY    set PacMC/DECAY.DEC
        UDECAY   set $UDECAYfile
        show
    }
} elseif { $suffix == "tcl" } { 
    echo "Source $UDECAYfile"
    sourceFoundFile $UDECAYfile
} else {
    ErrMsg fatal "unknown UDECAYfile suffix   $suffix"  
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

if { $DoDisplay == "yes" } {
    mod enable PacDisplay
    seq append MyAnalysisSequence PacDisplay
    talkto PacDisplay {
        saveGen set true
        saveSim set true
        saveRec set true
        saveDet set true
        saveHit set true
        saveEmc set false
        resolution set 200
    }
}

talkto PacQMonSVT {
    FitList set PmcRecoTracks
    evtSkip set 1
    HyposToTest set electron
    InnerInfo  set true
    show
}

talkto GfiEvtGen {
    UDECAY set MuToEGamma.dec
    show
}


path list


if { [ info exists NEVENTS ] && ${NEVENTS} > 0 } {
    ev begin -nev $NEVENTS
    ErrMsg trace "completed OK"
    exit
}
