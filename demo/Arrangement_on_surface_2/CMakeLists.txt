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

# Labelling widget
set( LABELLINGWIDGET_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingWidget.hpp )
set( LABELLINGWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingWidget.cpp )
set( LABELLINGWIDGET_UI  ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingWidget.ui )
qt4_wrap_cpp( MOCS_LABELLINGWIDGET ${LABELLINGWIDGET_HPP} )
if( NOT WIN32)
	qt4_wrap_ui( UI_LABELLINGWIDGET ${LABELLINGWIDGET_UI}  )
endif()

# Labelling view: remove this when #21 fixed
set( LABELLINGVIEW_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingView.hpp )
set( LABELLINGVIEW_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabellingView.cpp )
qt4_wrap_cpp( MOCS_LABELLINGVIEW ${LABELLINGVIEW_HPP} )

# Label item: remove this when #21 fixed
set( LABELITEM_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabelItem.hpp )
set( LABELITEM_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QLabelItem.cpp )
qt4_wrap_cpp( MOCS_LABELITEM ${LABELITEM_HPP} )

# ZoomableGraphicsView: remove this when #21 fixed
set( ZOOMABLEGRAPHICSVIEW_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QZoomableGraphicsView.hpp )
set( ZOOMABLEGRAPHICSVIEW_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../qt/QZoomableGraphicsView.cpp )
qt4_wrap_cpp( MOCS_ZOOMABLEGRAPHICSVIEW ${ZOOMABLEGRAPHICSVIEW_HPP} )

# QGraphicsRectWithLabelItem: remove this when #21 fixed
set( QGRAPHICSRECTWITHLABELITEM_HPP ${CMAKE_CURRENT_SOURCE_DIR}/../../QGraphicsRectWithLabelItem.hpp )
set( QGRAPHICSRECTWITHLABELITEM_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../../QGraphicsRectWithLabelItem.cpp )
qt4_wrap_cpp( MOCS_QGRAPHICSRECTWITHLABELITEM ${QGRAPHICSRECTWITHLABELITEM_HPP} )

# "Classic" C++
file( GLOB ALL_QDEMOARRANGEMENT2_HPP ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
file( GLOB ALL_QDEMOARRANGEMENT2_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/qt )

if(WIN32)
	set( APPLICATION_TYPE WIN32 )
endif()

add_executable( QDemoArrangement2 ${APPLICATION_TYPE}
								  ${MOCS_QDEMOARRANGEMENT2_HPP}
								  ${MOCS_QDEMOARRANGEMENT2_CPP}
								  ${MOCS_QDEMOARRANGEMENT2_UI}
								  ${ALL_MOCS_QDEMOARRANGEMENT2}
								  ${ALL_UI_QDEMOARRANGEMENT2}
								  ${RCS_QDEMOARRANGEMENT2}
                                  ${LOGWIDGET_HPP}
                                  ${LOGWIDGET_CPP}
                                  ${LOGWIDGET_UI}
                                  ${LABELLINGWIDGET_HPP}
                                  ${LABELLINGWIDGET_CPP}
                                  ${LABELLINGWIDGET_UI}
                                  ${LABELLINGVIEW_HPP}
                                  ${LABELLINGVIEW_CPP}
                                  ${LABELITEM_HPP}
                                  ${LABELITEM_CPP}
                                  ${ZOOMABLEGRAPHICSVIEW_HPP}
                                  ${ZOOMABLEGRAPHICSVIEW_CPP}
                                  ${QGRAPHICSRECTWITHLABELITEM_HPP}
                                  ${QGRAPHICSRECTWITHLABELITEM_CPP}
                                  ${MOCS_LOGWIDGET}
                                  ${MOCS_LABELLINGWIDGET}
                                  ${MOCS_LABELLINGVIEW}
                                  ${MOCS_LABELITEM}
                                  ${MOCS_ZOOMABLEGRAPHICSVIEW}
                                  ${MOCS_QGRAPHICSRECTWITHLABELITEM}
                                  ${UI_LOGWIDGET}
                                  ${UI_LABELLINGWIDGET}
								  ${ALL_QDEMOARRANGEMENT2_HPP}
								  ${ALL_QDEMOARRANGEMENT2_CPP} )
target_link_libraries( QDemoArrangement2 ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${QT_LIBRARIES} )
