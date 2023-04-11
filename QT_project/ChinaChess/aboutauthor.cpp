#include "aboutauthor.h"
#include "ui_aboutauthor.h"
#include <QIcon>
#include <QPainter>
#include <QPalette>

AboutAuthor::AboutAuthor(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::AboutAuthor)
{
    this->setWindowIcon(QIcon(":/resources/images/chess.svg"));
    ui->setupUi(this);

    QPalette pl = ui->textBrowser->palette();
    pl.setBrush(QPalette::Base, QBrush(QColor(255, 0, 0, 0)));
    ui->textBrowser->setPalette(pl);
}

AboutAuthor::~AboutAuthor()
{
    delete ui;
}

void AboutAuthor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/resources/images/background.jpg"));
    QDialog::paintEvent(event);
}
