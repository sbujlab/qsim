#----------------------------------------------------------------------------

# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(QSIM)


#ROOT path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(ROOT REQUIRED)

# Update header file
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/gitinfo.hh COMMAND ${PROJECT_SOURCE_DIR}/pullgitinfo.py ${PROJECT_SOURCE_DIR} COMMENT "Creating gitinfo.hh" )
# this is to ensure we find the header we need
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

ROOT_GENERATE_DICTIONARY(
    qsimDict                         # path to dictionary to generate
    "include/qsimRunData.hh"         # list of classes to process
    "include/qsimTextFile.hh"
    LINKDEF "include/qsimLinkDef.h"  # ROOT linkDef file
)

#----------------------------------------------------------------------------
# Find Boost libraries, in particular iostreams

if(USE_BOOST)
    find_package(Boost COMPONENTS iostreams)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIR})
    ADD_DEFINITIONS(-D__USE_BOOST)
    if(Boost_IOSTREAMS_FOUND)
        ADD_DEFINITIONS(-D__USE_BOOST_IOSTREAMS)
    endif()
endif()


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})


#debugging symbols, complains
#set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -g -Wno-shadow -Wno-pedantic")
#----------------------------------------------------------------------------
# Generate dictionaries as well (so we can write objects to ROOT files

#include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/qsim*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB libsources ${PROJECT_SOURCE_DIR}/src/qsimRunData.cc ${PROJECT_SOURCE_DIR}/src/qsimTextFile.cc)
file(GLOB libheaders ${PROJECT_SOURCE_DIR}/include/*.hh)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(qsim qsim.cc ${CMAKE_CURRENT_BINARY_DIR}/include/gitinfo.hh ${sources} ${headers} qsimDict.cxx)
#add_executable(qsim qsim.cc ${sources} ${headers})
add_library(qsimroot SHARED ${CMAKE_CURRENT_BINARY_DIR}/include/gitinfo.hh ${libsources} ${libheaders} qsimDict.cxx)
target_link_libraries(qsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
target_link_libraries(qsimroot ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )

#add_executable(qsim qsim.cc ${CMAKE_CURRENT_BINARY_DIR}/include/gitinfo.hh ${sources} ${headers} qsimDict.cxx)
#add_library(qsimroot SHARED ${CMAKE_CURRENT_BINARY_DIR}/include/gitinfo.hh ${libsources} ${libheaders} qsimDict.cxx)
#target_link_libraries(qsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
#target_link_libraries(qsimroot ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build qsim. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

#FILE( GLOB QSIM_GEO "geometry/*" )
#FILE( GLOB QSIM_GEO "geometry_perl/*" )

set(QSIM_SCRIPTS
    geometry_perl/detector.gdml
    geometry_perl/materialsNew.xml
    geometry_perl/qsimMother.gdml
    geometry_perl/solids.xml
    geometry_perl/schema/gdml.xsd
    geometry_perl/schema/gdml_core.xsd
    geometry_perl/schema/gdml_define.xsd
    geometry_perl/schema/gdml_extensions.xsd
    geometry_perl/schema/gdml_materials.xsd
    geometry_perl/schema/gdml_parameterised.xsd
    geometry_perl/schema/gdml_replicas.xsd
    geometry_perl/schema/gdml_solids.xsd
    geometry/schema/gdml.xsd
    geometry/schema/gdml_core.xsd
    geometry/schema/gdml_define.xsd
    geometry/schema/gdml_extensions.xsd
    geometry/schema/gdml_materials.xsd
    geometry/schema/gdml_parameterised.xsd
    geometry/schema/gdml_replicas.xsd
    geometry/schema/gdml_solids.xsd
    primaryDistribution.root
    macros/vis.mac
    vis/OIX.mac
    vis/Qt.mac
    vis/vis.mac
    vis/X.mac
    macros/runexample.mac
    macros/gui.mac
    macros/vis.mac
    macros/batchrun.mac
    macros/runscript.sh
    macros/handler.sh
	macros/merger.sh
    rootlogon.C
)

foreach(_script ${QSIM_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(QSIM DEPENDS qsim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS qsim DESTINATION bin)


