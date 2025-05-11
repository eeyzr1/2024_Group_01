#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void statusUpdateMessage(const QString &message, int timeout);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleButtonAdd();
    void handleTreeClicked();
    void updateRender();
    void updateRenderFromTree(const QModelIndex &index);

private slots:
    void on_actionOpen_File_triggered();
    void on_pushButtonOptions_clicked();
    void on_actionItem_Options_triggered();
    void on_actionOpen_Folder_triggered();
    void on_pushButtonDelete_clicked();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};
#endif // MAINWINDOW_H
