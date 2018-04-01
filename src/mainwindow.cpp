#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
    //
    //TEST
    //
    ui->List_Vertex->addItem("Vertex1");
    ui->List_Edge->addItem("Edge1");
    ui->List_Face->addItem("Face1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::on_List_Vertex_itemActivated(QListWidgetItem *item)
{
    ui->List_Vertex->addItem("Vertex1");
}

void MainWindow::on_List_Edge_itemActivated(QListWidgetItem *item)
{
    ui->List_Edge->addItem("Edge1");
}

void MainWindow::on_List_Face_itemActivated(QListWidgetItem *item)
{
    ui->List_Face->addItem("Face1");
}
