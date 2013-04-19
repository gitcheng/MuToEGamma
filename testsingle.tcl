set HistFileName MuToEGamma/testsingle.root
set PacDisplayFileName MuToEGamma/display_testsingle.root
set NEVENTS 2
set PRINTFREQ 1
set PACQADEBUG 1
ErrLoggingLevel routine
set ParticlePDT e+
set SingleMomentumLo 0.010
set SingleMomentumHi 0.020
set CosThetamin 0.5
set CosThetamax 0.6
set SinglePhimin 2.5
set SinglePhimax 3.8
set usePt true
sourceFoundFile PacQA/scripts/mu2egamma/mu2egamma.tcl
