#include "qutefan.h"
#include "ui_qutefan.h"


QuteFan::QuteFan(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuteFan)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    trayIcon = new QuteFanTrayIcon(this);

#ifdef Q_OS_WIN
    nvapi = new QNvAPI();
    if(nvapi->isAvailable())
        initializeNvAPI();
    else
        QMessageBox::critical(this, "Error", "No supported hardware was found.");
#endif

    storeGpuDefaults();

    // Dynamically add tabs for as many GPUs as were found.
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        gpuTab[i] = new GpuTab();
        ui->tabWidgetGpu->addTab(gpuTab[i], QString("%1").arg(nvapi->gpu[i].name));
    }

    // Resize window to the minimum possible and don't let it be resized.
    this->resize(minimumSizeHint());
    this->setFixedSize(this->size());

    // Signals and slots
    connect(timer, SIGNAL(timeout()), this, SLOT(regulateFan()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->spinBoxInterval, SIGNAL(valueChanged(int)), this, SLOT(intervalChanged(int)));

    timer->start(ui->spinBoxInterval->text().toInt() * 1000);
    trayIcon->show();
}

QuteFan::~QuteFan()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++)
        restoreGpuDefaults(i);
    delete ui;
}

void QuteFan::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        if(firstCloseToTray) {
            trayIcon->showMessage(tr("Qutefan"), tr("Qutefan will now keep on running in the tray"));
            firstCloseToTray = false;
        }
        this->hide();
        trayIcon->updateMenu();
        event->ignore();
    }
}

void QuteFan::regulateFan()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetThermalSettings(nvapi->gpu[i].handle, 0, &nvapi->gpu[i].thermalSettings);
        gpuTab[i]->setTempValue(QString("%1°C").arg(nvapi->gpu[i].thermalSettings.sensor[0].currentTemp));


        GpuTab::FanMode mode = gpuTab[i]->getMode();

        if(mode == GpuTab::FanMode::Off) {
            if(mode != lastMode[i])
                restoreGpuDefaults(i);

        } else {
            NV_GPU_COOLER_LEVELS newCoolerLevels;
            newCoolerLevels.cooler[0].policy = 1;

            switch(mode) {
            case GpuTab::FanMode::Quiet:
                newCoolerLevels.cooler[0].level = NVAPI_MIN_COOLER_LEVEL;
                break;
            case GpuTab::FanMode::Fixed:
                newCoolerLevels.cooler[0].level = gpuTab[i]->getFixedLevel();
                break;
            case GpuTab::FanMode::Linear:
                newCoolerLevels.cooler[0].level = nvapi->gpu[i].thermalSettings.sensor[0].currentTemp + gpuTab[i]->getLinearOffset();
                break;
            case GpuTab::FanMode::Graph:
                break;
            }

            nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpu[i].handle, 0, &newCoolerLevels);
        }

        lastMode[i] = mode;


        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpu[i].handle, 0, &nvapi->gpu[i].coolerSettings);
        gpuTab[i]->setLevelValue(QString("%1%").arg(nvapi->gpu[i].coolerSettings.cooler[0].currentLevel));

    }

}

void QuteFan::intervalChanged(int value)
{
    timer->stop();
    timer->start(value * 1000);
}

void QuteFan::initializeNvAPI()
{
    nvapi->status = nvapi->Initialize();

    nvapi->status = nvapi->GetInterfaceVersionString(nvapi->version);
    qDebug("NvAPI version: %s", nvapi->version);

    nvapi->status = nvapi->EnumPhysicalGPUs();
    qDebug("Total number of GPU's = %u", nvapi->gpuCount);

    for(unsigned int i = 0; i < nvapi->gpuCount; i++)
        nvapi->GPU_GetFullName(nvapi->gpu[i].handle, nvapi->gpu[i].name);
}

void QuteFan::initializeNVCtrl()
{

}

void QuteFan::storeGpuDefaults()
{
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        nvapi->status = nvapi->GPU_GetCoolerSettings(nvapi->gpu[i].handle, 0, &nvapi->gpu[i].coolerSettings);
        defaultCoolerLevels[i].cooler[0].level = nvapi->gpu[i].coolerSettings.cooler[0].currentLevel;
        defaultCoolerLevels[i].cooler[0].policy = nvapi->gpu[i].coolerSettings.cooler[0].currentPolicy;
    }
    qDebug("Saved defaults");
}

void QuteFan::restoreGpuDefaults(unsigned int index)
{
    nvapi->status = nvapi->GPU_SetCoolerLevels(nvapi->gpu[index].handle, 0, &defaultCoolerLevels[index]);
    qDebug("Restored defaults for GPU %u", index);
}
