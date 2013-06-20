#include "QLabellingLogWidget.hpp"
#include "ui_QLabellingLogWidget.h"

#include <QTime>
#include <QFile>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>

#include "config.hpp"

QLabellingLogWidget::QLabellingLogWidget() :
    QWidget(),
    ui(new Ui::QLabellingLogWidget)
{
    ui->setupUi(this);
    int currentIndex = ui->_comboBoxLogLevel->currentIndex();
    if(currentIndex>0)
        _loglevel = currentIndex;
}

QLabellingLogWidget::~QLabellingLogWidget()
{
    delete ui;
}

void QLabellingLogWidget::on__toolButtonClear_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->_textEdit->clear();
}

void QLabellingLogWidget::on__toolButtonSave_clicked(bool checked)
{
    Q_UNUSED(checked);
    QSettings settings(QLABELLING_ORGANIZATION_STRING, QLABELLING_NAME_STRING);
    settings.beginGroup("QLabellingMainWindow");
    QString defaultLogDirectory = settings.value("defaultLogDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Open image to label"), defaultLogDirectory, tr("Text files (*.txt *.log)"));
    if(!fileName.isNull())
    {
        QFile outfile;
        outfile.setFileName(fileName);
        outfile.open(QIODevice::Append | QIODevice::Text);
        QTextStream out(&outfile);
        out << ui->_textEdit->toPlainText();

        QFileInfo info(fileName);
        settings.setValue("defaultLogDirectory", info.absolutePath());
    }
    settings.endGroup();
}

void QLabellingLogWidget::logFatalError(const QString& text)
{
    if(_loglevel<=FATAL_ERROR)
        log(QString("[FATAL ERROR]"), text, Qt::red);
}

void QLabellingLogWidget::logError(const QString& text)
{
    if(_loglevel<=ERROR)
        log(QString("[ERROR]"), text, Qt::red);
}

void QLabellingLogWidget::logWarning(const QString& text)
{
    if(_loglevel<=WARNING)
        log(QString("[WARNING]"), text, QColor(255,115,0));
}

void QLabellingLogWidget::logInfo(const QString& text)
{
    if(_loglevel<=INFO)
        log(QString("[INFO]"), text, Qt::blue);
}

void QLabellingLogWidget::logTrace(const QString& text)
{
    if(_loglevel<=TRACE)
        log(QString("[TRACE]"), text, Qt::green);
}

void QLabellingLogWidget::logDebug(const QString& text)
{
    if(_loglevel<=DEBUG)
        log(QString("[DEBUG]"), text, Qt::magenta);
}

void QLabellingLogWidget::log(const QString& prefix, const QString& text, const QColor& color)
{
    ui->_textEdit->setTextColor(color);
    QTime time = QTime::currentTime();
    QString timeString = "[" + time.toString() + "]";
    ui->_textEdit->append( timeString + " " + prefix + " " + text );
}
