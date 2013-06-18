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

#ifndef ARRANGEMENT_DEMO_GRAPHICS_VIEW_H
#define ARRANGEMENT_DEMO_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QColor>

class ArrangementDemoGraphicsView : public QGraphicsView
{
public:
    ArrangementDemoGraphicsView( QWidget* parent = 0 );

    inline void setShowGrid(bool b)                     { _showGrid = b; }
    inline bool showGrid() const                        { return _showGrid; }
    inline void setGridSize(int size)                   { _gridSize = size; }
    inline int gridSize() const                         { return _gridSize; }
    inline void setGridColor(const QColor &color)       { _gridColor = color; }
    inline QColor gridColor() const                     { return _gridColor; }
    inline void setBackgroundColor(const QColor &color) { _backgroundColor = color; }
    inline QColor backgroundColor() const               { return _backgroundColor; }

    inline const QString imageToLabelFilename() const                     { return _imageToLabelFilename; }
    inline void          setImageToLabelFilename(const QString &filename) { _imageToLabelFilename = filename; }
    inline const QPixmap imageToLabel() const                             { return _imageToLabel; }
    inline void          setImageToLabel(const QPixmap& image)            { _imageToLabel = image; }
    inline const QSize   imageToLabelSize() const                         { return _imageToLabelSize; }
    inline const void    setImageToLabelSize(const QSize& size)           { _imageToLabelSize = size; }
    inline const int     imageToLabelWidth() const
    {
        if (_imageToLabelSize.isValid())
            return _imageToLabelSize.width();
        else
            return 0;
    }
    inline const int imageToLabelHeight() const
    {
        if (_imageToLabelSize.isValid())
            return _imageToLabelSize.height();
        else
            return 0;
    }

protected:
    void drawForeground( QPainter* painter, const QRectF& rect );
    QRectF getViewportRect( ) const;

    bool _showGrid;
    int _gridSize;
    QColor _gridColor;
    QColor _backgroundColor;

    /// Filename of the current image to label
    QString _imageToLabelFilename;
    /// Image to label
    QPixmap _imageToLabel;
    /// Image to label dimensions
    QSize _imageToLabelSize;
};

#endif // ARRANGEMENT_DEMO_GRAPHICS_VIEW_H
