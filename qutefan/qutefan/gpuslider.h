#pragma once

#include <QWidget>

#include "control.h"

namespace Ui {
class GpuSlider;
}

class GpuSlider : public QWidget
{
    Q_OBJECT

public:
    explicit GpuSlider(QWidget *parent = nullptr);
    ~GpuSlider();

    void setLimits(Control::Limits);
    unsigned int getValue();

private:
    unsigned int m_maximum;
    unsigned int m_minimum;
    int m_denominator;

    Ui::GpuSlider *ui;
};
