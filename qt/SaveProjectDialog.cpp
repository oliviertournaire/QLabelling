#include "SaveProjectDialog.h"
#include "ui_SaveProjectDialog.h"

#include <QFileInfo>
#include <QDir>

#include "QArrangementLabellingWidget.h"

SaveProjectDialog::SaveProjectDialog(const QString& inputImagePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveProjectDialog),
    _inputImagePath(inputImagePath)
{
    ui->setupUi(this);

    QFileInfo qfi(_inputImagePath);
    QDir dir = qfi.absoluteDir();
    _projectName = qfi.baseName();
    QString extension = qfi.completeSuffix();

    ui->_lineEditInputImagePath->setText(_inputImagePath);
    ui->_lineEditLabelsImagePath->setText(dir.path() + "/" + _projectName + "_labels." + extension);
    ui->_lineEditArrangementPath->setText(dir.path() + "/" + _projectName + "_arrangement.arr");
    ui->_lineEditLabelsDefinitionPath->setText( QArrangementLabellingWidget::instance()->labelsPath());
    ui->_lineEditProjectPath->setText(dir.path() + "/" + _projectName + ".qlb");
}

SaveProjectDialog::~SaveProjectDialog()
{
    delete ui;
}

QString SaveProjectDialog::labelsImagePath() const { return ui->_lineEditLabelsImagePath->text(); }

QString SaveProjectDialog::arrangementPath() const { return ui->_lineEditArrangementPath->text(); }

QString SaveProjectDialog::labelsDefinitionPath() const { return ui->_lineEditLabelsDefinitionPath->text(); }

QString SaveProjectDialog::projectPath() const { return ui->_lineEditProjectPath->text(); }