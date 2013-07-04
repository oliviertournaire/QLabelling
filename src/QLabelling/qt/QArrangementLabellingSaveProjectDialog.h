#ifndef __QARRANGMENT_LABELLING_SAVE_PROJECT_DIALOG_H__
#define __QARRANGMENT_LABELLING_SAVE_PROJECT_DIALOG_H__

#include <QDialog>

namespace Ui {
class QArrangementLabellingSaveProjectDialog;
}

class QArrangementLabellingSaveProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QArrangementLabellingSaveProjectDialog(const QString& inputImagePath, QWidget *parent = 0);
    ~QArrangementLabellingSaveProjectDialog();

    inline QString const inputImagePath() { return _inputImagePath; }
    QString labelsImagePath() const;
    QString arrangementPath() const;
    QString labelsDefinitionPath() const;
    QString projectPath() const;
    inline QString const projectName() { return _projectName; }

protected:
    QString _inputImagePath;
    QString _projectName;
    
private:
    Ui::QArrangementLabellingSaveProjectDialog *ui;
};

#endif // __QARRANGMENT_LABELLING_SAVE_PROJECT_DIALOG_H__
