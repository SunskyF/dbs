#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class logindlg;
}

class logindlg : public QDialog
{
    Q_OBJECT

public:
    explicit logindlg(QWidget *parent = 0);
    ~logindlg();
    QString getCurrentName();
private slots:
    void on_ok_clicked();

    void on_exit_clicked();

private:
    Ui::logindlg *ui;
    QString Username;
};

#endif // LOGINDLG_H
