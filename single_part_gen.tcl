FwkCfgVar ParticlePDT mu+
FwkCfgVar SingleMomentum 0.0
FwkCfgVar CosThetamin -1
FwkCfgVar CosThetamax +1
FwkCfgVar SingleMomentumLo
FwkCfgVar SingleMomentumHi
FwkCfgVar usePt false
FwkCfgVar SinglePhimin
FwkCfgVar SinglePhimax
FwkCfgVar UseCollision

if { ! [info exists SingleMomentumLo] } {
    set SingleMomentumLo $SingleMomentum
}
if { ! [info exists SingleMomentumHi] } {
    set SingleMomentumHi $SingleMomentum
}

disableGenerators 0
module enable GfiSingleParticle
module talk GfiSingleParticle
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
if { [ info exist UseCollision ] && $UseCollision=="yes" } {
      useCollision set $UseCollision
} else {
      ipX set 0.0
      ipY set 0.0
      ipZ set 0.0
      ipT set 0.0
}
       show
exit
