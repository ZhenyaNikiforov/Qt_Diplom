#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class DataBaseManager: public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = nullptr);
    ~DataBaseManager();

    void connectium(QString host, int port, QString user, QString password, QString name);
    void disConnectium();

private:
    QSqlDatabase diplomDataBase;

signals:
    void connectionSuccess();
    void connectionError();
    void disConnectiumDB();
};

#endif // DATABASEMANAGER_H
