#include "qutefan.h"
#include "ui_qutefan.h"

QuteFan::QuteFan(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuteFan)
{
    ui->setupUi(this);
}

QuteFan::~QuteFan()
{
    delete ui;
}
