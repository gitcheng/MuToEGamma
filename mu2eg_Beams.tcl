ErrMsg warning "Using MuToEGamma beam configuration"

mod talk PepBuildEnv
  pepFillPatternFile      set PepCond/pepFillPattern.raw
  pepPackedBunchesFile    set PepCond/pepPackedBunches.raw
  pepBeamSpotCalFile      set MuToEGamma/mu2egBeamSpotCal.raw
  pepBeamSpotCalMCGenFile set MuToEGamma/mu2egBeamSpotCal.raw
  pepBeamMomentaFile      set MuToEGamma/mu2egBeamMomenta.raw
  show
exit
