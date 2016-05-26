#ifndef BOOK_H
#define BOOK_H
#include <QString>

class book
{
public:
    book(QString No, QString Type, QString Name, QString pub, int year, QString auth, float price, int num);
    void addBook();
    void deleteBook();
private:
    QString BookNo,BookType,BookName,Publisher,Author;
    int Pubyear,BookTotal,BookStorage,Booknum;
    float BookPrice;
};

#endif // BOOK_H
