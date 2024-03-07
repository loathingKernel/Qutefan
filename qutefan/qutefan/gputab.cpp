#include "ui_gputab.h"
#include "gputab.h"

GpuTab::GpuTab(QSettings* settings, QWidget* parent) : QWidget(parent), ui(new Ui::GpuTab)
{
    ui->setupUi(this);

    m_settings = settings;

    // Hide unfinished UI elements
    // ui->overclockGroup->hide();
    ui->radioButtonGraph->hide();
    ui->pushButtonGraph->hide();
    ui->pushButtonChart->hide();

    ui->groupBoxCharts->hide();
    ui->spinBoxFixedLevel->setSingleStep(5);

    m_temp_label = new QLabel("Temperature:", this);
    m_temp_info = new DoubleLabel(this, "%1°C", 100);
    ui->formLayoutStatus->insertRow(0, m_temp_label, m_temp_info);

    int row;
    QFormLayout::ItemRole role;

    m_core_slider = new GpuSlider(this);
    m_core_slider->setDisabled(true);
    ui->overclockLayout->getWidgetPosition(ui->coreClockCheck, &row, &role);
    ui->overclockLayout->setWidget(row, QFormLayout::FieldRole, m_core_slider);
    connect(ui->coreClockCheck, &QCheckBox::stateChanged, m_core_slider, &GpuSlider::setEnabled);

    m_mem_slider = new GpuSlider(this);
    m_mem_slider->setDisabled(true);
    ui->overclockLayout->getWidgetPosition(ui->memClockCheck, &row, &role);
    ui->overclockLayout->setWidget(row, QFormLayout::FieldRole, m_mem_slider);
    connect(ui->memClockCheck, &QCheckBox::stateChanged, m_mem_slider, &GpuSlider::setEnabled);

    m_power_slider = new GpuSlider(this);
    m_power_slider->setDisabled(true);
    ui->overclockLayout->getWidgetPosition(ui->powerLimitCheck, &row, &role);
    ui->overclockLayout->setWidget(row, QFormLayout::FieldRole, m_power_slider);
    connect(ui->powerLimitCheck, &QCheckBox::stateChanged, m_power_slider, &GpuSlider::setEnabled);

#if USE_CHARTS
    connect(ui->pushButtonChart, SIGNAL(pressed()), this, SLOT(showChart()));
#endif
    connect(ui->pushButtonReset, &QPushButton::pressed, this, &GpuTab::resetMaximums);
}

GpuTab::~GpuTab()
{
    m_temp_label->deleteLater();
    m_temp_info->deleteLater();
    for (int c = m_fan_label.size() - 1; c >= 0; --c)
        m_fan_label[c]->deleteLater();
    for (int c = m_fan_info.size() - 1; c >= 0; --c)
        m_fan_info[c]->deleteLater();
    delete ui;
}

GpuTab::FanMode GpuTab::getMode()
{
    if(ui->radioButtonOff->isChecked())     return FanMode::Off;
    if(ui->radioButtonQuiet->isChecked())   return FanMode::Quiet;
    if(ui->radioButtonFixed->isChecked())   return FanMode::Fixed;
    if(ui->radioButtonLinear->isChecked())  return FanMode::Linear;
    if(ui->radioButtonGraph->isChecked())   return FanMode::Graph;

    return FanMode::Off;
}

void GpuTab::saveSettings(const QString &uuid)
{
    m_settings->beginGroup(uuid);
    m_settings->setValue("mode", static_cast<int>(getMode()));
    m_settings->setValue("fixed", ui->spinBoxFixedLevel->value());
    m_settings->setValue("linear", ui->spinBoxLinearOffset->value());
    m_settings->endGroup();
}

void GpuTab::loadSettings(const QString &uuid)
{
    m_settings->beginGroup(uuid);
    int mode = m_settings->value("mode", 0).toInt();
    if (mode == static_cast<int>(FanMode::Off)) ui->radioButtonOff->setChecked(true);
    if (mode == static_cast<int>(FanMode::Quiet)) ui->radioButtonQuiet->setChecked(true);
    if (mode == static_cast<int>(FanMode::Fixed)) ui->radioButtonFixed->setChecked(true);
    if (mode == static_cast<int>(FanMode::Linear)) ui->radioButtonLinear->setChecked(true);
    if (mode == static_cast<int>(FanMode::Graph)) ui->radioButtonGraph->setChecked(true);
    ui->spinBoxFixedLevel->setValue(m_settings->value("fixed", 0).toInt());
    ui->spinBoxLinearOffset->setValue(m_settings->value("linear", 0).toInt());
    m_settings->endGroup();
}

void GpuTab::addCoolers(Control::CoolerLevels levels)
{
    for (int c = 0; c < levels.count; ++c) {
        QLabel* label = new QLabel(QString("Fan %1 level:").arg(c));
        DoubleLabel* info = new DoubleLabel(this, "%1%", 100);
        info->setValue(levels.current[c]);
        ui->formLayoutStatus->insertRow(c + 1, label, info);
        m_fan_label.push_back(label);
        m_fan_info.push_back(info);
    }
}
