FwkCfgVar Particle1PDT e+
FwkCfgVar Particle2PDT gamma
FwkCfgVar SingleMomentum 0.0
FwkCfgVar CosThetamin -1
FwkCfgVar CosThetamax +1
FwkCfgVar SingleMomentumLo
FwkCfgVar SingleMomentumHi
FwkCfgVar SinglePhimin
FwkCfgVar SinglePhimax
FwkCfgVar TCone1Z -2.99
FwkCfgVar TCone2Z 2.99
FwkCfgVar TConeZ0 0
FwkCfgVar TConeR 0.49
FwkCfgVar BackToBack true

if { ! [info exists SingleMomentumLo] } {
    set SingleMomentumLo $SingleMomentum
}
if { ! [info exists SingleMomentumHi] } {
    set SingleMomentumHi $SingleMomentum
}

disableGenerators 0
seq append GfiSequence -a GfiSingleParticle GfiRandomEGammaOnCone
module enable GfiRandomEGammaOnCone
module talk GfiRandomEGammaOnCone
      p1name set $Particle1PDT
      p2name set $Particle2PDT
      Pmin set $SingleMomentumLo
      Pmax set $SingleMomentumHi
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

      backtoback set $BackToBack
       show
exit
