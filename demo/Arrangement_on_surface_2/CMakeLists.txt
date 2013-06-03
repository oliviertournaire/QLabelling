# Qt
file( GLOB MOCS_QDEMOARRANGEMENT2_HPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.h )
file( GLOB MOCS_QDEMOARRANGEMENT2_CPP ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.cpp )
file( GLOB MOCS_QDEMOARRANGEMENT2_UI  ${CMAKE_CURRENT_SOURCE_DIR}/qt/*.ui  )

qt4_wrap_cpp( ALL_MOCS_QDEMOARRANGEMENT2 ${MOCS_QDEMOARRANGEMENT2_HPP} )
qt4_wrap_ui(  ALL_UI_QDEMOARRANGEMENT2   ${MOCS_QDEMOARRANGEMENT2_UI}  )
qt4_add_resources( RCS_QDEMOARRANGEMENT2 ${CMAKE_CURRENT_SOURCE_DIR}/rc/ArrangementDemoWindow.qrc )

# Log widget
set( LOGWIDGET_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingLogWidget.hpp )
set( LOGWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingLogWidget.cpp )
set( LOGWIDGET_UI  ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingLogWidget.ui )
qt4_wrap_cpp( MOCS_LOGWIDGET ${LOGWIDGET_HPP} )
if( NOT WIN32)
	qt4_wrap_ui( UI_LOGWIDGET ${LOGWIDGET_UI}  )
endif()

# "Classic" C++
file( GLOB ALL_QDEMOARRANGEMENT2_HPP ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
file( GLOB ALL_QDEMOARRANGEMENT2_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/qt )

add_executable( QDemoArrangement2 ${MOCS_QDEMOARRANGEMENT2_HPP}
								  ${MOCS_QDEMOARRANGEMENT2_CPP}
								  ${MOCS_QDEMOARRANGEMENT2_UI}
								  ${ALL_MOCS_QDEMOARRANGEMENT2}
								  ${ALL_UI_QDEMOARRANGEMENT2}
								  ${RCS_QDEMOARRANGEMENT2}
                                  ${LOGWIDGET_HPP}
                                  ${LOGWIDGET_CPP}
                                  ${LOGWIDGET_UI}
                                  ${MOCS_LOGWIDGET}
                                  ${UI_LOGWIDGET}
								  ${ALL_QDEMOARRANGEMENT2_HPP}
								  ${ALL_QDEMOARRANGEMENT2_CPP} )
target_link_libraries( QDemoArrangement2 ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${QT_LIBRARIES} )
