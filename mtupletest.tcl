set DetectorConfig MuToEGamma/pacrat_mu2egamma_pb8.xml
set HistFileName ntuples/mtutest.root
set UDECAYTcl MuToEGamma/single_part_gen_cone.tcl
set TCone1Z -2.99
set TCone2Z 2.99
set TConeZ0 0
set TConeR 0.49
#set PacDisplayFileName display_pb8.root
#set UseCollision yes
#set BeamConstraint yes
set NEVENTS 10000
set PRINTFREQ 1
set MEGDEBUG 1
ErrLoggingLevel routine
sourceFoundFile MuToEGamma/mu2egamma_btm.tcl
