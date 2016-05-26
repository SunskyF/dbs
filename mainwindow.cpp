#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"
#include "db_utl.h"
#include "seach.h"
#include <QMessageBox>
#include <Qdebug>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Library Management System - by fh");

    QRegExp rx;
    rx.setPattern("^[0-9]*[1-9][0-9]*$");
    ui->booknumtext->setValidator( new QRegExpValidator(rx, this) );\
    rx.setPattern("^-?\\d+$");
    ui->pubyeartext->setValidator(new QRegExpValidator(rx, this));
    ui->pubyeartextlow->setValidator(new QRegExpValidator(rx, this));
    ui->pubyeartextlow->setValidator(new QRegExpValidator(rx, this));
    rx.setPattern("^(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
    ui->pricetext->setValidator(new QRegExpValidator(rx, this));
    ui->pricetexthigh->setValidator(new QRegExpValidator(rx, this));
    ui->pricetextlow->setValidator(new QRegExpValidator(rx, this));

    ui->bookin->hide();
    ui->lendbook->hide();
    ui->manage->hide();
    ui->returnbook->hide();
    ui->stackedWidget->hide();
    ui->name->setText("当前用户: 读者");
    QTimer *timer;
    timer=new QTimer(this);//用于时间显示
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(tim_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tim_slot()//时间显示
{
    QDateTime time = QDateTime::currentDateTime();

    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");

    ui->time->setText(str);

}

void MainWindow::on_exit_clicked()
{
    ui->exit->hide();
    ui->login->show();
    ui->bookin->hide();
    ui->lendbook->hide();
    ui->manage->hide();
    ui->returnbook->hide();
    ui->stackedWidget->hide();
    ui->name->setText("当前用户: 读者");
}

void MainWindow::on_login_clicked()
{
    login.show();
    if (login.exec()==QDialog::Accepted){
        ui->login->hide();
        ui->exit->show();
        ui->bookin->show();
        ui->lendbook->show();
        ui->manage->show();
        ui->returnbook->show();
        ui->stackedWidget->hide();
        ui->name->setText("当前用户: "+login.getCurrentName());
    }
}

void MainWindow::on_manage_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(0);
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("LibraryCard");
    model->setSort(5, Qt::AscendingOrder);
    model->setHeaderData(1, Qt::Horizontal, "CardNo");
    model->setHeaderData(2, Qt::Horizontal, "person_name");
    model->setHeaderData(3, Qt::Horizontal, "Department");
    model->setHeaderData(4, Qt::Horizontal, "CardType");
    model->setHeaderData(5, Qt::Horizontal, "UpdateTime");
    model->select();
    ui->Allcard->setModel(model);
    ui->Allcard->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->Allcard->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Allcard->resizeColumnsToContents();
    ui->Allcard->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_addcard_clicked()
{
    if (ui->addcardText->text().isEmpty()||ui->cardName->text().isEmpty()||ui->cardDepart->text().isEmpty()){
        QMessageBox::information(this, tr("Add card"), tr("Need Information"));
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO LibraryCard (CardNo, person_name, Department,CardType,UpdateTime) "
                  "VALUES (:cardnum, :name, :depa, :type, :tim)");
    query.bindValue(":cardnum", ui->addcardText->text());
    query.bindValue(":name", ui->cardName->text());
    query.bindValue(":depa", ui->cardDepart->text());
    query.bindValue(":type", ui->cardType->currentText());
    QDate qdateObj = QDate::currentDate();
    QString strDate = qdateObj.toString("yyyy-MM-dd");
    query.bindValue(":tim", strDate);
    query.exec();
    if (!query.lastError().isValid()){
        QMessageBox::information(this, tr("Add card"), tr("Success"));
        ui->addcardText->clear();
        ui->cardDepart->clear();
        ui->cardName->clear();
    }else{
        QMessageBox::information(this, tr("Add card"), tr("Failed"));
    }
}

void MainWindow::on_deletecard_clicked()
{
    QSqlQuery query;
    query.prepare("select count(*) from LibraryRecords where CardNo=:no and ReturnDate is null");//if exist
    query.bindValue(":no",ui->deletcardText->text());
    query.exec();
    query.first();
    qDebug()<<query.value(0).toInt();
    if (query.isActive()&&query.value(0).toInt()>0){
        QMessageBox::information(this, tr("Delete card"), tr("Need return book(s)"));
        return ;
    }
    query.prepare("select count(*) from LibraryCard where CardNo=:cardnum");
    query.bindValue(":cardnum",ui->deletcardText->text());

    query.exec();
    query.first();
    qDebug()<<query.value(0).toInt();
    if (query.value(0).toInt()==0){
        QMessageBox::information(this, tr("Delete card"), tr("No card"));
        return ;
    }
    query.prepare("delete from LibraryCard where CardNo=:cardnum");
    query.bindValue(":cardnum",ui->deletcardText->text());
    query.exec();
    if (!query.lastError().isValid()){
        QMessageBox::information(this, tr("Delete card"), tr("Success"));
        ui->deletcardText->clear();
    }else {
        QMessageBox::information(this, tr("Delete card"), tr("Failed"));
    }
}

void MainWindow::on_bookin_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(1);
    ui->booknotext->setFocus();
    BookRefresh();
}

void MainWindow::on_search_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(2);
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("books");
    model->setSort(9, Qt::DescendingOrder);
    model->setHeaderData(0, Qt::Horizontal, "书号");
    model->setHeaderData(1, Qt::Horizontal, "书类型");
    model->setHeaderData(2, Qt::Horizontal, "书名");
    model->setHeaderData(3, Qt::Horizontal, "出版社");
    model->setHeaderData(4, Qt::Horizontal, "出版时间");
    model->setHeaderData(5, Qt::Horizontal, "作者");
    model->setHeaderData(6, Qt::Horizontal, "书本价格");
    model->setHeaderData(7, Qt::Horizontal, "现有数量");
    model->setHeaderData(8, Qt::Horizontal, "总藏书量");
    model->setHeaderData(9, Qt::Horizontal, "入库时间");
    model->select();
    ui->BookinfoTable->setModel(model);
    ui->searchtable->setModel(model);
    ui->searchtable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchtable->resizeColumnsToContents();
    ui->searchtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_lendbook_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_returnbook_clicked()
{
    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_addBook_clicked()
{
    book books(ui->booknotext->text(),ui->booktypetext->text(),ui->booknametext->text(),
               ui->publishertext->text(),ui->pubyeartext->text().toInt(),ui->authortext->text(),
               ui->pricetext->text().toFloat(),ui->booknumtext->text().toInt());
    books.addBook();
    BookRefresh();
}

void MainWindow::on_refresh_clicked()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("LibraryCard");
    model->setSort(5, Qt::AscendingOrder);
    model->setHeaderData(0, Qt::Horizontal, "CardNo");
    model->setHeaderData(1, Qt::Horizontal, "person_name");
    model->setHeaderData(2, Qt::Horizontal, "Department");
    model->setHeaderData(3, Qt::Horizontal, "CardType");
    model->setHeaderData(4, Qt::Horizontal, "UpdateTime");
    model->select();
    ui->Allcard->setModel(model);
    ui->Allcard->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->Allcard->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Allcard->resizeColumnsToContents();
    ui->Allcard->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void MainWindow::BookRefresh(){
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("books");
    model->setSort(9, Qt::DescendingOrder);
    model->setHeaderData(0, Qt::Horizontal, "书号");
    model->setHeaderData(1, Qt::Horizontal, "书类型");
    model->setHeaderData(2, Qt::Horizontal, "书名");
    model->setHeaderData(3, Qt::Horizontal, "出版社");
    model->setHeaderData(4, Qt::Horizontal, "出版时间");
    model->setHeaderData(5, Qt::Horizontal, "作者");
    model->setHeaderData(6, Qt::Horizontal, "书本价格");
    model->setHeaderData(7, Qt::Horizontal, "现有数量");
    model->setHeaderData(8, Qt::Horizontal, "总藏书量");
    model->setHeaderData(9, Qt::Horizontal, "入库时间");
    model->select();
    ui->BookinfoTable->setModel(model);
    ui->BookinfoTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BookinfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BookinfoTable->resizeColumnsToContents();
    ui->BookinfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_searchbutton_clicked()
{
    QString search=searchSQL(ui->searchnametext->text(),ui->booktypetext_2->text(),ui->searchauthortext->text(),
                             ui->searchpublishertext->text(),ui->pricetextlow->text(),ui->pricetexthigh->text(),
                             ui->pubyeartextlow->text(),ui->pubyeartexthigh->text());
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("books");
    model->setSort(9, Qt::DescendingOrder);
    model->setHeaderData(0, Qt::Horizontal, "书号");
    model->setHeaderData(1, Qt::Horizontal, "书类型");
    model->setHeaderData(2, Qt::Horizontal, "书名");
    model->setHeaderData(3, Qt::Horizontal, "出版社");
    model->setHeaderData(4, Qt::Horizontal, "出版时间");
    model->setHeaderData(5, Qt::Horizontal, "作者");
    model->setHeaderData(6, Qt::Horizontal, "书本价格");
    model->setHeaderData(7, Qt::Horizontal, "现有数量");
    model->setHeaderData(8, Qt::Horizontal, "总藏书量");
    model->setHeaderData(9, Qt::Horizontal, "入库时间");
    model->setFilter(search);
    model->select();
    ui->searchtable->setModel(model);
    ui->searchtable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchtable->resizeColumnsToContents();
    ui->searchtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_clearbutton_clicked()
{
    ui->pricetexthigh->clear();
    ui->pricetextlow->clear();
    ui->pubyeartexthigh->clear();
    ui->pubyeartextlow->clear();
    ui->searchauthortext->clear();
    ui->searchnametext->clear();
    ui->booktypetext_2->clear();
    ui->searchpublishertext->clear();
}

void MainWindow::on_lendbookbutton_clicked()
{
    QSqlQuery query;
    query.prepare("select BookStorage from books where BookNo=:no");//if exist
    query.bindValue(":no",ui->lendbookno->text());
    query.exec();
    query.first();
    if (query.isActive()&&query.value(0).toInt()>0){
        query.prepare("update books set BookStorage=Bookstorage-1 where BookNo=:no");//if exist
        query.bindValue(":no",ui->lendbookno->text());
        query.exec();

        query.prepare("select max(FID) from LibraryRecords");//if exist
        query.exec();
        query.first();
        int temp;
        if (query.isActive()){
            temp=query.value(0).toInt()+1;
        }else {
            temp=0;
        }
        query.prepare("insert LibraryRecords values(:FID, :cardno,:bookno,current_timestamp,null,:operator);");
        query.bindValue(":FID",temp);
        query.bindValue(":cardno",ui->lendcardno->text());
        query.bindValue(":bookno",ui->lendbookno->text());
        query.bindValue(":operator",login.getCurrentName());
        query.exec();
        QMessageBox::information(this, tr("Lend Book"), tr("借书成功"));
    }else{
        QMessageBox::information(this, tr("Lend Book"), tr("库存不足"));
    }
}

void MainWindow::on_lendcheckbutton_clicked()
{
//    QString search;
//    QSqlQuery query;
//    query.prepare("select BookNo from LibraryRecords where CardNo=:id and ReturnDate is null");
//    query.bindValue(":id",ui->lendcardno->text());
//    query.exec();
//    while (query.next()){
//        if (search.isEmpty())
//            search+="books.BookNo="+query.value(0).toString();
//        else{
//            search+=" or books.BookNo="+query.value(0).toString();
//        }
//    }
//    if (!search.isEmpty()){
    ui->lendbooktable->clearSelection();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT LibraryRecords.BookNo,BookName, LentDate, ReturnDate, Operator FROM LibraryRecords,books "
                    "where LibraryRecords.BookNo=books.BookNo and books.BookNo in (select BookNo from LibraryRecords where CardNo='"+ui->lendcardno->text()+"' and ReturnDate is null)");
    qDebug()<<model->lastError();
    model->setHeaderData(1, Qt::Horizontal, tr("书名"));
    model->setHeaderData(0, Qt::Horizontal, tr("书号"));
    model->setHeaderData(2, Qt::Horizontal, tr("借书时间"));
    model->setHeaderData(3, Qt::Horizontal, tr("归还时间"));
    model->setHeaderData(4, Qt::Horizontal, tr("操作员"));
    ui->lendbooktable->setModel(model);
    ui->lendbooktable->show();
//    }
}

void MainWindow::on_returncheckbutton_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT LibraryRecords.BookNo,BookName, LentDate, ReturnDate, Operator FROM LibraryRecords,books "
                    "where LibraryRecords.BookNo=books.BookNo and books.BookNo in (select BookNo from LibraryRecords where CardNo="+ui->returncardno->text()+" and ReturnDate is null)");
    qDebug()<<model->lastError();
    model->setHeaderData(1, Qt::Horizontal, tr("书名"));
    model->setHeaderData(0, Qt::Horizontal, tr("书号"));
    model->setHeaderData(2, Qt::Horizontal, tr("借书时间"));
    model->setHeaderData(3, Qt::Horizontal, tr("归还时间"));
    model->setHeaderData(4, Qt::Horizontal, tr("操作员"));
    ui->returnbooktable->setModel(model);
    ui->returnbooktable->show();
}

void MainWindow::on_returnbookbutton_clicked()
{
    QSqlQuery query;
    if (!ui->returnbookno->text().isEmpty()){
        query.prepare("update LibraryRecords set ReturnDate=current_timestamp where Cardno=:cardno and BookNo=:bookno and ReturnDate is null limit 1;");
        query.bindValue(":cardno",ui->returncardno->text());
        query.bindValue(":bookno",ui->returnbookno->text());
        query.exec();
        if (!query.lastError().isValid()){
            query.prepare("update books set BookStorage=Bookstorage+1 where BookNo=:no");
            query.bindValue(":no",ui->returnbookno->text());
            query.exec();
            if (!query.lastError().isValid()) QMessageBox::information(this, tr("Lend Book"), tr("还书成功"));
            else QMessageBox::information(this, tr("Lend Book"), tr("还书失败"));
        }else{
            QMessageBox::information(this, tr("Lend Book"), tr("还书失败"));
        }
    }else{
        QMessageBox::information(this, tr("Lend Book"), tr("请输入书号"));
    }
}
