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

void DataBaseManager::scanDirectory(const QString &path) {
    QDir currentDir(path);

    if (!currentDir.exists()) {
        qWarning() << "Error: Directory not found:" << currentDir.absolutePath();
        return;
    }

    QDir::Filters filters = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot;
    QFileInfoList items = currentDir.entryInfoList(filters, QDir::Name);

    for (const QFileInfo &item : items) {
        if (item.isDir()) {
            qDebug() << "Found subfolder:" << item.absoluteFilePath();

            scanDirectory(item.absoluteFilePath());
        }
        else if (item.isFile() && item.suffix().toLower() == "txt") {
            this->processFile(item.absoluteFilePath(), item.fileName());
        }
    }
}

void DataBaseManager::processFile(const QString &filePath, const QString &fileName)
{
    QSqlQuery deleteQuery(this->diplomDataBase);
    deleteQuery.prepare("DELETE FROM file_words WHERE file_id = (SELECT id FROM files WHERE file_path = :path);");
    deleteQuery.bindValue(":path", fileName);
    deleteQuery.exec();

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "  Error: Could not open file:" << filePath;
        return;
    }

    QMap<QString, int> wordMap;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString word;
        in >> word;

        if (word.isEmpty()) {
            continue;
        }

        word = word.toLower();

        while (!word.isEmpty() && !word[word.length() - 1].isLetterOrNumber()) {
            word.chop(1);
        }

        while (!word.isEmpty() && !word[0].isLetterOrNumber()) {
            word.remove(0, 1);
        }

        if (!word.isEmpty()) {
            wordMap[word]++;
        }
    }

    file.close();

    QSqlQuery query(this->diplomDataBase);
    int fileId = -1;

    query.prepare("INSERT INTO files (file_path) VALUES (:path) "
                  "ON CONFLICT (file_path) DO UPDATE SET file_path = EXCLUDED.file_path "
                  "RETURNING id;");
    query.bindValue(":path", fileName);

    if (query.exec() && query.next()) {
        fileId = query.value(0).toInt();
    } else {
        qWarning() << "Failed to insert file:" << query.lastError().text();
        return;
    }

    QMap<QString, int>::const_iterator it = wordMap.constBegin();
    while (it != wordMap.constEnd()) {
        QString currentWord = it.key();
        int count = it.value();
        int wordId = -1;

        query.prepare("INSERT INTO words (word_text) VALUES (:word) "
                      "ON CONFLICT (word_text) DO NOTHING;");
        query.bindValue(":word", currentWord);
        query.exec();

        query.prepare("SELECT id FROM words WHERE word_text = :word;");
        query.bindValue(":word", currentWord);
        if (query.exec() && query.next()) {
            wordId = query.value(0).toInt();
        }

        if (fileId != -1 && wordId != -1) {
            query.prepare("INSERT INTO file_words (file_id, word_id, word_count) "
                          "VALUES (:f_id, :w_id, :cnt) "
                          "ON CONFLICT (file_id, word_id) DO UPDATE SET word_count = EXCLUDED.word_count;");
            query.bindValue(":f_id", fileId);
            query.bindValue(":w_id", wordId);
            query.bindValue(":cnt", count);

            if (!query.exec()) {
                qWarning() << "Failed to insert connection:" << query.lastError().text();
            }
        }

        ++it;
    }
    qDebug() << "  File" << fileName << "successfully indexed in DB!";
}

