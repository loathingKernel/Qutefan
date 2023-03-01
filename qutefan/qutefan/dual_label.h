#pragma once

#include <QWidget>

namespace Ui {
class DualLabel;
}

class DualLabel : public QWidget
{
    Q_OBJECT

public:
    explicit DualLabel(QWidget *parent = nullptr);
    ~DualLabel();

    void setCurrent(const QString &);
    void setMaximum(const QString &);

private:
    Ui::DualLabel *ui;
};

