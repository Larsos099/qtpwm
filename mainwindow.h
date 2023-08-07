#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_open_file_triggered();
    void on_close_triggered();
    void on_save_file_triggered();
    void on_newentry_clicked();
    void on_changeenty_clicked();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
