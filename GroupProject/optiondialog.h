/**
 * @file optiondialog.h
 * @brief Declaration of the OptionDialog class.
 * @details This dialog allows user interaction with ModelPart properties such as visibility, name, color, and rendering filters.
 * @version 1.0.0
 * @date 2025-05-12/2022
 * @author Woojin, Zhixing, Zhiyuan/Paul
 */
#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPart.h"
#include "ModelPartList.h"

namespace Ui {
class OptionDialog;
}

/**
 * @class OptionDialog
 * @brief Dialog for viewing and editing ModelPart properties.
 */
class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the OptionDialog window.
     * @param parent Pointer to the parent QWidget.
     */
    explicit OptionDialog(QWidget *parent = nullptr);
    /**
     * @brief Destructor. Cleans up UI elements.
     */
    ~OptionDialog();

public slots:
    /**
     * @brief Populates the dialog fields with data from the given ModelPart.
     * @param part Pointer to the ModelPart to edit.
     */
    void setDialog(ModelPart* part);
    /**
     * @brief Updates the ModelPart object with the values from the dialog UI.
     * @param part Pointer to the ModelPart to update.
     */
    void setModelPart(ModelPart* part);

private:
    Ui::OptionDialog *ui;/**< Pointer to the UI form. */
};

#endif // OPTIONDIALOG_H
