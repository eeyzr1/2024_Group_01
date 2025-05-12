/**
 * @file optiondialog.cpp
 * @brief Implementation of the OptionDialog class.
 * @details This dialog allows users to view and edit properties of a selected ModelPart, including name, visibility, color, and filters.
 * @version 1.0.0
 * @date 2025-05-12/2022
 * @author Woojin, Zhixing, Zhiyuan/Paul
 */
#include "optiondialog.h"
#include "ui_optiondialog.h"
/**
 * @brief Constructs the OptionDialog and initializes its UI.
 * @param parent Pointer to the parent widget.
 */
OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}
/**
 * @brief Destructor. Cleans up UI elements.
 */
OptionDialog::~OptionDialog()
{
    delete ui;
}
/**
 * @brief Populates the dialog UI fields with values from a ModelPart object.
 * @param part Pointer to the ModelPart whose data will be loaded into the dialog.
 */
void OptionDialog::setDialog(ModelPart* part)
{
    if (!part)
        return;
    ui->lineEditName->setText(part->data(0).toString());
    ui->isVisible->setChecked(part->visible());
    ui->spinBoxR->setValue(static_cast<int>(part->getColourR()));
    ui->spinBoxG->setValue(static_cast<int>(part->getColourG()));
    ui->spinBoxB->setValue(static_cast<int>(part->getColourB()));
    ui->checkBoxClipFilter->setChecked(part->clip());
    ui->checkBoxShrinkFilter->setChecked(part->shrink());
}
/**
 * @brief Updates the given ModelPart object with the current values from the dialog UI.
 * @param part Pointer to the ModelPart that will be updated.
 */
void OptionDialog::setModelPart(ModelPart* part)
{
    if (!part)
        return;
    part->set(0, ui->lineEditName->text());
    part->setVisible(ui->isVisible->isChecked());
    part->set(1, ui->isVisible->isChecked());
    part->setColour(static_cast<unsigned char>(ui->spinBoxR->value()),
                    static_cast<unsigned char>(ui->spinBoxG->value()),
                    static_cast<unsigned char>(ui->spinBoxB->value()));
    part->setClip(ui->checkBoxClipFilter->isChecked());
    part->setShrink(ui->checkBoxShrinkFilter->isChecked());
    part->setFilter();
}
