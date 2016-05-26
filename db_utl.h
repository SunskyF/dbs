#ifndef DB_UTL_H
#define DB_UTL_H
#include <QtSql>
#include <QString>

static bool connectionDB(){
    static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("Library");
    db.setPort(3306);
    if (!db.open()){
        return false;
    }
    return true;
}

#endif // DB_UTL_H
