#ifndef QSLABELLINGMAINWINDOW_HPP
#define QSLABELLINGMAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
    class QLabellingMainWindow;
}

class QLabellingWidget;

class QGraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsView;
class QPixmap;

class QLabellingMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QLabellingMainWindow(QWidget *parent = 0);
    
    QLabellingWidget *snapEditWidget() const;
    void setSnapEditWidget(QLabellingWidget *snapEditWidget);

    QGraphicsScene *labelsScene() const;
    void setLabelsScene(QGraphicsScene *labelsScene);

    QGraphicsView *labelsView() const;
    void setLabelsView(QGraphicsView *labelsView);
    
public slots:
    void openImageToLabel();
    void saveLabels();
    void updateLabelImage();

private:
    Ui::QLabellingMainWindow* _mainWindow;
    QLabellingWidget *_labellingWidget;
    QGraphicsScene *_labelsScene;
    QGraphicsView *_labelsView;
    QPixmap *_labelsPixmap;
    QGraphicsPixmapItem *_labelsPixmapItem;
};

#endif // QSLABELLINGMAINWINDOW_HPP
