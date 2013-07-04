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

#include "QArrangementLabellingGraphicsView.h"
#include "ArrangementGraphicsItem.h"
#include "ArrangementCurveInputCallback.h"
#include "DeleteCurveCallback.h"
#include "PointLocationCallback.h"
#include "MergeEdgeCallback.h"
#include "SplitEdgeCallback.h"
#include "FillFaceCallback.h"

class QGridLayout;

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

    virtual CGAL::Qt::ArrangementGraphicsItemBase* getArrangementGraphicsItem( )
    const;
    virtual CGAL::Qt::GraphicsViewCurveInputBase* getCurveInputCallback( ) const;
    virtual CGAL::Qt::Callback* getDeleteCurveCallback( ) const;
    virtual CGAL::Qt::Callback* getPointLocationCallback( ) const;
    virtual CGAL::Qt::Callback* getMergeEdgeCallback( ) const;
    virtual SplitEdgeCallbackBase* getSplitEdgeCallback( ) const;
    virtual FillFaceCallbackBase* getFillFaceCallback( ) const;

    bool _imageHasBeenLoaded;
    bool arrHasBeenSaved() { return !arrangementGraphicsItem->changed; };
    void setArrHasBeenSaved(bool saved) { arrangementGraphicsItem->changed = !saved; QArrangementLabellingInfoWidget::instance()->setChanged(!saved);  };
    bool _labelsHaveBeenSaved;

protected:
    virtual void setupUi( );

    QArrangementLabellingGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QGridLayout* layout;

    CGAL::Qt::ArrangementGraphicsItemBase* arrangementGraphicsItem;
    CGAL::Qt::GraphicsViewCurveInputBase* curveInputCallback;
    CGAL::Qt::Callback* deleteCurveCallback;
    CGAL::Qt::Callback* pointLocationCallback;
    CGAL::Qt::Callback* mergeEdgeCallback;
    SplitEdgeCallbackBase* splitEdgeCallback;
    FillFaceCallbackBase* fillFaceCallback;

}; // class ArrangementDemoTabBase

template < class Arr_ >
class QArrangementLabellingTab : public QArrangementLabellingTabBase
{
public:
    typedef QArrangementLabellingTabBase Superclass;
    typedef Arr_ Arrangement;

    QArrangementLabellingTab( Arrangement* arrangement_, QWidget* parent = 0 ):
        Superclass( parent ),
        arrangement( arrangement_ )
    {
        _imageHasBeenLoaded = false;
        _labelsHaveBeenSaved = true;
        
        // std::cout << this->scene->views( ).size( ) << std::endl;
        // set up demo components
        this->arrangementGraphicsItem = new CGAL::Qt::ArrangementGraphicsItem<Arrangement>(this->arrangement);
        this->curveInputCallback = new ArrangementCurveInputCallback<Arrangement>(this->arrangement, this);
        this->deleteCurveCallback = new DeleteCurveCallback<Arrangement>( this->arrangement, this );
        this->pointLocationCallback = new PointLocationCallback<Arrangement>( this->arrangement, this );
        this->mergeEdgeCallback = new MergeEdgeCallback<Arrangement>( this->arrangement, this );
        this->splitEdgeCallback = new SplitEdgeCallback<Arrangement>( this->arrangement, this );
        this->fillFaceCallback = new FillFaceCallback<Arrangement>( this->arrangement, this );

        this->scene->addItem( this->arrangementGraphicsItem );
        this->arrangementGraphicsItem->setScene( this->scene );
        this->curveInputCallback->setScene( this->scene );
        this->deleteCurveCallback->setScene( this->scene );
        this->pointLocationCallback->setScene( this->scene );
        this->mergeEdgeCallback->setScene( this->scene );
        this->splitEdgeCallback->setScene( this->scene );
        this->fillFaceCallback->setScene( this->scene );

        // set up callbacks
        this->scene->installEventFilter( this->curveInputCallback );
        QObject::connect(this->curveInputCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->deleteCurveCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->fillFaceCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this, SIGNAL(modelChanged()),
                         this->arrangementGraphicsItem, SLOT(modelChanged()));
        // TODO: Add a connection to update the demo window when the fill color
        //       changes
    }

    void setArrangement( Arrangement* newArr )
    {
        this->scene->removeItem( this->arrangementGraphicsItem );
        delete this->arrangementGraphicsItem;
        delete this->curveInputCallback;
        delete this->deleteCurveCallback;
        delete this->pointLocationCallback;
        delete this->mergeEdgeCallback;
        delete this->splitEdgeCallback;
        delete this->fillFaceCallback;

        this->arrangement = newArr;

        this->arrangementGraphicsItem =
                new CGAL::Qt::ArrangementGraphicsItem<Arrangement>( this->arrangement );

        this->curveInputCallback =
                new ArrangementCurveInputCallback<Arrangement>(this->arrangement, this);
        this->deleteCurveCallback =
                new DeleteCurveCallback<Arrangement>( this->arrangement, this );
        this->pointLocationCallback =
                new PointLocationCallback<Arrangement>( this->arrangement, this );
        this->mergeEdgeCallback =
                new MergeEdgeCallback<Arrangement>( this->arrangement, this );
        this->splitEdgeCallback =
                new SplitEdgeCallback<Arrangement>( this->arrangement, this );
        this->fillFaceCallback =
                new FillFaceCallback<Arrangement>( this->arrangement, this );

        this->scene->addItem( this->arrangementGraphicsItem );
        this->arrangementGraphicsItem->setScene( this->scene );
        this->curveInputCallback->setScene( this->scene );
        this->deleteCurveCallback->setScene( this->scene );
        this->pointLocationCallback->setScene( this->scene );
        this->mergeEdgeCallback->setScene( this->scene );
        this->splitEdgeCallback->setScene( this->scene );
        this->fillFaceCallback->setScene( this->scene );

        this->scene->installEventFilter(this->curveInputCallback);
        QObject::connect(this->curveInputCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->deleteCurveCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this->fillFaceCallback, SIGNAL(modelChanged()), this,
                         SIGNAL(modelChanged()));
        QObject::connect(this, SIGNAL(modelChanged()),
                         this->arrangementGraphicsItem, SLOT(modelChanged()));
        // TODO: Add a connection to update the demo window when the fill color
        //       changes

        emit modelChanged( );
    }

protected:
    Arrangement* arrangement;

}; // class ArrangementDemoTab

#endif // __QARRANGEMENT_LABELLING_TAB_H__
