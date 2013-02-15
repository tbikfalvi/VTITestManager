#ifndef DLGSTARTAT_H
#define DLGSTARTAT_H

#include <QDialog>

namespace Ui {
    class dlgStartAT;
}

class dlgStartAT : public QDialog
{
    Q_OBJECT

public:
    explicit dlgStartAT(QWidget *parent = 0);
    ~dlgStartAT();

private slots:
    void on_pushButton_clicked();

private:
    Ui::dlgStartAT *ui;
};

#endif // DLGSTARTAT_H
