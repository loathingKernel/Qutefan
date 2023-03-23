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
    m_control_nvapi = new ControlNvAPI();

    if(m_control_nvapi->available()) {
        m_control_nvapi->initialize();

        for(unsigned int g = 0; g < m_control_nvapi->gpu_count; ++g) {
            ControlNvAPI::NvGPU *gpu = m_control_nvapi->getGpuByIndex(g);
            GpuTabNvAPI *tab = new GpuTabNvAPI(m_control_nvapi, gpu, settings, this);
            m_gpu_tab.append(tab);
            ui->tabWidgetGpu->addTab(
                m_gpu_tab[g], QString("%1").arg(m_control_nvapi->name(gpu))
            );
        }
    }
#elif defined(Q_OS_LINUX)
    m_control_nvctrl = new ControlNVCtrl();

    if(m_control_nvctrl->available()) {
        m_control_nvctrl->initialize();

        for(int g = 0; g < m_control_nvctrl->gpu_count; ++g) {
            ControlNVCtrl::NvGPU *gpu = m_control_nvctrl->getGpuByIndex(g);
            GpuTabNVCtrl *tab = new GpuTabNVCtrl(m_control_nvctrl, gpu, settings, this);
            m_gpu_tab.append(tab);
            ui->tabWidgetGpu->addTab(
                m_gpu_tab[g], QString("%1").arg(m_control_nvctrl->name(gpu)));
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
    foreach(GpuTab* tab, m_gpu_tab) {
        tab->saveGpuSettings();
        tab->setGPUDefaults();
    }
    for (int t = m_gpu_tab.size() - 1; t >= 0; --t)
        delete m_gpu_tab[t];
    saveSettings();
#if defined(Q_OS_WIN)
    delete m_control_nvapi;
#elif defined(Q_OS_LINUX)
    delete m_control_nvctrl;
#endif
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
    foreach(GpuTab* tab, m_gpu_tab) {
        tab->regulateFans();
        tab->displayFrequencies();
    }
}

void QuteFan::intervalChanged(int value)
{
    timer->stop();
    timer->start(value * 1000);
}
