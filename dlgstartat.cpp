
#include <QProcess>
#include <QMessageBox>
#include <QStringList>
#include <QDir>

#include "dlgstartat.h"
#include "ui_dlgstartat.h"

dlgStartAT::dlgStartAT(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgStartAT)
{
    ui->setupUi(this);
}

dlgStartAT::~dlgStartAT()
{
    delete ui;
}

void dlgStartAT::on_pushButton_clicked()
{
    QProcess *qpCommand = new QProcess(this);

    QDir::setCurrent( QString("n:\\testinfra\\build") );

    qpCommand->startDetached( "tclsh80 TIPackBuild.tcl AIF_TestBuild" );

//    qpCommand->execute( ui->lineEdit->text() );

    if( qpCommand->waitForStarted() )
    {

//        qpCommand->waitForFinished();
//        qpCommand->close();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting command line\nError: %2").arg(qpCommand->errorString()) );
    }
}
