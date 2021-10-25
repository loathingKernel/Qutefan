#include "qutefan.h"
#include "ui_qutefan.h"


QuteFan::QuteFan(QWidget *parent) : QMainWindow(parent), ui(new Ui::QuteFan)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    timer = new QTimer(this);
    tray_icon = new QuteFanTrayIcon(this);

    settings = new QSettings(
                QSettings::IniFormat,
                QSettings::UserScope,
                qApp->organizationName(),
                qApp->applicationName(),
                this);

#if defined(Q_OS_WIN)
    qf_nvapi = new QuteFanNvAPI();
    if(qf_nvapi->available()) {
        qf_nvapi->initialize();
        // Dynamically add tabs for as many GPUs as were found.
        for(unsigned long i = 0; i < qf_nvapi->gpu_count; i++) {
            gpu_tabs.append(new GpuTabNvAPI(qf_nvapi, &qf_nvapi->gpu[i], settings, this));
            ui->tabWidgetGpu->addTab(gpu_tabs[static_cast<int>(i)], QString("%1").arg(qf_nvapi->gpu[i].name));
        }
    }
#elif defined(Q_OS_LINUX)
    qf_nvctrl = new QuteFanNVCtrl();
    if(qf_nvctrl->available()) {
        qf_nvctrl->initialize();
        for(int i = 0; i < qf_nvctrl->gpu_count; i++) {
            gpu_tabs.append(new GpuTabNVCtrl(qf_nvctrl, &qf_nvctrl->gpu[i], settings, this));
            ui->tabWidgetGpu->addTab(gpu_tabs[static_cast<int>(i)], QString("%1").arg(qf_nvctrl->gpu[i].name));
        }
    }
#endif
    else {
        QMessageBox::critical(this, "Error", "No supported hardware was found.");
        qApp->quit();
    }


    // Resize window to the minimum possible and don't let it be resized.
    this->resize(minimumSizeHint());
    this->setFixedSize(this->size());

    // Signals and slots
    connect(timer, SIGNAL(timeout()), this, SLOT(regulateFan()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onActionAboutTriggered()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(onActionAboutQtTriggered()));
    connect(ui->spinBoxInterval, SIGNAL(valueChanged(int)), this, SLOT(intervalChanged(int)));

    loadSettings();

    timer->start(ui->spinBoxInterval->value() * 1000);
    tray_icon->show();
}

QuteFan::~QuteFan()
{
    foreach(GpuTab* tab, gpu_tabs) {
        tab->saveGpuSettings();
        tab->setGPUDefaults();
    }
    saveSettings();
    delete ui;
}

void QuteFan::loadSettings()
{
    settings->beginGroup("Application");
    ui->spinBoxInterval->setValue(settings->value("interval", 1).toInt());
    settings->endGroup();
}

void QuteFan::saveSettings()
{
    settings->beginGroup("Application");
    settings->setValue("interval", ui->spinBoxInterval->value());
    settings->endGroup();
}

void QuteFan::closeEvent(QCloseEvent* event)
{
    if (tray_icon->isVisible()) {
        if(first_close_to_tray) {
            tray_icon->showMessage(tr("Qutefan"), tr("Qutefan will now keep on running in the tray"));
            first_close_to_tray = false;
        }
        this->hide();
        tray_icon->updateMenu();
        event->ignore();
    }
}

void QuteFan::onActionAboutTriggered()
{
    QString format =
            "<h2>%1</h2>"
            "<b>Version:</b> %2<br/>"
            "<b>Author:</b> %3<br/>"
            "<b>Github:</b> <a href=\"https://www.github.com/loathingkernel/qutefan\">Qutefan</a>";

    QString txt = format.arg(qApp->applicationName(),
                             qApp->applicationVersion(),
                             qApp->organizationName());

    QMessageBox::about(this, ui->actionAbout->text(), txt);
}

void QuteFan::onActionAboutQtTriggered()
{
    QMessageBox::aboutQt(this);
}

void QuteFan::regulateFan()
{
    foreach(GpuTab* tab, gpu_tabs) {
        tab->regulateFan();
        tab->displayStatus();
    }
}

void QuteFan::intervalChanged(int value)
{
    timer->stop();
    timer->start(value * 1000);
}
