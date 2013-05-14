#ifndef QLABELLINGVIEW_HPP
#define QLABELLINGVIEW_HPP

#include "QZoomableGraphicsView.hpp"
#include <QGraphicsLineItem>

#include <set>

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsRectWithLabelItem;

class QLabellingView : public QZoomableGraphicsView
{
    Q_OBJECT;

public:
    // Task #2: begins here!!!
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
        EDIT_MODE_LABELLING_UNKNOW
    } EditMode;
    // Task #2: ends here!!!

    QLabellingView();
    ~QLabellingView() { writeSettings(); }

    // Task #2: begins here!!!
    inline void setMode(const EditMode mode) { _editMode = mode; }
    inline void setLabellingMode(const bool labelling) { _labellingMode = labelling; }
    // Task #2: ends here!!!

    void fillLabelsImage(const QRect& rect, const QColor& color);

    // Task #2: begins here!!!
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
    // Task #2: ends here!!!

    const QString& imageToLabelFilename() const { return _imageToLabelFilename; }

    QPixmap imageToLabel() const;
    void setImageToLabel(const QString& filename);

    QImage labelsImage() const;
    void setLabelsImage(const QImage &labelsImage);

signals:
    void labelImageChanged();

protected:
    void mousePressEvent(QMouseEvent *event);

    QString _imageToLabelFilename;

    QPixmap _imageToLabel;
    QImage _labelsImage;
    QGraphicsScene *_scene;
    std::vector<QGraphicsRectWithLabelItem*> _rects;

    bool _labellingMode;
    // Task #2: begins here!!!
    EditMode _editMode;
    // Task #2: ends here!!!

    void writeSettings();
    void readSettings();

    int _alpha;
    // Task #2: begins here!!!
    QColor _windowColor;
    QColor _wallColor;
    QColor _balconyColor;
    QColor _doorColor;
    QColor _shopColor;
    QColor _roofColor;
    QColor _skyColor;
    QColor _unknowColor;
    // Task #2: ends here!!!

private:
    void removeAllItemsFromScene();
    void removeImagesFromScene();
    void removeAllRectanglesFromScene();
    void removeAllRectanglesWithLabelFromScene();

    void buildImageBoundaryRect();
    void rebuildRectanglesFromLastLine(const QLineF& line);
};

#endif // QLABELLINGVIEW_HPP
