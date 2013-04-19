FwkCfgVar ParticlePDT mu+
FwkCfgVar SingleMomentum 0.0
FwkCfgVar CosThetamin -1
FwkCfgVar CosThetamax +1
FwkCfgVar SingleMomentumLo
FwkCfgVar SingleMomentumHi
FwkCfgVar usePt false
FwkCfgVar SinglePhimin
FwkCfgVar SinglePhimax
FwkCfgVar TCone1Z -1.99
FwkCfgVar TCone2Z 1.99
FwkCfgVar TConeZ0 0
FwkCfgVar TConeR 0.49

if { ! [info exists SingleMomentumLo] } {
    set SingleMomentumLo $SingleMomentum
}
if { ! [info exists SingleMomentumHi] } {
    set SingleMomentumHi $SingleMomentum
}

disableGenerators 0
seq append GfiSequence -a GfiSingleParticle GfiSingleParticleOnCone
module enable GfiSingleParticleOnCone
module talk GfiSingleParticleOnCone
      GENERATE set $ParticlePDT
      Pmin set $SingleMomentumLo
      Pmax set $SingleMomentumHi
      usePt set $usePt
      CosThetamin set $CosThetamin
      CosThetamax set $CosThetamax
if [ info exists Phimin ] {
    Phimin set $SinglePhimin
}
if [ info exists Phimax ] {
    Phimax set $SinglePhimax
}
      cone1z set $TCone1Z
      cone2z set $TCone2Z
      conez0 set $TConeZ0
      coneR  set $TConeR

       show
exit
