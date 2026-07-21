#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent): QObject(parent)
{
    this->diplomDataBase = QSqlDatabase::addDatabase("QPSQL");
}

DataBaseManager::~DataBaseManager()
{
    this->disConnectium();
}

void DataBaseManager::connectium(QString host, int port, QString user, QString password, QString name)
{
    this->diplomDataBase.setHostName(host);
    this->diplomDataBase.setPort(port);
    this->diplomDataBase.setUserName(user);
    this->diplomDataBase.setPassword(password);
    this->diplomDataBase.setDatabaseName(name);

    if(!this->diplomDataBase.open())
    {
        emit connectionError();
        return;
    }

    emit connectionSuccess();
    return;
}

void DataBaseManager::disConnectium()
{
    if(this->diplomDataBase.isOpen())
    {
        this->diplomDataBase.close();
    }

    QString connectionName = this->diplomDataBase.connectionName();
    this->diplomDataBase = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
    emit disConnectiumDB();
}
