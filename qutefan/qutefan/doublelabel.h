#pragma once

#include <QWidget>

namespace Ui {
class DoubleLabel;
}

class DoubleLabel : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleLabel(QWidget *parent = nullptr);
    explicit DoubleLabel(QWidget *parent, const QString &);
    explicit DoubleLabel(QWidget *parent, const QString &, int);
    ~DoubleLabel();

    void setFormat(const QString &, int = 100);
    void setValue(int, bool = false);

private:
    QString m_format = "%1";
    int     m_maximum = 0;
    Ui::DoubleLabel *ui;
};

