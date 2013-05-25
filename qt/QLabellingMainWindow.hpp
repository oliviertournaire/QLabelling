#ifndef QSLABELLINGMAINWINDOW_HPP
#define QSLABELLINGMAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
    class QLabellingMainWindow;
}

class QLabellingWidget;
class QZoomableGraphicsView;
class QLabellingLogWidget;

class QGraphicsPixmapItem;
class QGraphicsScene;
class QPixmap;
class QCloseEvent;

class QLabellingMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QLabellingMainWindow(QWidget *parent = 0);
    ~QLabellingMainWindow();
    
    QLabellingWidget *snapEditWidget() const;
    void setSnapEditWidget(QLabellingWidget *snapEditWidget);

    QGraphicsScene *labelsScene() const;
    void setLabelsScene(QGraphicsScene *labelsScene);

    QZoomableGraphicsView *labelsView() const;
    void setLabelsView(QZoomableGraphicsView *labelsView);
    
    Ui::QLabellingMainWindow *mainWindow() const;
    void setMainWindow(Ui::QLabellingMainWindow *mainWindow);

    QLabellingLogWidget* loggerWidget() const;
    void setLoggerWidget(QLabellingLogWidget *loggerWidget);

public slots:
    void openImageToLabel();
    void saveLabels();
    void showAbout();
    void updateLabelImage();
    void quit();

protected:
    void connectAll();
    void disconnectAll();

    virtual void closeEvent(QCloseEvent *event);

    Ui::QLabellingMainWindow* _mainWindow;
    QLabellingWidget *_labellingWidget;
    QGraphicsScene *_labelsScene;
    QZoomableGraphicsView *_labelsView;
    QLabellingLogWidget *_loggerWidget;
    QPixmap *_labelsPixmap;
    QGraphicsPixmapItem *_labelsPixmapItem;
    bool _labelsPixmapSaved;
};

#endif // QSLABELLINGMAINWINDOW_HPP
