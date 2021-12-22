#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "headers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_importBTN_clicked();
    void on_tokensBTN_clicked();
    void on_checkBTN_clicked();
    void on_drawBTN_2_clicked();
    void on_Zoom_valueChanged(int value);
    void on_tabWidget_currentChanged(int index);
    void on_saveBTN_clicked();
    void on_AutoSave_stateChanged();

    void on_importCodeBTN_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
