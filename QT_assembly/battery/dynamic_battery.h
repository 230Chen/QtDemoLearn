#ifndef DYNAMIC_BATTERY_H
#define DYNAMIC_BATTERY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Dynamic_battery; }
QT_END_NAMESPACE

class Dynamic_battery : public QWidget
{
    Q_OBJECT

public:
    Dynamic_battery(QWidget *parent = 0);
    ~Dynamic_battery();

private:
    Ui::Dynamic_battery *ui;

private slots:
    void initForm();
};
#endif // DYNAMIC_BATTERY_H
