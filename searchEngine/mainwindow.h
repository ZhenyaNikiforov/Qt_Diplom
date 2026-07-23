#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DataBaseManager dataBase;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

private slots:
    void on_connectDB_clicked();

    void on_allWordsSearch_clicked();
    void on_wordSearch_clicked();

    void on_allWordsClean_clicked();
    void on_wordClean_clicked();
};
#endif // MAINWINDOW_H
