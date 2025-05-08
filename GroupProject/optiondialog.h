#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPart.h"
#include "ModelPartList.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

public slots:
    void setDialog(ModelPart* part);
    void setModelPart(ModelPart* part);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
