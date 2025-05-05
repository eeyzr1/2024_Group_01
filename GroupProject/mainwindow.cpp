#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "ModelPart.h"
#include "ModelPartList.h"
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    this->partList = new ModelPartList("PartsList");

    ui->treeView->setModel(this->partList);

    ModelPart *rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart *childItem = new ModelPart({name, visible});

        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({name, visible});

            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->addAction(ui->actionItem_Options);


    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);


    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);


    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);


    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());


    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(45.0);


    renderer->AddActor(cylinderActor);


    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("The selected item is: " + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
    );

    if (!fileName.isEmpty()) {
        QString justName = fileName.section('/', -1);
        emit statusUpdateMessage("Open File selected: " + justName, 0);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}


void MainWindow::on_actionItem_Options_triggered()
{
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}

