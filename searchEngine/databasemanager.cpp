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
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void DataBaseManager::wordSearch(QTableView* table, QString wordInput)
{
    QStringList rawWords = wordInput.split(" ", Qt::SkipEmptyParts);
    QStringList cleanWords;

    for (QString word : rawWords) {
        word = word.toLower();

        while (!word.isEmpty() && !word[word.length() - 1].isLetterOrNumber()) {
            word.chop(1);
        }
        while (!word.isEmpty() && !word[0].isLetterOrNumber()) {
            word.remove(0, 1);
        }

        if (!word.isEmpty()) {
            cleanWords.append(word);
        }
    }

    if (cleanWords.isEmpty()) {
        qWarning() << "Поисковый запрос пуст или содержит только знаки препинания!";
        return;
    }

    QString sqlWords = "'" + cleanWords.join("', '") + "'";
    int wordsCount = cleanWords.size();

    QString sqlQueryText = QString(
                               "SELECT f.file_path, SUM(fw.word_count) AS total_count "
                               "FROM file_words fw "
                               "JOIN files f ON fw.file_id = f.id "
                               "JOIN words w ON fw.word_id = w.id "
                               "WHERE w.word_text IN (%1) "
                               "GROUP BY f.id, f.file_path "
                               "HAVING COUNT(DISTINCT w.word_text) = %2 "
                               "ORDER BY total_count DESC;"
                               ).arg(sqlWords).arg(wordsCount);

    this->modelPrivate->setQuery(sqlQueryText, this->myDb);
    this->modelPrivate->setHeaderData(0, Qt::Horizontal, "Имя файла");
    this->modelPrivate->setHeaderData(1, Qt::Horizontal, "Общая встречаемость");

    table->setModel(this->modelPrivate);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void DataBaseManager::allWordsClean(QTableView* table)
{
    table->setModel(nullptr);
}

void DataBaseManager::wordClean(QTableView* table)
{
    table->setModel(nullptr);
}
