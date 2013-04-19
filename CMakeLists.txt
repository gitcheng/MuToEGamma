string(REGEX REPLACE "^.*/([^ ]+)$" "\\1" pkgname ${CMAKE_CURRENT_SOURCE_DIR})

#
# Set specific compiler flags for the package
#
set(${pkgname}_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${pkgname}_CXX_FLAGS}")

configPkg(${pkgname})

#
# Generate library
#
add_library(${pkgname} ${Sources})
set(LIB_LINK_LIST
AbsCalo
AbsEnv
AbsEvent
AbsParm
AssocTools
BaBar
BbrGeom
BetaCoreTools
BetaMC
BetaMicroAdapter
Beta
CLHEP
DetectorModel
ErrLogger
Framework
G3Data
GenEnv
GUtils
HepTuple
m
PacDirc
PacEmc
PacGeom
PacIfr
PacMC
PacSim
PacTrk
PDT
ProbTools
ProxyDict
RecoData
ROOT_Core
ROOT_Hist
ROOT_Matrix
ROOT_Physics
ROOT_Tree
TrkBase
TrkEnv
)
SetSpecialLib(LIB_LINK_LIST)
target_link_libraries(${pkgname} ${LIB_LINK_LIST})

get_target_property(tmp app_main_obj LOCATION)
if(NOT tmp STREQUAL "tmp-NOTFOUND")
    set(FMK_OBJ_DIR ${tmp})
endif()

add_executable(MuToEGammaApp bins/MuToEGammaApp.cc)
set(BIN_LINK_LIST ${pkgname}
BaBar
ErrLogger
Framework
KalTests
PacDisplay
PacMC
BtaTupleMaker
)
target_link_libraries(MuToEGammaApp ${FMK_OBJ_DIR}/AppMain.o ${BIN_LINK_LIST})
add_dependencies(MuToEGammaApp app_main_obj)

add_test(tMuToEGammaApp ${EXECUTABLE_OUTPUT_PATH}/MuToEGammaApp)

list(APPEND ExeList MuToEGammaApp)
set(ExeList "${ExeList}" PARENT_SCOPE)

#
# Define custom target to limit the build to libraries or binaries
#
add_custom_target(${pkgname}bin DEPENDS MuToEGammaApp)
add_custom_target(${pkgname}lib DEPENDS ${pkgname})
