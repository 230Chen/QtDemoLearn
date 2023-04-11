
#include "chessmenumain.h"

#include <QMessageBox>
#include<QDialog>
#include<QPushButton>
#include<QVBoxLayout>

ChessMenuMain::ChessMenuMain(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("选择游戏方式");
    this->setFixedSize(250,120);
    this->setWindowIcon(QIcon(":/resources/images/chess.svg"));

    QVBoxLayout * lay = new QVBoxLayout(this);
    lay->addWidget(m_buttons[0]= new QPushButton("玩家自由对战"));
    lay->addWidget(m_buttons[1] = new QPushButton("玩家 VS AI"));
    lay->addWidget(m_buttons[2] = new QPushButton("玩家网络对战"));

    //1.游戏方式1，自己下棋
    connect(m_buttons[0],&QPushButton::clicked,[=](){
        this->hide();
        m_pAgainstYourself = new ChessBoard();
        m_pAgainstYourself->setWindowTitle("玩家自己对战");
        m_pAgainstYourself->show();

        //返回主窗口
        connect(m_pAgainstYourself,&ChessBoard::toMenu,[=](){
            m_pAgainstYourself->close();
            this->show();
        });

    });

    //游戏方式2，玩家与电脑下棋
    connect(m_buttons[1],&QPushButton::clicked,[=](){
        this->hide();

        m_machineGame = new MachineGame();
        m_machineGame->setWindowTitle("玩家和AI对战");
        m_machineGame->show();

        //返回主窗口
        connect(m_machineGame,&ChessBoard::toMenu,[=](){
            m_machineGame->close();
            this->show();
        });
    });

    //游戏方式3，双人局域网下棋
    connect(m_buttons[2], &QPushButton::clicked,[=](){
        this->hide();

        QMessageBox::StandardButtons ret = QMessageBox::question(NULL, "提示", "是否作为服务器启动[选择红方]?");

        bool bServer = false;
        if(ret == QMessageBox::Yes)
            bServer = true;

        m_netWorkGame = new NetWorkGame(bServer);
        m_netWorkGame->setWindowTitle("双人网络对战");
        m_netWorkGame->show();

        //返回主窗口
        connect(m_netWorkGame,&ChessBoard::toMenu,[=](){
            m_netWorkGame->close();
            this->show();
        });
    });

}

ChessMenuMain::~ChessMenuMain()
{
}
