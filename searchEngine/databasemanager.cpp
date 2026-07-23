#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent)
: QObject(parent)
{
    this->myDb = QSqlDatabase::addDatabase("QPSQL");
}

void DataBaseManager::connectDB(QString host, QString port, QString user, QString password, QString dataBase)
{
    if(this->myDb.isOpen())
    {
        this->myDb.close();
        emit this->yesDisconnect();
        return;
    }
    this->myDb.setHostName(host);
    this->myDb.setPort(port.toInt());
    this->myDb.setUserName(user);
    this->myDb.setPassword(password);
    this->myDb.setDatabaseName(dataBase);
    this->myDb.open();

    if(this->myDb.isOpen())
    {
        emit this->yesConnect();
        return;
    }

    if(!this->myDb.isOpen())
    {
        qDebug()<<"Oshibka otkryitiya bazyi!";
    }

}

void DataBaseManager::allWordsSearch(QTableView* table)
{
    qDebug()<<"Vse slova poisk!";
}

void DataBaseManager::wordSearch(QTableView* table, QString wordInput)
{
    qDebug()<<"Poisk slova: "<<wordInput;
}

void DataBaseManager::allWordsClean(QTableView* table)
{
    qDebug()<<"Vse slova ochistka!";
}

void DataBaseManager::wordClean(QTableView* table)
{
    qDebug()<<"Ochistka vyibrannyikh slov!";
}
