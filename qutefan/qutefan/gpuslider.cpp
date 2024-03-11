#include "gpuslider.h"
#include "ui_gpuslider.h"

GpuSlider::GpuSlider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GpuSlider)
{
    ui->setupUi(this);

    connect(ui->slider, qOverload<int>(&QSlider::valueChanged), ui->spinbox, &QSpinBox::setValue);
    connect(ui->spinbox, qOverload<int>(&QSpinBox::valueChanged), ui->slider, &QSlider::setValue);
}

GpuSlider::~GpuSlider()
{
    delete ui;
}

void GpuSlider::setLimits(Control::Limits limits)
{
    QString suffix;

    switch (limits.type) {
    case Control::LimitType::CONTROL_LIMIT_FREQUENCY: {
        ui->spinbox->setSingleStep(1);
        suffix = "Mhz";
        break;
    }
    case Control::LimitType::CONTROL_LIMIT_WATTAGE: {
        ui->spinbox->setSingleStep(5);
        suffix = "W";
        break;
    }
    default: {
        suffix = "";
    }
    }

    m_minimum = limits.minimum;
    m_maximum = limits.maximum;
    m_denominator = limits.denominator;

    int minimum = limits.minimum/limits.denominator;
    int maximum = limits.maximum/limits.denominator;
    int current = limits.current/limits.denominator;

    ui->slider->setMinimum(minimum);
    ui->slider->setMaximum(maximum);
    ui->slider->setValue(current);

    ui->spinbox->setMinimum(minimum);
    ui->spinbox->setMaximum(maximum);
    ui->spinbox->setValue(current);

    ui->minLabel->setText(QString("%1").arg(minimum).append(suffix));
    ui->maxLabel->setText(QString("%1").arg(maximum).append(suffix));
    ui->limitLabel->setText(QString("%1").arg(current).append(suffix));
}

unsigned int GpuSlider::getValue()
{
    unsigned int limit = ui->slider->value();
    limit = limit * m_denominator;
    if (limit < m_minimum ) limit = m_minimum;
    if (limit > m_maximum ) limit = m_maximum;
    return limit;
}
