set DetectorConfig MuToEGamma/pacrat_mu2egamma_pb8.xml
set HistFileName ntuples/mu2egtest_pb8-randomeg.root
set UDECAYTcl MuToEGamma/random_egamma_gen_cone.tcl
set PacDisplayFileName ntuples/display_events_pb8_randomeg.root
set Particle1PDT e+
set Particle2PDT e-
set SingleMomentum 0.05283
set BackToBack true
set TCone1Z -3.0
set TCone2Z 3.0
set TConeZ0 0
set TConeR 0.5
set NEVENTS 30
set PRINTFREQ 1
set MEGDEBUG 1
ErrLoggingLevel routine
sourceFoundFile MuToEGamma/mu2egamma_btm.tcl
