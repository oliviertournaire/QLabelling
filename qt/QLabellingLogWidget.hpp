#ifndef QLABELLINGLOGWIDGET_HPP
#define QLABELLINGLOGWIDGET_HPP

#include "utilities/Singleton.hpp"

#include <QWidget>

namespace Ui {
class QLabellingLogWidget;
}

class QLabellingLogWidget : public QWidget, public Singleton<QLabellingLogWidget>
{
    friend class Singleton<QLabellingLogWidget>;
    Q_OBJECT
    
private:
    explicit QLabellingLogWidget();
    ~QLabellingLogWidget();

public slots:
    void on__toolButtonClear_clicked(bool checked = false);
    void on__toolButtonSave_clicked(bool checked = false);

    void logFatalError(const QString& text);
    void logError(const QString& text);
    void logException(const QString& text);
    void logTrace(const QString& text);
    void logWarning(const QString& text);
    void logInfo(const QString& text);

private:
    Ui::QLabellingLogWidget *ui;

    void log(const QString& prefix, const QString& text, const QColor &color);
};

#endif // QLABELLINGLOGWIDGET_HPP
