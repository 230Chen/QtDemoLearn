
#include "networkgame.h"
#include <QDebug>

NetWorkGame::NetWorkGame(bool isServer)
{
    m_tcpServer = NULL;
    m_tcpSocket = NULL;

    if (isServer)
    {
        m_isTcpServer = true;
        m_tcpServer = new QTcpServer(this);
        m_tcpServer->listen(QHostAddress::Any, 9999);

        connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    }
    else
    {
        m_isTcpServer = false;
        m_tcpSocket = new QTcpSocket(this);
        m_tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 9999);

        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotRecv()));
    }
}

NetWorkGame::~NetWorkGame()
{
}

void NetWorkGame::clickPieces(int checkId, int &row, int &col)
{
    if (m_isTcpServer) //作为服务器一方不能替黑子下棋
    {
        if (selectID == -1 && checkID != -1)
        {
            if (m_isTcpServer != c_allPawns[checkId].c_Red)
                return;
        }
    }
    else
    {
        //作为非下棋，无法选中红棋
        if (selectID == -1 && checkID != -1)
        {
            if (m_isTcpServer != c_allPawns[checkId].c_Red)
                return;
        }
    }

    whowin();
    ChessBoard::clickPieces(checkId, row, col);
    char arry[3];
    arry[0] = checkId;
    arry[1] = row;
    arry[2] = col;
    m_tcpSocket->write(arry, 3);
}

void NetWorkGame::slotNewConnection()
{
    if (m_tcpSocket)
        return;

    m_tcpSocket = m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotRecv()));
}

void NetWorkGame::slotRecv()
{
    QByteArray arr = m_tcpSocket->readAll();

    int s_checkedId = arr[0];
    int s_row = arr[1];
    int s_col = arr[2];

    qDebug() << s_checkedId << "   " << s_row << "   " << s_col << "   ";
    ChessBoard::clickPieces(s_checkedId, s_row, s_col);
}
