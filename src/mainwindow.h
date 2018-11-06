#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QFile>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString directory;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_chooseFile_btn_clicked();

    void on_Translate_btn_clicked();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
