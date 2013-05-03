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

# Boost
set(BOOST_REQUIRED_LIBRARIES program_options thread system date_time)
if(WIN32)
	# On Windows, need to add chrono...
	set(BOOST_REQUIRED_LIBRARIES ${BOOST_REQUIRED_LIBRARIES} chrono)
else()
endif()
find_package( Boost COMPONENTS ${BOOST_REQUIRED_LIBRARIES} )
add_definitions("-DBOOST_ALL_DYN_LINK")
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
include_directories(${Boost_INCLUDE_DIRS} )

# Qt
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file( GLOB MOCS_QLABELLING_HPP ${CMAKE_SOURCE_DIR}/qt/*.hpp )
file( GLOB MOCS_QLABELLING_CPP ${CMAKE_SOURCE_DIR}/qt/*.cpp )
file( GLOB MOCS_QLABELLING_UI  ${CMAKE_SOURCE_DIR}/qt/*.ui  )

file( GLOB ALL_QLABELLING_HPP ${CMAKE_SOURCE_DIR}/*.hpp )
file( GLOB ALL_QLABELLING_CPP ${CMAKE_SOURCE_DIR}/*.cpp )

qt4_wrap_cpp( ALL_MOCS_QLABELLING ${MOCS_QLABELLING_HPP} )
qt4_wrap_ui(  ALL_UI_QLABELLING   ${MOCS_QLABELLING_UI}  )
qt4_add_resources( RCS_QLABELLING ${CMAKE_SOURCE_DIR}/rc/RessourcesQSnapEdit.qrc )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/qt )

add_executable( QLabelling ${MOCS_QLABELLING_HPP}
                           ${MOCS_QLABELLING_CPP}
                           ${MOCS_QLABELLING_UI}
                           ${ALL_QLABELLING_HPP}
                           ${ALL_QLABELLING_CPP}
                           ${ALL_MOCS_QLABELLING}
                           ${ALL_UI_QLABELLING}
                           ${RCS_QLABELLING} )
target_link_libraries( QLabelling ${QT_LIBRARIES} )
set_target_properties( QLabelling PROPERTIES DEBUG_POSTFIX "_debug" )
