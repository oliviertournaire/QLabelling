#include "SaveProjectDialog.h"
#include "ui_SaveProjectDialog.h"

#include <QFileInfo>
#include <QDir>

#include "QLabellingWidget.hpp"

SaveProjectDialog::SaveProjectDialog(const QString& inputImagePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveProjectDialog),
    _inputImagePath(inputImagePath)
{
    ui->setupUi(this);

    QFileInfo qfi(_inputImagePath);
    QDir dir = qfi.absoluteDir();
    QString baseName = qfi.baseName();
    QString extension = qfi.completeSuffix();

    ui->_lineEditInputImagePath->setText(_inputImagePath);
    ui->_lineEditLabelsImagePath->setText(dir.path() + "/" + baseName + "_labels." + extension);
    ui->_lineEditArrangementPath->setText(dir.path() + "/" + baseName + "_arrangement.arr");
    ui->_lineEditArrangementPath->setText(dir.path() + "/" + baseName + "_arrangement.arr");
    ui->_lineEditLabelsDefinitionPath->setText( QLabellingWidget::instance()->labelsPath());
    ui->_lineEditProjectPath->setText(dir.path() + "/" + baseName + ".qlb");
}

SaveProjectDialog::~SaveProjectDialog()
{
    delete ui;
}
