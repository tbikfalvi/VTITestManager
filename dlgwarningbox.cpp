//====================================================================================
//
//
//
//====================================================================================
//
// Filename    : dlgwarningbox.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Hosszabb process-ek alatt megjeleno dialog, homokora helyett
//====================================================================================

#include <QCloseEvent>
#include <QRect>

#include "dlgwarningbox.h"
#include "tmpreferences.h"

//====================================================================================
cDlgWarningBox::cDlgWarningBox( QWidget *p_poParent ) : QDialog( p_poParent )
//====================================================================================
{
    setupUi( this );
    m_poParent = p_poParent;
}
//====================================================================================
cDlgWarningBox::~cDlgWarningBox()
//====================================================================================
{
}
//====================================================================================
void cDlgWarningBox::showWarningBox( showPosition p_spValue )
//====================================================================================
{
    if( p_spValue )
    {
        QRect   qrSize = tgPrefs::instance().screenSize();

        if( p_spValue == POS_TOP )
        {
            setGeometry( 0, 0, qrSize.width(), height() );
        }
        else if( p_spValue == POS_BOTTOM )
        {
            setGeometry( 0, qrSize.height()-height(), qrSize.width(), height() );
        }
    }
    show();
    QApplication::processEvents();
}
//====================================================================================
void cDlgWarningBox::hideWarningBox()
//====================================================================================
{
    hide();
}
//====================================================================================
void cDlgWarningBox::closeEvent( QCloseEvent *p_poEvent )
{
    p_poEvent->ignore();
}
