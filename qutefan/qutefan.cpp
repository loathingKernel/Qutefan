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

    // Dynamically add tabs for as many GPUs as were found.
    for(unsigned int i = 0; i < nvapi->gpuCount; i++) {
        gpuTab[i] = new GpuTab(nvapi, &nvapi->gpu[i]);
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
        gpuTab[i]->setGPUDefaults();
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
    for(unsigned int i = 0; i < nvapi->gpuCount; i++)
        gpuTab[i]->regulateFan();
}

void QuteFan::intervalChanged(int value)
{
    timer->stop();
    timer->start(value * 1000);
}

void QuteFan::initializeNvAPI()
{
    nvapi->Initialize();

    nvapi->GetInterfaceVersionString(nvapi->version);
    qDebug("NvAPI version: %s", nvapi->version);

    nvapi->EnumPhysicalGPUs();
    qDebug("Total number of GPU's = %u", nvapi->gpuCount);

    for(unsigned int i = 0; i < nvapi->gpuCount; i++)
        nvapi->GPU_GetFullName(nvapi->gpu[i].handle, nvapi->gpu[i].name);
}

void QuteFan::initializeNVCtrl()
{

}

