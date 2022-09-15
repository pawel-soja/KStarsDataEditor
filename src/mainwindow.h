#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stardatabase.h"

#include <QMainWindow>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void load();

private slots:
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_defaultButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    StarDatabase starDatabase;
};
#endif // MAINWINDOW_H
