#ifndef PICTUREPROCESSORUI_H
#define PICTUREPROCESSORUI_H

#include <QtWidgets/QMainWindow>
#include "ui_pictureprocessorui.h"

class PictureProcessorUI : public QMainWindow
{
    Q_OBJECT

public:
    PictureProcessorUI(QWidget *parent = 0);
    ~PictureProcessorUI();

private:
    Ui::PictureProcessorUIClass ui;
};

#endif // PICTUREPROCESSORUI_H
