/**
 * @file mainwindow.h
 * @brief Header file for the MainWindow class.
 * @details Declares the main GUI window for the application. Handles interactions between the user interface,
 *          3D rendering, and the model data. Uses Qt for GUI and VTK for visualization.
 * @version 1.0.0
 * @author Woojin, Zhixing, Zhiyuan/Paul
 * @date 2025-05-12/2022
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>

/**
 * @class MainWindow
 * @brief Main application window integrating Qt UI with VTK rendering.
 */
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    /**
     * @brief Emitted to display a message in the status bar.
     * @param message The message string.
     * @param timeout Display duration in milliseconds.
     */
    void statusUpdateMessage(const QString &message, int timeout);

public:
    /**
     * @brief Constructs the main window and initializes UI and rendering components.
     * @param parent Pointer to the parent widget.
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destructor for the MainWindow class.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Adds a new model part to the selected item in the tree view.
     */
    void handleButtonAdd();
    /**
     * @brief Handles a click on a tree view item and displays its name.
     */
    void handleTreeClicked();
    /**
     * @brief Clears and re-renders the VTK scene based on model tree.
     */
    void updateRender();
    /**
     * @brief Recursively renders tree view model parts.
     * @param index The starting index in the model tree.
     */
    void updateRenderFromTree(const QModelIndex &index);
    /**
     * @brief Configures lighting in the VTK renderer.
     */
    void updateLight();

private slots:
    /**
     * @brief Opens one or more STL files and loads them into the scene.
     */
    void on_actionOpen_File_triggered();
    /**
     * @brief Opens the options dialog for the selected item.
     */
    void on_pushButtonOptions_clicked();
    /**
     * @brief Opens the options dialog from the context menu.
     */
    void on_actionItem_Options_triggered();
    /**
     * @brief Opens a folder and loads all STL files under the selected node.
     */
    void on_actionOpen_Folder_triggered();
    /**
     * @brief Deletes the selected model part from the tree and the scene.
     */
    void on_pushButtonDelete_clicked();

private:
    Ui::MainWindow *ui;  /**< Pointer to the generated UI elements */
    ModelPartList* partList;  /**< The data model managing the parts hierarchy */
    vtkSmartPointer<vtkRenderer> renderer;  /**< VTK renderer for 3D content */
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;  /**< VTK render window */
};
#endif // MAINWINDOW_H
