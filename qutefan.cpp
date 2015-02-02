#include "qutefan.h"
#include "ui_qutefan.h"


QuteFan::QuteFan(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuteFan)
{
    ui->setupUi(this);

    nvapi = new QNvAPI();
    if(!nvapi->Available()) {
        QMessageBox::critical(this, "Error", "NvAPI is not available.");
        exit(1);
    }

    nvapi->status = nvapi->Initialize();


    for (i = 0; nvapi->status == NVAPI_OK; i++) {
        nvapi->status = nvapi->EnumNvidiaDisplayHandle(i, &nvapi->displayHandle[i]);
        if (nvapi->status == NVAPI_OK)
            nvapi->displayCount++;
    }
    qDebug("No of displays = %u", nvapi->displayCount);


    nvapi->status = nvapi->EnumPhysicalGPUs(nvapi->gpuHandle, &nvapi->gpuCount);
    qDebug("Total number of GPU's = %u", nvapi->gpuCount);

    for(i = 0; i < nvapi->gpuCount; i++) {
        nvapi->GPU_GetFullName(nvapi->gpuHandle[i], nvapi->gpuName[i]);
        ui->tabWidget->setTabText(i, QString("%1").arg(nvapi->gpuName[i]));
    }


    nvapi->status = nvapi->GetInterfaceVersionString(nvapi->version);
    qDebug("Version: %s", nvapi->version);


    saveDefaults();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(regulate()));

    timer->start(1000);
}

QuteFan::~QuteFan()
{
    restoreDefaults();
    delete ui;
}

void QuteFan::saveDefaults()
{
    for(i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpuHandle[i], 0, &nvapi->coolerSettings[i]);

        defaultCoolerLevels[i].cooler[0].level = nvapi->coolerSettings[i].cooler[0].currentLevel;
        defaultCoolerLevels[i].cooler[0].policy = nvapi->coolerSettings[i].cooler[0].currentPolicy;
    }
}

void QuteFan::restoreDefaults()
{
    for(i = 0; i < nvapi->gpuCount; i++)
        nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &defaultCoolerLevels[i]);
}

void QuteFan::regulate()
{
    for(i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetThermalSettings(nvapi->gpuHandle[i], 0, &nvapi->thermalSettings[i]);
        ui->labelTempValue->setText(QString("%1°C").arg(nvapi->thermalSettings[i].sensor[0].currentTemp));

        NV_GPU_COOLER_LEVELS newCoolerLevels;
        newCoolerLevels.cooler[0].level = nvapi->thermalSettings[i].sensor[0].currentTemp;
        newCoolerLevels.cooler[0].policy = 1;
        nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpuHandle[i], 0, &newCoolerLevels);

        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpuHandle[i], 0, &nvapi->coolerSettings[i]);
        ui->labelLevelValue->setText(QString("%1%").arg(nvapi->coolerSettings[i].cooler[0].currentLevel));
    }

}
