#include "databasemanager.h"

DataBaseManager::DataBaseManager(QObject *parent)
: QObject(parent)
{
    this->myDb = QSqlDatabase::addDatabase("QPSQL");
    this->modelAll = new QSqlQueryModel(this);
    this->modelPrivate = new QSqlQueryModel(this);
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
    QSqlQuery query(this->myDb);
    query.prepare(R"(
        SELECT
            w.word_text AS "Слово",
            SUM(fw.word_count) AS "Общая частота"
        FROM file_words fw
        JOIN words w ON fw.word_id = w.id
        GROUP BY w.id, w.word_text
        ORDER BY "Общая частота" DESC;
    )");
    if (!query.exec())
    {
        qDebug() << "Ошибка выполнения SQL:" << query.lastError().text();
        return;
    }
    this->modelAll->setQuery(std::move(query));
    table->setModel(modelAll);

}

void DataBaseManager::wordSearch(QTableView* table, QString wordInput)
{
    qDebug()<<"Poisk slova: "<<wordInput;
}

void DataBaseManager::allWordsClean(QTableView* table)
{
    table->setModel(nullptr);
}

void DataBaseManager::wordClean(QTableView* table)
{
    qDebug()<<"Ochistka vyibrannyikh slov!";
}
