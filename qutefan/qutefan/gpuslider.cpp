#include "gpuslider.h"
#include "ui_gpuslider.h"

GpuSlider::GpuSlider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GpuSlider)
{
    ui->setupUi(this);
}

GpuSlider::~GpuSlider()
{
    delete ui;
}
