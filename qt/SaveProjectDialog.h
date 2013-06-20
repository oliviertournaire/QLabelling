#ifndef SAVEPROJECTDIALOG_HPP
#define SAVEPROJECTDIALOG_HPP

#include <QDialog>

namespace Ui {
class SaveProjectDialog;
}

class SaveProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SaveProjectDialog(const QString& inputImagePath, QWidget *parent = 0);
    ~SaveProjectDialog();

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
    Ui::SaveProjectDialog *ui;
};

#endif // SAVEPROJECTDIALOG_HPP
