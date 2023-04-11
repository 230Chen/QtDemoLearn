
#ifndef CHESSMENUMAIN_H
#define CHESSMENUMAIN_H

#include <QDialog>
#include <QWidget>
#include "chessboard.h"
#include "machinegame.h"
#include"networkgame.h"

class ChessMenuMain : public QDialog
{
    Q_OBJECT
public:
    explicit ChessMenuMain(QWidget *parent = nullptr);
    ~ChessMenuMain();

private:
    QPushButton *m_buttons[3];
    ChessBoard *m_pAgainstYourself;
    MachineGame * m_machineGame;
    NetWorkGame *m_netWorkGame;
};

#endif // CHESSMENUMAIN_H
