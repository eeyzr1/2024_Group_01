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
    QModelIndex idx = ui->treeView->currentIndex();
    QModelIndex parentIdx = idx.isValid() ? idx : QModelIndex();
    QList<QVariant> data = { "NewPart", "true" };
    partList->appendChild(parentIdx, data);
    updateRender();
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
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Open STL Files"),
        QString(),
        tr("STL Files (*.stl)")
        );
    if (fileNames.isEmpty())
        return;

    QModelIndex parentIdx = ui->treeView->currentIndex();

    for (const QString &filePath : fileNames) {
        QString justName = QFileInfo(filePath).fileName();

        QList<QVariant> data = { justName, true };
        partList->appendChild(parentIdx, data);

        int newRow = partList->rowCount(parentIdx) - 1;
        QModelIndex newIdx = partList->index(newRow, 0, parentIdx);
        auto newPart = static_cast<ModelPart*>(newIdx.internalPointer());
        newPart->loadSTL(filePath);
        renderer->AddActor(newPart->getActor());
    }

    renderer->ResetCamera();
    renderWindow->Render();

    updateRender();
    emit statusUpdateMessage(
        tr("Loaded %1 files").arg(fileNames.size()), 3000);
}


void MainWindow::on_pushButton_2_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    OptionDialog dialog(this);
    dialog.setDialog(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.setModelPart(selectedPart);
        updateRender();
        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}


void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    OptionDialog dialog(this);
    dialog.setDialog(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.setModelPart(selectedPart);
        updateRender();
        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(QModelIndex());
    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex &index)
{
    int n = partList->rowCount(index);
    for (int row = 0; row < n; ++row) {

        QModelIndex child = partList->index(row, 0, index);
        auto part = static_cast<ModelPart*>(child.internalPointer());

        if (part->getActor()) {
            renderer->AddActor(part->getActor());
        }

        updateRenderFromTree(child);
    }
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Open Folder of STL Models"),
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    if (dir.isEmpty())
        return;

    QModelIndex parentIdx = ui->treeView->currentIndex();
    if (!parentIdx.isValid())
        parentIdx = QModelIndex();

    QDir d(dir);
    QStringList files = d.entryList({ "*.stl" }, QDir::Files);
    for (const QString &file : files) {
        QString fullPath = d.filePath(file);

        QList<QVariant> data = { file, true };
        partList->appendChild(parentIdx, data);

        int newRow = partList->rowCount(parentIdx) - 1;
        QModelIndex newIdx = partList->index(newRow, 0, parentIdx);
        auto newPart = static_cast<ModelPart*>(newIdx.internalPointer());
        newPart->loadSTL(fullPath);
        renderer->AddActor(newPart->getActor());
    }

    renderer->ResetCamera();
    renderWindow->Render();
    updateRender();

    emit statusUpdateMessage(
        tr("Loaded %1 files from \"%2\"")
            .arg(files.size())
            .arg(QFileInfo(dir).fileName()),
        3000
        );
}

