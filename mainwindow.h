#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "logindlg.h"
#include <QAbstractButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void BookRefresh();

private slots:
    void on_exit_clicked();

    void on_login_clicked();

    void tim_slot();

    void on_manage_clicked();

    void on_addcard_clicked();

    void on_deletecard_clicked();

    void on_bookin_clicked();

    void on_search_clicked();

    void on_lendbook_clicked();

    void on_returnbook_clicked();

    void on_addBook_clicked();

    void on_refresh_clicked();

    void on_searchbutton_clicked();

    void on_clearbutton_clicked();

    void on_lendbookbutton_clicked();

    void on_lendcheckbutton_clicked();

    void on_returncheckbutton_clicked();

    void on_returnbookbutton_clicked();

private:
    Ui::MainWindow *ui;
    logindlg login;
};

#endif // MAINWINDOW_H
