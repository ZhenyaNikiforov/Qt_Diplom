#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlError>

class DataBaseManager: public QObject
{
    Q_OBJECT

private:
    QSqlDatabase myDb;

public:
    explicit DataBaseManager(QObject *parent = nullptr);

    void connectDB(QString host, QString port, QString user, QString password, QString dataBase);

    void allWordsSearch(QTableView* table);
    void wordSearch(QTableView* table, QString wordInput);

    void allWordsClean(QTableView* table);
    void wordClean(QTableView* table);

signals:
    void yesDisconnect();
    void yesConnect();
};

#endif // DATABASEMANAGER_H
