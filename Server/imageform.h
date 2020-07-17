#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include <QWidget>

namespace Ui {
    class ImageForm;
}

class ImageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ImageForm(QWidget* parent = nullptr);
    ~ImageForm();

private:
    Ui::ImageForm* ui;
};

#endif // IMAGEFORM_H
