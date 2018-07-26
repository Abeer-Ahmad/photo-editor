

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QImage>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDialog>
#include <QLayout>
#include <QSpinBox>
#include <QSlider>
#include "clipscene.h"
#include "select.h"

QString selectedFileName;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_HCI_QtAndroidGallery_QtAndroidGallery_fileSelected(JNIEnv */*env*/,
                                                             jobject /*obj*/,
                                                             jstring results)
{
    selectedFileName = QAndroidJniObject(results).toString();
}

#ifdef __cplusplus
}
#endif


enum Duration{
    SHORT = 0,
    LONG = 1
};

void showToast(const QString& message, Duration duration = LONG){
    QtAndroid::runOnAndroidThread([message,duration]{
        QAndroidJniObject javaStr = QAndroidJniObject::fromString(message);
        QAndroidJniObject toast = QAndroidJniObject::callStaticObjectMethod("android/widget/Toast","makeText"
                                                                            ,"(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;",
                                                                            QtAndroid::androidActivity().object(),
                                                                            javaStr.object(),
                                                                            jint(duration));
        toast.callMethod<void>("show");
    });
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new ClipScene(this,ui->graphicsView);
    zoomObj = new zoom();
    rotateObj = new rotate(scene);
    setSelectionMode(false);
    //ui->dial->setRange(-180,180);
    //ui->dial->setValue(0);
    //QObject::connect(ui->dial, SIGNAL(valueChanged(int)), rotateObj, SLOT(rotateSlot(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_triggered()
{
    selectedFileName = "#";
    QAndroidJniObject::callStaticMethod<void>("com/HCI/QtAndroidGallery/QtAndroidGallery",
                                              "openAnImage",
                                              "()V");
    while(selectedFileName == "#")
        qApp->processEvents();

    if(QFile(selectedFileName).exists())
    {
//        QImage img(selectedFileName);
//        ui->label->setPixmap(QPixmap::fromImage(img));

        scene->setImage(selectedFileName);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->sceneRect() ,Qt::KeepAspectRatio);

    }
}


void MainWindow::on_zoomOut_triggered()
{
    zoomObj->zoomOut(ui->graphicsView);
}

void MainWindow::on_zoomIn_triggered()
{
    zoomObj->zoomInSelection(ui->graphicsView,scene->getSelectionRect());
}

void MainWindow::on_save_triggered()
{
    //check if there is image
    if(!scene->getCurr())   return;

    QDir picDir = QDir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    QDir saveDir = QDir(picDir.filePath("ImageProcessor"));
    if(!saveDir.exists())
        picDir.mkdir("ImageProcessor");

    QString fileName =  QString::number(QDateTime::currentMSecsSinceEpoch()) + ".jpg";
    QImage  imageObject = scene->getCurr()->toImage();
    QString filePath = saveDir.filePath(fileName);
    imageObject.save(filePath);

    //print msg to user
    showToast(QLatin1String("Image has been saved"));
}

void MainWindow::on_select_triggered()
{
    selectClicked();
}
void MainWindow::on_rotateRight_triggered()
{
    rotateObj->rotateSlot(10);

}
void MainWindow::on_rotateLeft_triggered()
{
    rotateObj->rotateSlot(-10);

}
void MainWindow::on_crop_triggered()
{
    rotateObj->deg = 0;
    scene->applyCrop();
}

void MainWindow::on_reset_triggered()
{
    rotateObj->deg = 0;
    scene->resetImage();
}
