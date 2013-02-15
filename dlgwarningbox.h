#ifndef DLGWARNINGBOX_H
#define DLGWARNINGBOX_H

#include "ui_dlgwarningbox.h"

class cDlgWarningBox : public QDialog, protected Ui::dlgWarningBox
{
    Q_OBJECT

public:

    enum showPosition {
        POS_DEFAULT = 0,
        POS_TOP,
        POS_BOTTOM
    };

    cDlgWarningBox( QWidget *p_poParent = 0 );
    virtual ~cDlgWarningBox();

    void    showWarningBox( showPosition p_spValue = POS_DEFAULT );
    void    hideWarningBox();

protected:

private:
    QWidget *m_poParent;

private slots:
    void closeEvent( QCloseEvent *p_poEvent );

};

#endif
