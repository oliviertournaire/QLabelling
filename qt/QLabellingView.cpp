#include "QLabellingView.hpp"

#include <QWheelEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QLine>
#include <QTime>
#include <QSettings>
#include <QDebug>

#include <math.h>

#include <iostream>

#include "QGraphicsRectWithLabelItem.hpp"
#include "config.hpp"

using namespace std;

QLabellingView::QLabellingView() :
    QGraphicsView(),
    _labellingMode(false),
    _scene(new QGraphicsScene),
    _editMode(EDIT_MODE_NONE),
    _lineWidth(qreal(1.))
{
    qsrand(QTime::currentTime().msec());
    this->setScene(_scene);

    readSettings();
}

void QLabellingView::writeSettings()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    settings.beginGroup("QLabellingView");
    settings.setValue("lineWidth",    _lineWidth   );
    settings.setValue("lineColor",    _lineColor   );
    settings.setValue("lineStyle",    _lineStyle   );
    settings.setValue("alpha",        _alpha       );
    settings.setValue("windowColor",  _windowColor );
    settings.setValue("wallColor",    _wallColor   );
    settings.setValue("balconyColor", _balconyColor);
    settings.setValue("doorColor",    _doorColor   );
    settings.setValue("shopColor",    _shopColor   );
    settings.setValue("roofColor",    _roofColor   );
    settings.setValue("skyColor",     _skyColor    );
    settings.setValue("unknowColor",  _unknowColor );
    settings.endGroup();
}

void QLabellingView::readSettings()
{
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);

    settings.beginGroup("QLabellingView");
    _lineWidth    = settings.value("lineWidth", qreal(1.)).toReal();
    _lineColor    = settings.value("lineColor", Qt::red).value<QColor>();
    _lineStyle    = (Qt::PenStyle)settings.value("lineStyle", Qt::SolidLine).toInt();
    _alpha        = settings.value("alpha", 127).toInt();
    _windowColor  = settings.value("windowColor", Qt::yellow).value<QColor>();
    _wallColor    = settings.value("wallColor", Qt::gray).value<QColor>();
    _balconyColor = settings.value("balconyColor", Qt::blue).value<QColor>();
    _doorColor    = settings.value("doorColor", QColor(0,255,0)).value<QColor>();
    _shopColor    = settings.value("shopColor", Qt::magenta).value<QColor>();
    _roofColor    = settings.value("roofColor", Qt::cyan).value<QColor>();
    _skyColor     = settings.value("skyColor", Qt::darkBlue).value<QColor>();
    _unknowColor  = settings.value("unknowColor", Qt::darkGray).value<QColor>();
    setAlpha(_alpha);
    settings.endGroup();
}

void QLabellingView::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void QLabellingView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

int QLabellingView::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

void QLabellingView::mousePressEvent(QMouseEvent *event)
{
    QPointF pScene = this->mapToScene(event->pos());
    qreal x = pScene.x();
    qreal y = pScene.y();

    // Selection mode
    if ( event->modifiers() == Qt::ControlModifier && _labellingMode )
    {
        QGraphicsItem* selectedItem = _scene->itemAt(x, y, QTransform());
        if ( QGraphicsRectWithLabelItem* rectItem = qgraphicsitem_cast<QGraphicsRectWithLabelItem*>(selectedItem) )
        {
            vector<QGraphicsRectWithLabelItem*>::iterator it;
            it = std::find(_rects.begin(), _rects.end(), rectItem);

            switch (_editMode)
            {
            case EDIT_MODE_LABELLING_WINDOW:
                rectItem->setBrush( QBrush(_windowColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_WINDOW );
                break;
            case EDIT_MODE_LABELLING_WALL:
                rectItem->setBrush( QBrush(_wallColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_WALL );
                break;
            case EDIT_MODE_LABELLING_BALCONY:
                rectItem->setBrush( QBrush(_balconyColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_BALCONY );
                break;
            case EDIT_MODE_LABELLING_DOOR:
                rectItem->setBrush( QBrush(_doorColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_DOOR );
                break;
            case EDIT_MODE_LABELLING_SHOP:
                rectItem->setBrush( QBrush(_shopColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_SHOP );
                break;
            case EDIT_MODE_LABELLING_ROOF:
                rectItem->setBrush( QBrush(_roofColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_ROOF );
                break;
            case EDIT_MODE_LABELLING_SKY:
                rectItem->setBrush( QBrush(_skyColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_SKY );
                break;
            default:
                rectItem->setBrush( QBrush(_unknowColor, Qt::SolidPattern) );
                (*it)->setLabel( QGraphicsRectWithLabelItem::LABEL_UNKNOW );
                break;
            }

            fillLabelsImage(rectItem->rect().toRect(), rectItem->brush().color());
            emit labelImageChanged();
            _scene->update();
        }
    }
    // Edit mode
    else
    {
        if (_editMode != EDIT_MODE_HORIZONTAL_LINE && _editMode != EDIT_MODE_VERTICAL_LINE)
            return;

        if ( x < 0. || y < 0. || x > _imageToLabel.width()-1 || y > _imageToLabel.height()-1 )
            return;

        QPen pen;
        pen.setWidthF(_lineWidth);
        pen.setColor(_lineColor);
        //pen.setStyle(_lineStyle);

        QLineF lastLine;

        if (_editMode == EDIT_MODE_HORIZONTAL_LINE)
        {
            lastLine.setLine(0, y, _imageToLabel.width()-1, y);
        }
        else if (_editMode == EDIT_MODE_VERTICAL_LINE)
        {
            lastLine.setLine(x, 0, x, _imageToLabel.height()-1);
        }
        else
            return;

        rebuildRectanglesFromLastLine(lastLine);
    }
}

void QLabellingView::fillLabelsImage(const QRect& rect, const QColor& color)
{
    QRgb pixelsValue = color.rgb();

    int x0 = rect.x();
    int y0 = rect.y();
    int x1 = rect.right();
    int y1 = rect.bottom();

    /*
    for(unsigned int i=y0;i<y1;++i)
    {
        QRgb* pixelsLine = (QRgb*)_labelsImage.scanLine(i);
        for (unsigned int j=x0;j<x1;++j)
        {
            QRgb* currentPixel = pixelsLine + j*sizeof(QImage::Format_RGB32);
            currentPixel = &pixelsValue;
        }
    }
    */

    for(unsigned int i=y0;i<y1;++i)
        for (unsigned int j=x0;j<x1;++j)
            _labelsImage.setPixel(j,i,pixelsValue);
}

void QLabellingView::buildImageBoundaryRect()
{
    QGraphicsRectWithLabelItem* boundaries = new QGraphicsRectWithLabelItem;
    boundaries->setRect(0, 0, _imageToLabel.width(), _imageToLabel.height());
    boundaries->setLabel(QGraphicsRectWithLabelItem::LABEL_UNKNOW);
    boundaries->setBrush( QColor(0,0,0,0) );
    _scene->addItem(boundaries);
    _rects.push_back( boundaries );
}

void QLabellingView::removeAllItemsFromScene()
{
    QList<QGraphicsItem *> items = _scene->items();
    for(unsigned int i=0;i<items.size();++i)
        _scene->removeItem(items[i]);
}

void QLabellingView::removeImagesFromScene()
{
    QList<QGraphicsItem *> items = _scene->items();
    for(unsigned int i=0;i<items.size();++i)
        if ( qgraphicsitem_cast<QGraphicsPixmapItem*>(items[i]) )
            _scene->removeItem(items[i]);
}

void QLabellingView::removeAllRectanglesFromScene()
{
    QList<QGraphicsItem *> items = _scene->items();
    for(unsigned int i=0;i<items.size();++i)
        if ( qgraphicsitem_cast<QGraphicsRectItem*>(items[i]) )
            _scene->removeItem(items[i]);
}

void QLabellingView::removeAllRectanglesWithLabelFromScene()
{
    for(unsigned int i=0;i<_rects.size();++i)
        _scene->removeItem(_rects[i]);
}

void QLabellingView::rebuildRectanglesFromLastLine(const QLineF& line)
{
    QPen pen;
    pen.setWidthF(_lineWidth);
    pen.setColor(_lineColor);

    vector<QGraphicsRectWithLabelItem*> collidingRects;

    unsigned int numIntersectedRectWithLabel = 0;
    QGraphicsLineItem lineItem;
    lineItem.setLine(line);
    QList<QGraphicsItem *> collidingItems = _scene->collidingItems (&lineItem);
    for(unsigned int i=0;i<collidingItems.size();++i)
        if ( QGraphicsRectWithLabelItem* collidedRect = qgraphicsitem_cast<QGraphicsRectWithLabelItem*>(collidingItems[i]) )
        {
            ++numIntersectedRectWithLabel;
            collidingRects.push_back(collidedRect);
        }

    // Split colliding rects wrt lastLine
    // 1) Find the rect in the _rects vector
    for(unsigned int i=0;i<collidingRects.size();++i)
    {
        for(unsigned int j=0;j<_rects.size();++j)
        {
            bool found = false;
            if ( collidingRects[i]->rect() == _rects[j]->rect() )
            {
                found = true;
            }
            if (found)
            {
                QGraphicsRectWithLabelItem::Label intersectedLabel = _rects[j]->label();
                QGraphicsRectWithLabelItem* newRect1 = new QGraphicsRectWithLabelItem;
                QGraphicsRectWithLabelItem* newRect2 = new QGraphicsRectWithLabelItem;

                // Vertical Line
                if ( line.x1() == line.x2() )
                {
                    newRect1->setRect( collidingRects[i]->rect().x(),
                                       collidingRects[i]->rect().y(),
                                       line.x1()-collidingRects[i]->rect().x(),
                                       collidingRects[i]->rect().height() );

                    newRect2->setRect( line.x1(),
                                       collidingRects[i]->rect().y(),
                                       collidingRects[i]->rect().right()-line.x1(),
                                       collidingRects[i]->rect().height() );
                }
                // Horizontal line
                else
                {
                    newRect1->setRect( collidingRects[i]->rect().x(),
                                       collidingRects[i]->rect().y(),
                                       collidingRects[i]->rect().width(),
                                       line.y1()-collidingRects[i]->rect().y() );

                    newRect2->setRect( collidingRects[i]->rect().x(),
                                       line.y1(),
                                       collidingRects[i]->rect().width(),
                                       collidingRects[i]->rect().bottom()-line.y1() );
                }
                vector<QGraphicsRectWithLabelItem*>::iterator it = std::find(_rects.begin(), _rects.end(), _rects[j]);
                _scene->removeItem(collidingRects[i]);
                _rects.erase(it);
                newRect1->setLabel(intersectedLabel);
                newRect2->setLabel(intersectedLabel);
                _rects.push_back(newRect1);
                _rects.push_back(newRect2);

                QBrush brush(Qt::SolidPattern);
                if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_WINDOW)
                    brush.setColor( _windowColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_WALL)
                    brush.setColor( _wallColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_BALCONY)
                    brush.setColor( _balconyColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_DOOR)
                    brush.setColor( _doorColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_SHOP)
                    brush.setColor( _shopColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_ROOF)
                    brush.setColor( _roofColor );
                else if(intersectedLabel == QGraphicsRectWithLabelItem::LABEL_SKY)
                    brush.setColor( _skyColor );
                else
                    brush.setColor( _unknowColor );

                newRect1->setBrush(brush);
                newRect2->setBrush(brush);

                _scene->addItem(newRect1);
                _scene->addItem(newRect2);
                _scene->update();
            }
        }
    }
}

QImage QLabellingView::labelsImage() const
{
    return _labelsImage;
}

void QLabellingView::setLabelsImage(const QImage &labelsImage)
{
    _labelsImage = labelsImage;
}

QPixmap QLabellingView::imageToLabel() const
{
    return _imageToLabel;
}

void QLabellingView::setImageToLabel(const QString& filename)
{
    _imageToLabelFilename = filename;
    _imageToLabel = QPixmap(filename);
    removeAllItemsFromScene();
    _scene->addPixmap(_imageToLabel);
    _labelsImage = QImage(_imageToLabel.width(), _imageToLabel.height(), QImage::Format_RGB32);
    _labelsImage.fill( _unknowColor );
    buildImageBoundaryRect();
}
