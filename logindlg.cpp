#include "logindlg.h"
#include "ui_logindlg.h"
#include "mainwindow.h"
#include "db_utl.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>
logindlg::logindlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindlg)
{
    ui->setupUi(this);
    ui->pwd->setEchoMode(QLineEdit::Password);
    ui->acco->setFocus();
    this->setWindowTitle("Login");
    if (!connectionDB()){
        QMessageBox::critical(this, tr("Error"), tr("Database can't be connected."));
        return ;
    }
}

logindlg::~logindlg()
{
    delete ui;
}

QString logindlg::getCurrentName(){
    return Username;
}

void logindlg::on_ok_clicked()
{
    QString dba_account=ui->acco->text();
    QString dba_pwd=ui->pwd->text();

    QSqlQuery query;
    query.prepare("select count(*) from users where UserID=:id");//if exist
    query.bindValue(":id",dba_account);
    query.exec();
    query.first();
    if (query.value(0).toInt()>0){
        query.prepare("select userPassword from users where UserID=:id");//verify the password
        query.bindValue(":id",dba_account);
        query.exec();
        query.first();
        if (query.value(0).toString()==dba_pwd){
            query.prepare("select userName from users where UserID=:id");//get the user name
            query.bindValue(":id",dba_account);
            query.exec();
            query.first();
            Username=query.value(0).toString();
            accept();query.prepare("select COUNT(*) from users where UserID=:id");
            query.bindValue(":id",dba_account);
            query.exec();
            query.first();
        }else {
            QMessageBox::critical(this, tr("Error"), tr("No User Name or Password is wrong."));
        }
        ui->acco->clear();
        ui->pwd->clear();
        ui->acco->setFocus();
    }else {
        QMessageBox::critical(this, tr("Error"), tr("No User Name or Password is wrong."));
        ui->acco->clear();
        ui->pwd->clear();
    }
}

void logindlg::on_exit_clicked()
{
    this->hide();
    ui->acco->clear();
    ui->pwd->clear();
}

