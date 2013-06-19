set DetectorConfig MuToEGamma/pacrat_mu2egamma_pb8.xml
set HistFileName ntuples/mu2egtest_pb8.root
set UDECAYTcl MuToEGamma/single_part_gen_cone.tcl
#set PacDisplayFileName ntuples/display_events_pb8.root
set TCone1Z -2.99
set TCone2Z 2.99
set TConeZ0 0
set TConeR 0.49
set NEVENTS 2500
set PRINTFREQ 1
#set MEGDEBUG 1
ErrLoggingLevel routine
#sourceFoundFile PacQA/scripts/mu2egamma/mu2egamma.tcl
sourceFoundFile MuToEGamma/mu2egamma_btm.tcl
