#include "book.h"
#include <QSqlQuery>
#include "db_utl.h"
#include <QMessageBox>
book::book(QString No, QString Type, QString Name, QString pub, int year, QString auth, float price,int num)
{
    BookNo=No;
    BookType=Type;
    BookName=Name;
    Publisher=pub;
    Author=auth;
    Pubyear=year;
    BookPrice=price;
    Booknum=num;
}

void book::addBook(){
    if (BookNo.isEmpty()||BookName.isEmpty()||BookType.isEmpty()||Publisher.isEmpty()||Author.isEmpty()||!Pubyear||!Booknum){
        QMessageBox::information(NULL, "Add card", "Need Infromation");
        return;
    }
    QSqlQuery query;
    query.prepare("select BookTotal from books where BookName=:name and BookType=:Type and Publisher=:pub and "
                  "Pubyear=:year and BookNo=:bookno and BookPrice=:price and Author=:auth");
    query.bindValue(":name",BookName);
    query.bindValue(":Type",BookType);
    query.bindValue(":pub",Publisher);
    query.bindValue(":year",Pubyear);
    query.bindValue(":bookno",BookNo);
    query.bindValue(":price",BookPrice);
    query.bindValue(":auth",Author);
    query.exec();
    query.first();
    if (query.size()&&query.value(0).toInt()>0){
        query.prepare("update books set BookTotal=BookTotal+:num where BookName=:name and BookType=:Type and Publisher=:pub and "
                      "Pubyear=:year and BookNo=:bookno and BookPrice=:price and Author=:auth");
        query.bindValue(":num",Booknum);
        query.bindValue(":name",BookName);
        query.bindValue(":Type",BookType);
        query.bindValue(":pub",Publisher);
        query.bindValue(":year",Pubyear);
        query.bindValue(":bookno",BookNo);
        query.bindValue(":price",BookPrice);
        query.bindValue(":auth",Author);
        query.exec();
    }else {
        query.prepare("insert books values(:bookno, :Type, :name, :pub, "
                      ":year,:auth, :price,:num1, :num2, current_timestamp);");
        query.bindValue(":num1",Booknum);
        query.bindValue(":num2",Booknum);
        query.bindValue(":name",BookName);
        query.bindValue(":Type",BookType);
        query.bindValue(":pub",Publisher);
        query.bindValue(":year",Pubyear);
        query.bindValue(":bookno",BookNo);
        query.bindValue(":price",BookPrice);
        query.bindValue(":auth",Author);
        query.exec();
        if (query.lastError().isValid()){
            QMessageBox::information(NULL, "Add book", "Error");
        }
    }
}

void book::deleteBook()
{

}
