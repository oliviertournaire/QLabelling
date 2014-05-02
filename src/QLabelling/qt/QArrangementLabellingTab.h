// Copyright (c) 2012  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: $
// $Id: $
//
// Author(s)     : Alex Tsui <alextsui05@gmail.com>

#ifndef __QARRANGEMENT_LABELLING_TAB_H__
#define __QARRANGEMENT_LABELLING_TAB_H__

#include <QWidget>
class QGridLayout;

#include "ArrangementGraphicsItem.h"
#include "QArrangementLabellingGraphicsView.h"
#include "ArrangementCurveInputCallback.h"
#include "DeleteCurveCallback.h"
#include "PointLocationCallback.h"
#include "MergeEdgeCallback.h"
#include "SplitEdgeCallback.h"
#include "FillFaceCallback.h"
#include "arrangement/ArrangementBuffer.h"

class QArrangementLabellingTabBase : public QWidget
{
    Q_OBJECT

signals:
    void modelChanged( );

public:
    QArrangementLabellingTabBase( QWidget* parent );
    virtual ~QArrangementLabellingTabBase() {}

    virtual QGraphicsScene* getScene( ) const;
    virtual QArrangementLabellingGraphicsView* getView( ) const;

    virtual CGAL::Qt::ArrangementGraphicsItemBase* getArrangementGraphicsItem( ) const;
    virtual CGAL::Qt::GraphicsViewCurveInputBase* getCurveInputCallback( ) const;
    virtual CGAL::Qt::Callback* getDeleteCurveCallback( ) const;
    virtual CGAL::Qt::Callback* getPointLocationCallback( ) const;
    virtual CGAL::Qt::Callback* getMergeEdgeCallback( ) const;
    virtual SplitEdgeCallbackBase* getSplitEdgeCallback( ) const;
    virtual FillFaceCallbackBase* getFillFaceCallback( ) const;
    void FaceLabelToolTip ();

    bool _imageHasBeenLoaded;
    bool arrHasBeenSaved();
    void setArrHasBeenSaved(bool saved);
    bool _labelsHaveBeenSaved;
    //WIP
public slots:
   void UpdateFaceLabel();
   //WIP
protected:
    virtual void setupUi( );

    QArrangementLabellingGraphicsView* _graphicsView;
    QGraphicsScene* _scene;
    QGridLayout* _layout;

    CGAL::Qt::ArrangementGraphicsItemBase* _arrangementGraphicsItem;
    CGAL::Qt::GraphicsViewCurveInputBase* _curveInputCallback;
    CGAL::Qt::Callback* _deleteCurveCallback;
    CGAL::Qt::Callback* _pointLocationCallback;
    CGAL::Qt::Callback* _mergeEdgeCallback;
    SplitEdgeCallbackBase* _splitEdgeCallback;
    FillFaceCallbackBase* _fillFaceCallback;

}; // class ArrangementDemoTabBase

template < class Arr_ >
class QArrangementLabellingTab : public QArrangementLabellingTabBase
{

public:
    typedef QArrangementLabellingTabBase Superclass;
    typedef Arr_ Arrangement;

    QArrangementLabellingTab( Arrangement* arrangement, QWidget* parent = 0 ):
        Superclass( parent ),
        _arrangement( arrangement )
    {
        _imageHasBeenLoaded = false;
        _labelsHaveBeenSaved = true;
        
        // std::cout << this->scene->views( ).size( ) << std::endl;
        // set up demo components
        this->_arrangementGraphicsItem = new CGAL::Qt::ArrangementGraphicsItem<Arrangement>(this->_arrangement);
        this->_curveInputCallback = new ArrangementCurveInputCallback<Arrangement>(this->_arrangement, this);
        // TODO: set snapping distances (grid and vertex)
        this->_deleteCurveCallback = new DeleteCurveCallback<Arrangement>( this->_arrangement, this );
        this->_pointLocationCallback = new PointLocationCallback<Arrangement>( this->_arrangement, this );
        this->_mergeEdgeCallback = new MergeEdgeCallback<Arrangement>( this->_arrangement, this );
        this->_splitEdgeCallback = new SplitEdgeCallback<Arrangement>( this->_arrangement, this );
        this->_fillFaceCallback = new FillFaceCallback<Arrangement>( this->_arrangement, this );

        this->_scene->addItem( this->_arrangementGraphicsItem );
        this->_arrangementGraphicsItem->setScene( this->_scene );
        this->_curveInputCallback->setScene( this->_scene );
        this->_deleteCurveCallback->setScene( this->_scene );
        this->_pointLocationCallback->setScene( this->_scene );
        this->_mergeEdgeCallback->setScene( this->_scene );
        this->_splitEdgeCallback->setScene( this->_scene );
        this->_fillFaceCallback->setScene( this->_scene );

        // set up callbacks
        this->_scene->installEventFilter( this->_curveInputCallback );
        QObject::connect(this->_curveInputCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->_deleteCurveCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->_fillFaceCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this, SIGNAL(modelChanged()),
                         this->_arrangementGraphicsItem, SLOT(modelChanged()));
        //WIP
        QObject::connect(this, SIGNAL(modelChanged()),
                         this, SLOT(UpdateFaceLabel()));
        //WIP
        // TODO: Add a connection to update the demo window when the fill color
        //       changes

        ArrangementBuffer::instance()->push_back(_arrangement);
    }

    void setArrangement( Arrangement* newArr, bool pushInBuffer = true )
    {
        this->_scene->removeItem( this->_arrangementGraphicsItem );
        delete this->_arrangementGraphicsItem;
        delete this->_curveInputCallback;
        delete this->_deleteCurveCallback;
        delete this->_pointLocationCallback;
        delete this->_mergeEdgeCallback;
        delete this->_splitEdgeCallback;
        delete this->_fillFaceCallback;

        this->_arrangement = newArr;

        if(pushInBuffer)
            ArrangementBuffer::instance()->push_back(_arrangement);

        // std::cout << this->scene->views( ).size( ) << std::endl;
        // set up demo components
        this->_arrangementGraphicsItem = new CGAL::Qt::ArrangementGraphicsItem<Arrangement>(this->_arrangement);
        this->_curveInputCallback = new ArrangementCurveInputCallback<Arrangement>(this->_arrangement, this);
        // TODO: set snapping distances (grid and vertex)
        this->_deleteCurveCallback = new DeleteCurveCallback<Arrangement>( this->_arrangement, this );
        this->_pointLocationCallback = new PointLocationCallback<Arrangement>( this->_arrangement, this );
        this->_mergeEdgeCallback = new MergeEdgeCallback<Arrangement>( this->_arrangement, this );
        this->_splitEdgeCallback = new SplitEdgeCallback<Arrangement>( this->_arrangement, this );
        this->_fillFaceCallback = new FillFaceCallback<Arrangement>( this->_arrangement, this );

        this->_scene->addItem( this->_arrangementGraphicsItem );
        this->_arrangementGraphicsItem->setScene( this->_scene );
        this->_curveInputCallback->setScene( this->_scene );
        this->_deleteCurveCallback->setScene( this->_scene );
        this->_pointLocationCallback->setScene( this->_scene );
        this->_mergeEdgeCallback->setScene( this->_scene );
        this->_splitEdgeCallback->setScene( this->_scene );
        this->_fillFaceCallback->setScene( this->_scene );

        // set up callbacks
        this->_scene->installEventFilter( this->_curveInputCallback );
        QObject::connect(this->_curveInputCallback, SIGNAL(modelChanged()), this,
            SIGNAL(modelChanged()));
        QObject::connect(this->_deleteCurveCallback, SIGNAL(modelChanged()), this,
            SIGNAL(modelChanged()));
        QObject::connect(this->_fillFaceCallback, SIGNAL(modelChanged()), this,
            SIGNAL(modelChanged()));
        QObject::connect(this, SIGNAL(modelChanged()),
            this->_arrangementGraphicsItem, SLOT(modelChanged()));
        //WIP
        QObject::connect(this, SIGNAL(modelChanged()),
            this, SLOT(UpdateFaceLabel()));

        // TODO: Add a connection to update the demo window when the fill color
        //       changes
        emit modelChanged( );
    }
protected:
    Arrangement* _arrangement;

}; // class ArrangementDemoTab

#endif // __QARRANGEMENT_LABELLING_TAB_H__
