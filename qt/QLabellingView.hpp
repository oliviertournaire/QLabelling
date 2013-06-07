#ifndef QLABELLINGVIEW_HPP
#define QLABELLINGVIEW_HPP

#include "utilities/Singleton.hpp"

#include "QZoomableGraphicsView.hpp"
#include <QGraphicsLineItem>

#include <set>

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsRectWithLabelItem;

class QLabellingView : public QZoomableGraphicsView, public Singleton<QLabellingView>
{
    friend class Singleton<QLabellingView>;
    Q_OBJECT;

private:
    QLabellingView();
    ~QLabellingView() { writeSettings(); }

public:
    typedef enum
    {
        EDIT_MODE_NONE,
        EDIT_MODE_HORIZONTAL_LINE,
        EDIT_MODE_VERTICAL_LINE,
        EDIT_MODE_LABELLING
    } EditMode;

    inline void setMode(const EditMode mode) { _editMode = mode; }
    inline void setLabellingMode(const bool labelling) { _labellingMode = labelling; }

    void fillLabelsImage(const QRect& rect, const QColor& color);

    inline void setAlpha( const int alpha )
    {
        _alpha = alpha;
        emit alphaValueChanged(alpha);
    }

    const QString& imageToLabelFilename() const { return _imageToLabelFilename; }

    QPixmap imageToLabel() const;
    void setImageToLabel(const QString& filename);

    QImage labelsImage() const;
    void setLabelsImage(const QImage &labelsImage);

    void writeSettings();
    void readSettings();

signals:
    void labelImageChanged();
    void alphaValueChanged(const int alpha);

protected:
    void mousePressEvent(QMouseEvent *event);

    QString _imageToLabelFilename;

    QPixmap _imageToLabel;
    QImage _labelsImage;
    QGraphicsScene *_scene;
    std::vector<QGraphicsRectWithLabelItem*> _rects;

    bool _labellingMode;
    EditMode _editMode;

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
