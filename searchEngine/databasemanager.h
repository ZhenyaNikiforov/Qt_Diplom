#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTableView>

class DataBaseManager: public QObject
{
    Q_OBJECT

public:
    explicit DataBaseManager(QObject *parent = nullptr);

    void connect(QString host, QString port, QString user, QString password, QString dataBase);

    void allWordsSearch(QTableView* table);
    void wordSearch(QTableView* table, QString wordInput);

    void allWordsClean(QTableView* table);
    void wordClean(QTableView* table);

signals:
};

#endif // DATABASEMANAGER_H
