cmake_minimum_required(VERSION 2.8)

project( QLabelling )

if(NOT CMAKEMODULES_PATH)
    set(CMAKEMODULES_PATH $ENV{CMAKEMODULES_PATH})
endif()
if(NOT CMAKEMODULES_PATH)
    if(WIN32)
        set(CMAKEMODULES_PATH C:/SDK/CMakeModules)
    else()
        set(CMAKEMODULES_PATH /opt/local/share/CMakeModules)
    endif()
endif()
file(TO_CMAKE_PATH ${CMAKEMODULES_PATH} CMAKEMODULES_PATH)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKEMODULES_PATH} ${CMAKE_SOURCE_DIR})

if(WIN32)
	add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif()

set( DEFAULT_LABEL_PATH ${CMAKE_SOURCE_DIR}/rc/labels_facade_simple.txt )
configure_file( ${CMAKE_SOURCE_DIR}/config.hpp.cmake.in ${CMAKE_SOURCE_DIR}/config.hpp )

# Qt
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# CGAL
find_package( CGAL COMPONENTS Core Qt4 REQUIRED )
include( ${CGAL_USE_FILE} )

# BOOST
find_package( Boost COMPONENTS system thread REQUIRED)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
include_directories(${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file( GLOB ALL_UTILITIES_HPP ${CMAKE_SOURCE_DIR}/utilities/*.hpp )

file( GLOB MOCS_QLABELLING_HPP ${CMAKE_SOURCE_DIR}/qt/*.hpp )
file( GLOB MOCS_QLABELLING_CPP ${CMAKE_SOURCE_DIR}/qt/*.cpp )
file( GLOB MOCS_QLABELLING_UI  ${CMAKE_SOURCE_DIR}/qt/*.ui  )

file( GLOB ALL_QLABELLING_HPP ${CMAKE_SOURCE_DIR}/*.hpp )
file( GLOB ALL_QLABELLING_CPP ${CMAKE_SOURCE_DIR}/*.cpp )

file( GLOB ALL_QLABELLING_CGAL_HPP ${CMAKE_SOURCE_DIR}/CGAL/Qt/*.hpp )
file( GLOB ALL_QLABELLING_CGAL_CPP ${CMAKE_SOURCE_DIR}/CGAL/Qt/*.cpp )

qt4_wrap_cpp( ALL_MOCS_QLABELLING ${MOCS_QLABELLING_HPP} )
qt4_wrap_ui(  ALL_UI_QLABELLING   ${MOCS_QLABELLING_UI}  )
qt4_add_resources( RCS_QLABELLING ${CMAKE_SOURCE_DIR}/rc/RessourcesQLabelling.qrc )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/qt )

add_executable( QLabelling ${ALL_UTILITIES_HPP}
                           ${MOCS_QLABELLING_HPP}
                           ${MOCS_QLABELLING_CPP}
                           ${MOCS_QLABELLING_UI}
                           ${ALL_QLABELLING_HPP}
                           ${ALL_QLABELLING_CPP}
                           ${ALL_MOCS_QLABELLING}
                           ${ALL_UI_QLABELLING}
                           ${RCS_QLABELLING}
                           ${ALL_QLABELLING_CGAL_HPP}
                           ${ALL_QLABELLING_CGAL_CPP} )
target_link_libraries( QLabelling ${QT_LIBRARIES} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} )
set_target_properties( QLabelling PROPERTIES DEBUG_POSTFIX "_debug" )
add_dependencies( QLabelling ${CMAKE_SOURCE_DIR}/config.hpp.cmake.in )
add_dependencies( QLabelling ${CMAKE_SOURCE_DIR}/config.hpp )

# Test executable --> replaced with the code from <CGAL>/demo/Arrangement_on_surface_2
# add_subdirectory( test/QCGALArrangement )
add_subdirectory( demo/Arrangement_on_surface_2 )
