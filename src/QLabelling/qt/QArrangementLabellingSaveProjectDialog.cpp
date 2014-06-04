#include "QArrangementLabellingSaveProjectDialog.h"
#include "ui_QArrangementLabellingSaveProjectDialog.h"

#include <QFileInfo>
#include <QDir>

#include "QArrangementLabellingWidget.h"

QArrangementLabellingSaveProjectDialog::QArrangementLabellingSaveProjectDialog(const QString& inputImagePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QArrangementLabellingSaveProjectDialog),
    _inputImagePath(inputImagePath)
{
    ui->setupUi(this);

    QFileInfo qfi(_inputImagePath);
    QDir dir = qfi.absoluteDir();
    _projectName = qfi.baseName();
    QString extension = qfi.completeSuffix();

    ui->_lineEditInputImagePath->setText(_inputImagePath);
    ui->_lineEditLabelsImagePath->setText(dir.path() + "/" + _projectName + "_labels.png" );
    ui->_lineEditArrangementPath->setText(dir.path() + "/" + _projectName + "_arrangement.arr");
    ui->_lineEditLabelsDefinitionPath->setText( QArrangementLabellingWidget::instance()->labelsPath());
    ui->_lineEditProjectPath->setText(dir.path() + "/" + _projectName + ".qlb");
}

QArrangementLabellingSaveProjectDialog::~QArrangementLabellingSaveProjectDialog()
{
    delete ui;
}

QString QArrangementLabellingSaveProjectDialog::labelsImagePath() const { return ui->_lineEditLabelsImagePath->text(); }

QString QArrangementLabellingSaveProjectDialog::arrangementPath() const { return ui->_lineEditArrangementPath->text(); }

QString QArrangementLabellingSaveProjectDialog::labelsDefinitionPath() const { return ui->_lineEditLabelsDefinitionPath->text(); }

QString QArrangementLabellingSaveProjectDialog::projectPath() const { return ui->_lineEditProjectPath->text(); }