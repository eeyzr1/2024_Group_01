#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);

    // Connect statusUpdateMessage signal to statusbar's showMessage slot
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    /* Create/allocate the ModelList */
    this->partList = new ModelPartList("PartsList");

    /* Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);

    /* Manually create a model tree */
    ModelPart *rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++) {
        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        /* Create child item */
        ModelPart *childItem = new ModelPart({ name, visible });

        /* Append to tree top-level */
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item%1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({ name, visible });

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete partList;  // Don't forget to clean up the partList
}

void MainWindow::handleButton()
{
    QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    msgBox.exec();

    // Emit signal to update status bar
    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}
