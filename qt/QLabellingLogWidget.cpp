#include "QLabellingLogWidget.hpp"
#include "ui_QLabellingLogWidget.h"

#include <QTime>
#include <QFile>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>

#include "config.hpp"

QLabellingLogWidget::QLabellingLogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLabellingLogWidget)
{
    ui->setupUi(this);
}

QLabellingLogWidget::~QLabellingLogWidget()
{
    delete ui;
}

void QLabellingLogWidget::on__toolButtonClear_clicked(bool checked)
{
    ui->_textEdit->clear();
}

void QLabellingLogWidget::on__toolButtonSave_clicked(bool checked)
{
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
    log(QString("[FATAL ERROR]"), text, Qt::red);
}

void QLabellingLogWidget::logError(const QString& text)
{
    log(QString("[ERROR]"), text, Qt::red);
}

void QLabellingLogWidget::logException(const QString& text)
{
    log(QString("[EXCEPTION]"), text, Qt::red);
}

void QLabellingLogWidget::logTrace(const QString& text)
{
    log(QString("[TRACE]"), text, Qt::green);
}

void QLabellingLogWidget::logWarning(const QString& text)
{
    log(QString("[WARNING]"), text, QColor(255,115,0));
}

void QLabellingLogWidget::logInfo(const QString& text)
{
    log(QString("[INFO]"), text, Qt::blue);
}

void QLabellingLogWidget::log(const QString& prefix, const QString& text, const QColor& color)
{
    ui->_textEdit->setTextColor(color);
    QTime time = QTime::currentTime();
    QString timeString = "[" + time.toString() + "]";
    ui->_textEdit->append( timeString + " " + prefix + " " + text );
}