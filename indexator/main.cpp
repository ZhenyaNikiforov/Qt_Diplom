#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream welcomeMessage(stdout);
    welcomeMessage<<"Privet!"<<Qt::endl;
    welcomeMessage<<"Chitaem failyi i pishem v bazu? Da / Net:"<<Qt::endl;

    QString userAnswer;
    QTextStream answerMessage(stdin);
    userAnswer = answerMessage.readLine();

    if(userAnswer != "Da")
    {
        welcomeMessage<<"Ponyatno. Prervali."<<Qt::endl;
        return 0;
    }

    QString host = "";
    QString port = "";
    QString userName = "";
    QString password = "";
    QString databaseName = "";

    welcomeMessage<<"\nhost:"<<Qt::endl;
    host = answerMessage.readLine();

    welcomeMessage<<"\nport:"<<Qt::endl;
    port = answerMessage.readLine();

    welcomeMessage<<"\nuser name"<<Qt::endl;
    userName = answerMessage.readLine();

    welcomeMessage<<"\npassword"<<Qt::endl;
    password = answerMessage.readLine();

    welcomeMessage<<"\ndata base name"<<Qt::endl;
    databaseName = answerMessage.readLine();

    DataBaseManager dataBase;

    QObject::connect(&dataBase, &DataBaseManager::connectionSuccess, [&](){//-Сигнал успеш. подключения
        welcomeMessage<<"\nUspeshno pogkluchilis`!"<<Qt::endl;

        dataBase.disConnectium();
    });

    QObject::connect(&dataBase, &DataBaseManager::connectionError, [&](){//-Сигнал ошиб. подключения
        welcomeMessage<<"\nOshibka pogklucheniya!"<<Qt::endl;
        qApp->quit();
    });

    QObject::connect(&dataBase, &DataBaseManager::disConnectiumDB, [&](){//-Сигнал отключения
        welcomeMessage<<"\nUspeshno otkluchilis`!"<<Qt::endl;
        qApp->quit();
    });

    QTimer::singleShot(0, [&]{
        dataBase.connectium(host, port.toInt(), userName, password, databaseName);
    });

    return a.exec();
}
