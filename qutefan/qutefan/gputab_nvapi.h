#ifndef GPUTABNVAPI_H
#define GPUTABNVAPI_H

#include <QDateTime>

#include "gputab.h"
#include "control_nvapi.h"

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
#if USE_CHARTS
    void showChart();
#endif

private:
    ControlNvAPI        *m_api;
    ControlNvAPI::NvGPU *m_gpu;

#if USE_CHARTS
    typedef struct {
        QDateTime time;
        int level;
        int temp;
    } NvLogger;
    NvLogger history[600] = {};
    NvLogger* current_entry = history;
# endif
};

#endif // GPUTABNVAPI_H
