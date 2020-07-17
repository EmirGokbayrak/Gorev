#include "imageform.h"
#include "ui_imageform.h"

ImageForm::ImageForm(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ImageForm)
{
    ui->setupUi(this);
}

ImageForm::~ImageForm()
{
    delete ui;
}
