#include "qutefan.h"
#include "ui_qutefan.h"


QuteFan::QuteFan(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuteFan)
{
    ui->setupUi(this);

    nvapi = new QNvAPI();
    trayIcon = new QSystemTrayIcon(this);
    timer = new QTimer(this);

    if(!nvapi->Available())
        QMessageBox::critical(this, "Error", "NvAPI is not available.");

    nvapi->status = nvapi->Initialize();

    for(unsigned int i = 0; nvapi->status == NVAPI_OK; i++) {
        nvapi->status = nvapi->EnumNvidiaDisplayHandle(i, &nvapi->displayHandle[i]);
        if (nvapi->status == NVAPI_OK)
            nvapi->displayCount++;
    }
    qDebug("No of displays = %u", nvapi->displayCount);

    nvapi->status = nvapi->EnumPhysicalGPUs(nvapi->gpuHandle, &nvapi->gpuCount);
    qDebug("Total number of GPU's = %u", nvapi->gpuCount);

    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        nvapi->GPU_GetFullName(nvapi->gpuHandle[i], nvapi->gpuName[i]);

        gpuTab[i] = new GpuTab();
        gpuTab[i]->setObjectName(QStringLiteral("gpuTab_%1").arg(i));
        ui->tabWidgetGpu->addTab(gpuTab[i], QString("%1").arg(nvapi->gpuName[i]));
    }

    nvapi->status = nvapi->GetInterfaceVersionString(nvapi->version);
    qDebug("Version: %s", nvapi->version);

    trayIcon->setIcon(windowIcon());
    trayIcon->show();

    // Signals and slots
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(timer, SIGNAL(timeout()), this, SLOT(regulate()));

    saveDefaults();
    timer->start(1000);
}

QuteFan::~QuteFan()
{
    restoreDefaults();
    delete ui;
}

void QuteFan::saveDefaults()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpuHandle[i], 0, &nvapi->coolerSettings[i]);
        defaultCoolerLevels[i].cooler[0].level = nvapi->coolerSettings[i].cooler[0].currentLevel;
        defaultCoolerLevels[i].cooler[0].policy = nvapi->coolerSettings[i].cooler[0].currentPolicy;
    }
    qDebug("Saved defaults");
}

void QuteFan::restoreDefaults()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++)
        nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &defaultCoolerLevels[i]);
    qDebug("Restored defaults");
}

void QuteFan::regulate()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetThermalSettings(nvapi->gpuHandle[i], 0, &nvapi->thermalSettings[i]);
        gpuTab[i]->setTempValue(QString("%1°C").arg(nvapi->thermalSettings[i].sensor[0].currentTemp));

        NV_GPU_COOLER_LEVELS newCoolerLevels;
        newCoolerLevels.cooler[0].policy = 1;


        GpuTab::FanMode mode = gpuTab[i]->getMode();
        if(mode == GpuTab::FanMode::Off) {
            if(mode != lastMode[i]) restoreDefaults();

        } else if(mode == GpuTab::FanMode::Quiet) {
            newCoolerLevels.cooler[0].level = NVAPI_MIN_COOLER_LEVEL;
            nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &newCoolerLevels);

        } else if(mode == GpuTab::FanMode::Fixed) {
            newCoolerLevels.cooler[0].level = gpuTab[i]->getFixedLevel();
            nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &newCoolerLevels);

        } else if(mode == GpuTab::FanMode::Linear) {
            newCoolerLevels.cooler[0].level = nvapi->thermalSettings[i].sensor[0].currentTemp + gpuTab[i]->getLinearOffset();
            nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &newCoolerLevels);

        } else if(mode == GpuTab::FanMode::Graph) {

        }

        lastMode[i] = mode;


        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpuHandle[i], 0, &nvapi->coolerSettings[i]);
        gpuTab[i]->setLevelValue(QString("%1%").arg(nvapi->coolerSettings[i].cooler[0].currentLevel));
    }

}
