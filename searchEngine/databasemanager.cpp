#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent)
: QObject(parent)
{

}

void DataBaseManager::connect(QString host, QString port, QString user, QString password, QString dataBase)
{
    qDebug()<<host;
    qDebug()<<port;
    qDebug()<<user;
    qDebug()<<password;
    qDebug()<<dataBase;
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
