/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class for the Qt + VTK application.
 * @details This file contains the logic for the GUI, including rendering STL models,
 *          interacting with the model hierarchy, and responding to user actions.
 * @version 1.0.0
 * @author Zhixing
 * @date 2025-05-12
 */
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
#include <vtkLight.h>

/**
 * @brief Constructs the main window and initializes the UI and VTK renderer.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonAdd, &QPushButton::released, this, &MainWindow::handleButtonAdd);
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
    cylinderActor->RotateY(-45.0);


    renderer->AddActor(cylinderActor);


    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    updateLight();
}
/**
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief Handles the Add button click to insert a new part into the tree view.
 */
void MainWindow::handleButtonAdd()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }
    QModelIndex parentIndex = index.isValid() ? index : QModelIndex();
    QList<QVariant> data = { "NewPart", "true" };
    partList->appendChild(parentIndex, data);

    renderer->ResetCamera();
    renderWindow->Render();
    updateRender();
    updateLight();

    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}
/**
 * @brief Handles item selection in the tree view and shows the selected item's name in the status bar.
 */
void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage("The selected item is: " + text, 0);
}
/**
 * @brief Opens multiple STL files and loads them into the scene under the selected tree item.
 */
void MainWindow::on_actionOpen_File_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Open STL Files"),
        "C:\\",
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
    updateLight();

    emit statusUpdateMessage(
        tr("Loaded %1 files").arg(fileNames.size()), 3000);
}

/**
 * @brief Opens the item options dialog for the currently selected tree item.
 */
void MainWindow::on_pushButtonOptions_clicked()
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
        renderer->ResetCamera();
        renderWindow->Render();
        updateRender();
        updateLight();

        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}

/**
 * @brief Opens the item options dialog from the tree view context menu.
 */
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
        renderer->ResetCamera();
        renderWindow->Render();
        updateRender();
        updateLight();

        emit statusUpdateMessage("Dialog accepted", 0);
    } else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}
/**
 * @brief Clears the VTK scene and recursively renders all visible parts from the tree.
 */
void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(QModelIndex());
    renderer->Render();
}
/**
 * @brief Recursive helper to render model parts from the given tree index.
 * @param index The QModelIndex representing a node in the model tree.
 */
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
/**
 * @brief Opens a folder, loads all STL files within it, and adds them under the selected tree node.
 */
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
    updateLight();

    emit statusUpdateMessage(
        tr("Loaded %1 files from \"%2\"")
            .arg(files.size())
            .arg(QFileInfo(dir).fileName()),
        3000
        );
}

/**
 * @brief Deletes the currently selected item from the model tree and updates the render view.
 */
void MainWindow::on_pushButtonDelete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString partName = selectedPart->data(0).toString();

    partList->removeRows(index.row(), 1, index.parent());

    renderer->ResetCamera();
    renderWindow->Render();
    updateRender();
    updateLight();

    emit statusUpdateMessage("'" + partName + "' deleted", 0);
}
/**
 * @brief Updates lighting in the VTK scene with key, fill, and back lights.
 */
void MainWindow::updateLight()
{
    renderer->SetAmbient(0.2, 0.2, 0.2);

    vtkSmartPointer<vtkLight> keyLight = vtkSmartPointer<vtkLight>::New();
    keyLight->SetLightTypeToSceneLight();
    keyLight->SetPosition(200.0, 200.0, 400.0);
    keyLight->SetFocalPoint(0.0, 0.0, 0.0);
    keyLight->SetPositional(true);
    keyLight->SetDiffuseColor(1.0, 1.0, 1.0);
    keyLight->SetSpecularColor(1.0, 1.0, 1.0);
    keyLight->SetConeAngle(30.0);
    keyLight->SetIntensity(0.6);
    renderer->AddLight(keyLight);

    vtkSmartPointer<vtkLight> fillLight = vtkSmartPointer<vtkLight>::New();
    fillLight->SetLightTypeToSceneLight();
    fillLight->SetPosition(-200.0, -100.0, 100.0);
    fillLight->SetFocalPoint(0.0, 0.0, 0.0);
    fillLight->SetPositional(true);
    fillLight->SetDiffuseColor(1.0, 1.0, 1.0);
    fillLight->SetIntensity(0.4);
    renderer->AddLight(fillLight);

    vtkSmartPointer<vtkLight> backLight = vtkSmartPointer<vtkLight>::New();
    backLight->SetLightTypeToSceneLight();
    backLight->SetPosition(0.0, -200.0, 200.0);
    backLight->SetFocalPoint(0.0, 0.0, 0.0);
    backLight->SetPositional(true);
    backLight->SetDiffuseColor(1.0, 1.0, 1.0);
    backLight->SetIntensity(0.2);
    renderer->AddLight(backLight);
}
