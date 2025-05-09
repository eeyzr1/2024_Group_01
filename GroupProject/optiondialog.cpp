#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

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
}
