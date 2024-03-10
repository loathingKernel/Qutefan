#pragma once

#include <QDateTime>

#include "gputab.h"
#include "qnvml.h"

namespace Ui {
class GpuTabNvml;
}

class GpuTabNvml : public GpuTab
{
public:
    GpuTabNvml(ControlNvml*, ControlNvml::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNvml();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFans();
    void displayFrequencies();

private slots:
    void resetMaximums();

private:
    ControlNvml           *m_api;
    ControlNvml::NvGPU    *m_gpu;
};
