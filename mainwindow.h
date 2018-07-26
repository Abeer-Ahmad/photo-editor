
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clipscene.h"
#include <QtWidgets>
#include <QtAndroidExtras>
#include "zoom.h"
#include "rotate.h"

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
    void on_open_triggered();

    void on_zoomOut_triggered();

    void on_zoomIn_triggered();

    void on_select_triggered();

    void on_save_triggered();

    void on_rotateRight_triggered();

    void on_rotateLeft_triggered();

    void  on_crop_triggered();

    void on_reset_triggered();

private:
    Ui::MainWindow *ui;
    ClipScene* scene;
    zoom* zoomObj;
    rotate* rotateObj;

};

#endif // MAINWINDOW_H
