#ifndef QLABELLINGVIEW_HPP
#define QLABELLINGVIEW_HPP

class QWheelEvent;
class QMouseEvent;

#include <QGraphicsView>
#include <QGraphicsLineItem>

#include <set>

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsRectWithLabelItem;

class QLabellingView : public QGraphicsView
{
    Q_OBJECT;

public:
    friend class QSnapEdit;

    typedef enum
    {
        EDIT_MODE_NONE,
        EDIT_MODE_HORIZONTAL_LINE,
        EDIT_MODE_VERTICAL_LINE,
        EDIT_MODE_LABELLING_WINDOW,
        EDIT_MODE_LABELLING_WALL,
        EDIT_MODE_LABELLING_BALCONY,
        EDIT_MODE_LABELLING_DOOR,
        EDIT_MODE_LABELLING_SHOP,
        EDIT_MODE_LABELLING_ROOF,
        EDIT_MODE_LABELLING_SKY,
        EDIT_MODE_LABELLING_UNKNOW // You should use this edit mode ...
    } EditMode;

    QLabellingView();
    ~QLabellingView() { writeSettings(); }

    inline void setMode(const EditMode mode) { _editMode = mode; }
    inline void setLabellingMode(const bool labelling) { _labellingMode = labelling; }

    void fillLabelsImage(const QRect& rect, const QColor& color);

    inline void setLineWidth(const qreal width)         { _lineWidth = width; }
    inline void setLineColor(const QColor& color)       { _lineColor = color; }
    inline void setLineStyle(const Qt::PenStyle& style) { _lineStyle = style; }

    inline qreal        lineWidth() { return _lineWidth; }
    inline QColor       lineColor() { return _lineColor; }
    inline Qt::PenStyle lineStyle() { return _lineStyle; }

    inline void setWindowColor(const QColor& color)  { _windowColor  = color; }
    inline void setWallColor(const QColor& color)    { _wallColor    = color; }
    inline void setBalconyColor(const QColor& color) { _balconyColor = color; }
    inline void setDoorColor(const QColor& color)    { _doorColor    = color; }
    inline void setShopColor(const QColor& color)    { _shopColor    = color; }
    inline void setRoofColor(const QColor& color)    { _roofColor    = color; }
    inline void setSkyColor(const QColor& color)     { _skyColor     = color; }
    inline void setUnknowColor(const QColor& color)  { _unknowColor  = color; }

    inline QColor windowColor()  { return _windowColor;  }
    inline QColor wallColor()    { return _wallColor;    }
    inline QColor balconyColor() { return _balconyColor; }
    inline QColor doorColor()    { return _doorColor;    }
    inline QColor shopColor()    { return _shopColor;    }
    inline QColor roofColor()    { return _roofColor;    }
    inline QColor skyColor()     { return _skyColor;     }
    inline QColor unknowColor()  { return _unknowColor;  }

    inline void setAlpha( const int alpha )
    {
        _alpha = alpha;
        _windowColor.setAlpha(alpha);
        _wallColor.setAlpha(alpha);
        _balconyColor.setAlpha(alpha);
        _doorColor.setAlpha(alpha);
        _shopColor.setAlpha(alpha);
        _roofColor.setAlpha(alpha);
        _skyColor.setAlpha(alpha);
        _unknowColor.setAlpha(alpha);
    }

    const QString& imageToLabelFilename() const { return _imageToLabelFilename; }

    QPixmap imageToLabel() const;
    void setImageToLabel(const QString& filename);

    QImage labelsImage() const;
    void setLabelsImage(const QImage &labelsImage);

signals:
    void labelImageChanged();

public slots:
    void zoomIn()  { scaleView(qreal(1.2));     }
    void zoomOut() { scaleView(1 / qreal(1.2)); }

protected:
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void mousePressEvent(QMouseEvent *event);

    QString _imageToLabelFilename;

    QPixmap _imageToLabel;
    QImage _labelsImage;
    QGraphicsScene *_scene;
    std::vector<QGraphicsRectWithLabelItem*> _rects;

    bool _labellingMode;
    EditMode _editMode;

    void writeSettings();
    void readSettings();

    qreal _lineWidth;
    QColor _lineColor;
    Qt::PenStyle _lineStyle;

    int _alpha;
    QColor _windowColor;
    QColor _wallColor;
    QColor _balconyColor;
    QColor _doorColor;
    QColor _shopColor;
    QColor _roofColor;
    QColor _skyColor;
    QColor _unknowColor;

private:
    int randInt(int low, int high);

    void removeAllItemsFromScene();
    void removeImagesFromScene();
    void removeAllRectanglesFromScene();
    void removeAllRectanglesWithLabelFromScene();

    void buildImageBoundaryRect();
    void rebuildRectanglesFromLastLine(const QLineF& line);
};

#endif // QLABELLINGVIEW_HPP
