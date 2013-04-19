#
# Main tcl for running PacMC.  You should not need to edit this file, instead
# override the FwkCfgVar variables below inside a framework snippet to configure
# your job.
#----------------------------------------------------------------------

path delete AllPath
path create PacMC


## Simulation configuration parameters
FwkCfgVar DetectorConfig MuToEGamma/pacrat_mu2egamma.xml
FwkCfgVar BeamConfig MuToEGamma/mu2eg_Beams.tcl

## basic confguration parameters
FwkCfgVar HistFileName PacMC.root ;# set the name of the histogram file
FwkCfgVar MCAssociator "GHit"

## production information
FwkCfgVar RUNNUM 1000
## note that we limit production to 10^6 events/run
FwkCfgVar FIRSTEVT 1

FwkCfgVar PRINTFREQ 1000
# set this to 'routine' to get configuration output
FwkCfgVar PacErrLogging warning
ErrLoggingLevel $PacErrLogging

#physics configuration parameters
#FwkCfgVar BTaggerName Tag04; # Tag04 or Tag08
FwkCfgVar ChargedHadrons ChargedTracks

# Global variable used to let the framework know whether
# the PacPidFilterTrackSequence has already been included
# in a sequence.
# FwkCfgVar PacPidFilterTrackSequenceRun "No"

# setup tagging sequences, and the proc for creating analysis-specific tagging
sourceFoundFile SimpleComposition/SmpProcs.tcl
#sourceFoundFile PacMC/PmcTagVertexSequence.tcl

# generic tuple-maker configuration: this will be propagated to all the clones
talkto BtuTupleMaker {
  truthAssociator set $MCAssociator
#  tagvtxAlgorithm set BtgBTagger${BTaggerName}
}

# analysis configuration.  If this is a simple name, it's interpreted as a module
# (which must be configured in the snipet).  If it's a path/name it's interpreted as

# a sequence, which is configured and appended
FwkCfgVar PmcAnalysis PacMC/PmcDummyAnalysis

# a sequence or module capable of filtering at the simtrack level

FwkCfgVar SimFilter ""

# analysis simple composition module
FwkCfgVar PmcSmpModule MySmpModule

# configure geometry
talkto PacBuildEnvModule {
	#
	# define the pac geometry input file 
	#
	ConfigFile set $DetectorConfig
}

# setup basic simulation sequence
#
ErrMsg trace "before configuring simulation sequence"
sourceFoundFile PacMC/PmcSimulationSequence.tcl
path append PacMC PmcSimulationSequence
ErrMsg trace "done configuring simulation sequence"

# setup reconstruction sequence
#
sourceFoundFile PacMC/PmcReconstructionSequence_mu2egamma.tcl
path append PacMC PmcReconstructionSequence
ErrMsg trace "done configuring reconstruction sequence"

# setup basic physics sequence
#
sourceFoundFile PacMC/PmcPhysicsSequence_mu2egamma.tcl
path append PacMC PmcPhysicsSequence
ErrMsg trace "done configuring physics sequence"

#  Now configure the modules
# Beam
sourceFoundFile $BeamConfig
ErrMsg trace "done configuring beam"

talkto PacBuildEnvModule {
  ConfigFile set $DetectorConfig
}

# tuple output
talkto RooTupleEnv {
    histFileName set $HistFileName
}

talkto BtuTupleMaker {
    truthAssociator set $MCAssociator
# turn off verbose comments
    verbose set false
    production set true
}

if { ! [ ErrLogging routine ] } {

  talkto PmcSimulate {
    verbose set false
    production set true
  }

  talkto BtaLoadMcCandidates {
    verbose set false
    production set true  
  }

  mod disable StdHepPrint
}

ErrMsg trace "done configuring modules"

# create the anlysis sequence.  This is empty to start with

sequence create MyAnalysisSequence
path append PacMC MyAnalysisSequence

# Setup users Analysis.  A simple name is taken as a module, a composite name as a sequence.
#  Note that the sequence name MUST be the same as the sequence.tcl file.

if { $PmcAnalysis != "" } {
  set parts [split $PmcAnalysis "/"]

  if { [llength $parts] > 1 } {
     # sequence: source the tcl
    sourceFoundFile ${PmcAnalysis}.tcl

  }

# strip out the directory before appending the sequence
  sequence append MyAnalysisSequence [lindex $parts end]  
}
## Apply PacSimTrack level filter
if { $SimFilter != "" } {
  ErrMsg trace "Inserting sim filter $SimFilter"
  path insert PacMC -before PmcReconstructionSequence $SimFilter
}

ErrMsg routine "done configuring sequence"
# disable unwanted action modules
action disable all
action enable BtaSetEventCacheAction
action enable "ErrLogger Action"
if { [ ErrLogging routine ] } {
  action enable SumTimeAction
}


#disable unwanted modules
module disable PacDisplay
module disable TrkEffTableCreator
