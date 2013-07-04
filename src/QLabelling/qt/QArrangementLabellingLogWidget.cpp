#include "QArrangementLabellingLogWidget.h"
#include "ui_QArrangementLabellingLogWidget.h"

#include <QTime>
#include <QFile>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>

#include "config.hpp"

QArrangementLabellingLogWidget::QArrangementLabellingLogWidget() :
    QWidget(),
    ui(new Ui::QArrangementLabellingLogWidget)
{
    ui->setupUi(this);
    int currentIndex = ui->_comboBoxLogLevel->currentIndex();
    if(currentIndex>0)
        _loglevel = currentIndex;
}

QArrangementLabellingLogWidget::~QArrangementLabellingLogWidget()
{
    delete ui;
}

void QArrangementLabellingLogWidget::on__toolButtonClear_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->_textEdit->clear();
}

void QArrangementLabellingLogWidget::on__toolButtonSave_clicked(bool checked)
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

void QArrangementLabellingLogWidget::logFatalError(const QString& text)
{
    if(_loglevel<=FATAL_ERROR)
        log(tr("[FATAL ERROR]"), text, Qt::red);
}

void QArrangementLabellingLogWidget::logError(const QString& text)
{
    if(_loglevel<=ERROR)
        log(tr("[ERROR]"), text, Qt::red);
}

void QArrangementLabellingLogWidget::logWarning(const QString& text)
{
    if(_loglevel<=WARNING)
        log(tr("[WARNING]"), text, QColor(255,115,0));
}

void QArrangementLabellingLogWidget::logInfo(const QString& text)
{
    if(_loglevel<=INFO)
        log(tr("[INFO]"), text, Qt::blue);
}

void QArrangementLabellingLogWidget::logTrace(const QString& text)
{
    if(_loglevel<=TRACE)
        log(tr("[TRACE]"), text, Qt::green);
}

void QArrangementLabellingLogWidget::logDebug(const QString& text)
{
    if(_loglevel<=DEBUG)
        log(tr("[DEBUG]"), text, Qt::magenta);
}

void QArrangementLabellingLogWidget::log(const QString& prefix, const QString& text, const QColor& color)
{
    ui->_textEdit->setTextColor(color);
    QTime time = QTime::currentTime();
    QString timeString = "[" + time.toString() + "]";
    ui->_textEdit->append( timeString + " " + prefix + " " + text );
}
