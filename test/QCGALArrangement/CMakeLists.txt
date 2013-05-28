include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/qt )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

message( STATUS "--> " ${CMAKE_CURRENT_SOURCE_DIR} )

file( GLOB ALL_HPP_QCGAL_ARRANGEMENT ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp )
file( GLOB ALL_CPP_QCGAL_ARRANGEMENT ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

file( GLOB MOCS_QCGAL_ARRANGEMENT_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.hpp )
file( GLOB MOCS_QCGAL_ARRANGEMENT_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.cpp )
file( GLOB MOCS_QCGAL_ARRANGEMENT_UI  ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.ui  )

qt4_wrap_cpp( ALL_MOCS_QCGAL_ARRANGEMENT ${MOCS_QCGAL_ARRANGEMENT_HPP} )
qt4_wrap_ui(  ALL_UI_QCGAL_ARRANGEMENT   ${MOCS_QCGAL_ARRANGEMENT_UI}  )
qt4_add_resources( RCS_QCGAL_ARRANGEMENT ${CMAKE_CURRENT_SOURCE_DIR}/rc/RessourcesQCGALArrangement.qrc )

add_executable( QCGALArrangement ${ALL_HPP_QCGAL_ARRANGEMENT}
                                 ${ALL_CPP_QCGAL_ARRANGEMENT}
                                 ${MOCS_QCGAL_ARRANGEMENT_HPP}
                                 ${MOCS_QCGAL_ARRANGEMENT_CPP}
                                 ${MOCS_QCGAL_ARRANGEMENT_UI}
                                 ${ALL_MOCS_QCGAL_ARRANGEMENT}
                                 ${ALL_UI_QCGAL_ARRANGEMENT}
                                 ${RCS_QCGAL_ARRANGEMENT}
								 ${ALL_QLABELLING_CGAL_HPP}
								 ${ALL_QLABELLING_CGAL_CPP} )
target_link_libraries( QCGALArrangement ${QT_LIBRARIES} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} )
set_target_properties( QCGALArrangement PROPERTIES DEBUG_POSTFIX "_debug" )
