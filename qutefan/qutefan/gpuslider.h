#pragma once

#include <QWidget>

namespace Ui {
class GpuSlider;
}

class GpuSlider : public QWidget
{
    Q_OBJECT

public:
    explicit GpuSlider(QWidget *parent = nullptr);
    ~GpuSlider();

private:
    Ui::GpuSlider *ui;
};
