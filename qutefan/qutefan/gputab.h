#pragma once

#include <QLabel>
#include <QWidget>
#include <QSettings>

#if USE_CHARTS
#include <QtCharts>
#endif

#include "control.h"
#include "doublelabel.h"
#include "gpuslider.h"

namespace Ui {
class GpuTab;
}

class GpuTab : public QWidget
{
    Q_OBJECT

public:
    explicit GpuTab(QSettings*, QWidget* parent = nullptr);
    ~GpuTab();

    enum class FanMode {
        Off = 0,
        Quiet,
        Fixed,
        Linear,
        Graph
    };

    GpuTab::FanMode getMode();
    GpuTab::FanMode last_mode = FanMode::Off;

    void saveSettings(const QString &);
    void loadSettings(const QString &);

    virtual void saveGpuSettings() = 0;
    virtual void setGPUDefaults() = 0;
    virtual void regulateFans() = 0;
    virtual void displayFrequencies() = 0;

public slots:
    virtual void applyOverclock() = 0;
    virtual void resetOverclock() = 0;
    virtual void resetMaximums() = 0;
#if USE_CHARTS
    virtual void showChart() = 0;
#endif

protected:
    void addCoolers(Control::CoolerLevels);
    void setPowerLimits(Control::Limits);

    QLabel*      m_temp_label;
    DoubleLabel* m_temp_info;

    QVector<QLabel*>      m_fan_label;
    QVector<DoubleLabel*> m_fan_info;

    GpuSlider*   m_core_slider;
    GpuSlider*   m_mem_slider;
    GpuSlider*   m_power_slider;

#if USE_CHARTS
    typedef struct {
        QDateTime time;
        int level;
        int temp;
    } NvLogger;
    NvLogger history[600] = {};
    NvLogger* current_entry = history;
# endif

    Ui::GpuTab* ui;

private:
    QSettings* m_settings;
};
