
#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include "chessboard.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class NetWorkGame : public ChessBoard
{
    Q_OBJECT
public:
    NetWorkGame(bool isServer);
    ~NetWorkGame();

    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;

    virtual void clickPieces(int checkId, int &row, int &col);

public slots:
    void slotNewConnection();
    void slotRecv();
};

#endif // NETWORKGAME_H
