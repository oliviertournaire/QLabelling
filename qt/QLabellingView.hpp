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
    typedef enum
    {
        EDIT_MODE_NONE,
        EDIT_MODE_HORIZONTAL_LINE,
        EDIT_MODE_VERTICAL_LINE,
        EDIT_MODE_LABELLING
    } EditMode;

    QLabellingView();
    ~QLabellingView() { writeSettings(); }

    inline void setMode(const EditMode mode) { _editMode = mode; }
    inline void setLabellingMode(const bool labelling) { _labellingMode = labelling; }

    void fillLabelsImage(const QRect& rect, const QColor& color);

    inline void setAlpha( const int alpha )
    {
        _alpha = alpha;
        // Task #2: begins here!!!
        /*
        _windowColor.setAlpha(alpha);
        _wallColor.setAlpha(alpha);
        _balconyColor.setAlpha(alpha);
        _doorColor.setAlpha(alpha);
        _shopColor.setAlpha(alpha);
        _roofColor.setAlpha(alpha);
        _skyColor.setAlpha(alpha);
        _unknowColor.setAlpha(alpha);
        */
        // Task #2: ends here!!!
    }

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
    EditMode _editMode;

    void writeSettings();
    void readSettings();

    int _alpha;

private:
    void removeAllItemsFromScene();
    void removeImagesFromScene();
    void removeAllRectanglesFromScene();
    void removeAllRectanglesWithLabelFromScene();

    void buildImageBoundaryRect();
    void rebuildRectanglesFromLastLine(const QLineF& line);
};

#endif // QLABELLINGVIEW_HPP
