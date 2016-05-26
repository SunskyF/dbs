#ifndef SEACH_H
#define SEACH_H
#include <QString>

QString searchSQL(QString name, QString type, QString auth, QString publisher, QString pricelow, QString pricehigh, QString yearlow, QString yearhigh ){
    QString res;
    if(!name.isEmpty()){
        res+="BookName='"+name+"'";
    }else if (!type.isEmpty()){
        if (!res.isEmpty())
            res+=" and ";
        res+="BookType='"+type+"'";
    }else if (!auth.isEmpty()){
        if (!res.isEmpty())
            res+=" and ";
        res+="Author='"+auth+"'";
    }else if (!publisher.isEmpty()){
        if (!res.isEmpty())
            res+=" and ";
        res+="Publisher='"+publisher+"'";
    }else if (!pricehigh.isEmpty()||!pricelow.isEmpty()){
        if (!res.isEmpty())
            res+=" and ";
        QString temp;
        if (!pricehigh.isEmpty()){
            temp+="BookPrice<="+pricehigh;
        }
        if (!pricelow.isEmpty()){
            if(!temp.isEmpty())
                temp+=" and ";
            temp+="BookPrice>="+pricelow;
        }
        res+=temp;
    }else if (!yearhigh.isEmpty()||!yearlow.isEmpty()){
        if (!res.isEmpty())
            res+=" and ";
        QString temp;
        if (!yearhigh.isEmpty()){
            temp+="Pubyear<="+yearhigh;
        }
        if (!yearlow.isEmpty()){
            if(!temp.isEmpty())
                temp+=" and ";
            temp+="Pubyear>="+yearlow;
        }
        res+=temp;
    }
    return res;
}
#endif // SEACH_H
