set DetectorConfig MuToEGamma/pacrat_mu2egamma_pb8.xml
set HistFileName ntuples/mtutest.root
set UDECAYTcl MuToEGamma/single_part_gen_cone.tcl
#set PacDisplayFileName 
#set UseCollision yes
#set BeamConstraint yes
set NEVENTS 20000
set PRINTFREQ 100
#set MEGDEBUG 1
ErrLoggingLevel routine
sourceFoundFile MuToEGamma/mu2egamma_btm.tcl
