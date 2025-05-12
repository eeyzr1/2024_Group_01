/**
 * @file main.cpp
 * @brief Entry point for the Qt GUI application.
 * @details This file contains the main function which initializes the Qt application
 *          and displays the main window interface of the program.
 * @version 1.0.0
 * @author Zhixing
 * @date 2025-05-12
 */
#include "mainwindow.h"

#include <QApplication>

/**
 * @brief Main function that launches the Qt GUI application.
 * @param argc Argument count from the command line.
 * @param argv Array of command-line arguments.
 * @return Exit code returned by the Qt application loop.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
