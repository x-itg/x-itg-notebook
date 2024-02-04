#ifndef NEWDIALOG_H
#define NEWDIALOG_H
#include <QDialog>

namespace Ui {
class newDialog;
}

class newDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit newDialog(QMainWindow *parent = 0);
    ~newDialog();

private:
    Ui::newDialog *ui;


};
#endif // NEWDIALOG_H
