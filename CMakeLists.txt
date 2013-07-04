cmake_minimum_required(VERSION 2.8)

project( QArrangementLabelling )

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
configure_file( ${CMAKE_SOURCE_DIR}/config/config.hpp.cmake.in ${CMAKE_SOURCE_DIR}/config/config.hpp )

# Qt
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# CGAL
find_package( CGAL COMPONENTS Core Qt4 REQUIRED )
include( ${CGAL_USE_FILE} )
# We need to check that we are using a CGAL version above 4.2
message( STATUS "*******************************************" )
message( STATUS "********** Checking CGAL version **********" )
IS_VERSION_GREATER( ${CGAL_VERSION} "4.2.0.0" VERSION_GREATER )
IS_VERSION_EQUAL( ${CGAL_VERSION} "4.2.0.0" VERSION_EQUAL )
if( ${VERSION_GREATER} OR ${VERSION_EQUAL} )
        message( STATUS "*  --> Your CGAL version (" ${CGAL_VERSION} ") is OK *" )
else()
        message( FATAL_ERROR "!!! You need a CGAL version above 2.4!!! Your CGAL version is " ${CGAL_VERSION} ". Please update your CGAL version ..." )
endif()
message( STATUS "******** Checking CGAL version done *******" )
message( STATUS "*******************************************" )

# BOOST
find_package( Boost COMPONENTS system thread REQUIRED)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
include_directories(${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# Qt
file( GLOB MOCS_QARRANGEMENTLABELLING_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.h )
file( GLOB MOCS_QARRANGEMENTLABELLING_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.cpp )
file( GLOB MOCS_QARRANGEMENTLABELLING_UI  ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.ui  )

qt4_wrap_cpp( ALL_MOCS_QARRANGEMENTLABELLING ${MOCS_QARRANGEMENTLABELLING_HPP} )
qt4_wrap_ui(  ALL_UI_QARRANGEMENTLABELLING   ${MOCS_QARRANGEMENTLABELLING_UI}  )
qt4_add_resources( RCS_QARRANGEMENTLABELLING ${CMAKE_CURRENT_SOURCE_DIR}/rc/QArrangementLabellingWindow.qrc )

# Log widget
set( LOGWIDGET_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingLogWidget.h )
set( LOGWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingLogWidget.cpp )
set( LOGWIDGET_UI  ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingLogWidget.ui )
qt4_wrap_cpp( MOCS_LOGWIDGET ${LOGWIDGET_HPP} )
if( NOT WIN32)
	qt4_wrap_ui( UI_LOGWIDGET ${LOGWIDGET_UI}  )
endif()

# Labelling widget
set( LABELLINGWIDGET_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingWidget.h )
set( LABELLINGWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingWidget.cpp )
set( LABELLINGWIDGET_UI  ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingWidget.ui )
qt4_wrap_cpp( MOCS_LABELLINGWIDGET ${LABELLINGWIDGET_HPP} )
if( NOT WIN32)
	qt4_wrap_ui( UI_LABELLINGWIDGET ${LABELLINGWIDGET_UI}  )
endif()

# Label item: remove this when #21 fixed
set( LABELITEM_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QLabelItem.hpp )
set( LABELITEM_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/QLabelItem.cpp )
qt4_wrap_cpp( MOCS_LABELITEM ${LABELITEM_HPP} )

# "Classic" C++
file( GLOB ALL_QARRANGEMENTLABELLING_HPP ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
file( GLOB ALL_QARRANGEMENTLABELLING_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

file( GLOB ALL_QARRANGEMENTLABELLING_CALLBACKS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/callbacks/*.h )
file( GLOB ALL_QARRANGEMENTLABELLING_CALLBACKS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/callbacks/*.cpp )

file( GLOB ALL_QARRANGEMENTLABELLING_GRAPHICS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.h )
file( GLOB ALL_QARRANGEMENTLABELLING_GRAPHICS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.cpp )

file( GLOB ALL_QARRANGEMENTLABELLING_CONFIG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/config/*.h )

file( GLOB ALL_QARRANGEMENTLABELLING_UTILITIES_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/utilities/*.h* )

file( GLOB ALL_QARRANGEMENTLABELLING_ARRANGEMENT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/arrangment/*.h )
file( GLOB ALL_QARRANGEMENTLABELLING_ARRANGEMENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arrangment/*.cpp )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/qt )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/callbacks )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/graphics )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/config )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/utilities )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/arrangement )

if(WIN32)
	set( APPLICATION_TYPE WIN32 )
endif()


SET( FILES_TO_TRANSLATE ${ALL_UI_QARRANGEMENTLABELLING}
                        ${RCS_QARRANGEMENTLABELLING}
                        ${LOGWIDGET_HPP}
                        ${LOGWIDGET_CPP}
                        ${LOGWIDGET_UI}
                        ${LABELLINGWIDGET_HPP}
                        ${LABELLINGWIDGET_CPP}
                        ${LABELLINGWIDGET_UI}
                        ${LABELITEM_HPP}
                        ${LABELITEM_CPP}
                        ${MOCS_LOGWIDGET}
                        ${MOCS_LABELLINGWIDGET}
                        ${MOCS_LABELITEM}
                        ${UI_LOGWIDGET}
                        ${UI_LABELLINGWIDGET}
                        ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingInfoWidget.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingInfoWidget.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/qt/QArrangementLabellingInfoWidget.ui
)

SET(LANGUAGES fr_FR)

SET(LANGUAGE_TS_FILES)
FOREACH(LANGUAGE ${LANGUAGES})
    SET(TS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake_${LANGUAGE}.ts")
    SET(LANGUAGE_TS_FILES ${LANGUAGE_TS_FILES} ${TS_FILE})
	SET_SOURCE_FILES_PROPERTIES(${TS_FILE} PROPERTIES OUTPUT_LOCATION ${EXECUTABLE_OUTPUT_PATH})
ENDFOREACH(LANGUAGE ${LANGUAGES})

IF(LANGUAGE_TS_FILES)
    QT4_CREATE_TRANSLATION(qm_files ${FILES_TO_TRANSLATE} ${LANGUAGE_TS_FILES})
ENDIF(LANGUAGE_TS_FILES)


add_executable( QArrangementLabelling ${APPLICATION_TYPE}
                                      ${MOCS_QARRANGEMENTLABELLING_HPP}
                                      ${MOCS_QARRANGEMENTLABELLING_CPP}
                                      ${MOCS_QARRANGEMENTLABELLING_UI}
                                      ${ALL_MOCS_QARRANGEMENTLABELLING}
                                      ${ALL_UI_QARRANGEMENTLABELLING}
                                      ${RCS_QARRANGEMENTLABELLING}
                                      ${LOGWIDGET_HPP}
                                      ${LOGWIDGET_CPP}
                                      ${LOGWIDGET_UI}
                                      ${LABELLINGWIDGET_HPP}
                                      ${LABELLINGWIDGET_CPP}
                                      ${LABELLINGWIDGET_UI}
                                      ${LABELITEM_HPP}
                                      ${LABELITEM_CPP}
                                      ${MOCS_LOGWIDGET}
                                      ${MOCS_LABELLINGWIDGET}
                                      ${MOCS_LABELITEM}
                                      ${UI_LOGWIDGET}
                                      ${UI_LABELLINGWIDGET}
      								  ${ALL_QARRANGEMENTLABELLING_HPP}
                                      ${ALL_QARRANGEMENTLABELLING_CPP}
      								  ${ALL_QARRANGEMENTLABELLING_CALLBACKS_HEADERS}
                                      ${ALL_QARRANGEMENTLABELLING_CALLBACKS_SOURCES}
      								  ${ALL_QARRANGEMENTLABELLING_GRAPHICS_HEADERS}
                                      ${ALL_QARRANGEMENTLABELLING_GRAPHICS_SOURCES}
									  ${ALL_QARRANGEMENTLABELLING_CONFIG_HEADERS}
									  ${ALL_QARRANGEMENTLABELLING_UTILITIES_HEADERS}
									  ${ALL_QARRANGEMENTLABELLING_ARRANGEMENT_HEADERS}
									  ${ALL_QARRANGEMENTLABELLING_ARRANGEMENT_SOURCES}
                                      ${qm_files})

target_link_libraries( QArrangementLabelling ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${QT_LIBRARIES} )

