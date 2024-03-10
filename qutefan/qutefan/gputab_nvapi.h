#pragma once

#include <QDateTime>

#include "gputab.h"
#include "qnvapi.h"

namespace Ui {
class GpuTabNvAPI;
}

class GpuTabNvAPI : public GpuTab
{
public:
    explicit GpuTabNvAPI(ControlNvAPI*, ControlNvAPI::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNvAPI();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFans();
    void displayFrequencies();

private slots:
    void resetMaximums();

private:
    ControlNvAPI        *m_api;
    ControlNvAPI::NvGPU *m_gpu;
};
