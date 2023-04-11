#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QMainWindow>
#include <QFrame>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QtGlobal>

#include "chesspieces.h"
#include "chessvoice.h"
#include "chessstep.h"
#include "aboutauthor.h"


/*
    棋盘
*/

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ChessBoard;
}
QT_END_NAMESPACE

class ChessBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();

    bool isRed(int id);
    bool isDead(int id);
    void killStone(int id);     //吃子
    void rollbackStone(int id); //回退

    void moveStone(int moveid, int row, int col); //移动棋子
    bool sameColor(int moveid, int killId);       //棋子是否同色

    int getStoneId(int row, int col);
    //车，炮辅助函数，判断是否在同一行，返回直线中棋子个数
    int getStoneAliveCount(int row1, int col1, int row2, int col2);

    void whowin(); //谁胜谁负

    bool isCheck(QPoint pt, int &row, int &col);          //是否选中棋子
    int relation(int row1, int col1, int row2, int col2); //计算选中棋子位置和需要移动位置的关系
    QPoint getRealPoint(QPoint pt);                       // 使mouseMoveEvent取得的坐标同Painter的坐标一致
    bool isGeneral();                                     //移动后是否<将军>

private:
    bool encounterGenerals();          //对将
    bool havePieces(int row, int col); //判断是否有棋子
    void reset();                      //重置棋盘
    void victoryMessageBox(QString title, QString msg);

    //视图
public:
    QPoint center(int row, int col); //棋盘坐标转换
    QPoint center(int id);

    virtual void paintEvent(QPaintEvent *);                            //绘画棋盘
    void drawChessPieces(QPainter &painter, int id);                   //绘画单个具体的棋子
    void drawLastStep(QPainter &painter, QVector<ChessStep *> &steps); //绘制上次移动棋子的起止位置
    void drawTextStep();                                               //绘制文本棋谱

    /*  -----象棋移动规则-----
        卒/兵
        炮
        车
        马
        象/相
        士/仕
        将/帅
    */
    bool canMove(int moveId, int killId, int row, int col);
    bool canSelect(int id); //  允许红方or黑方选棋
    bool canMovePawn(int moveId, int killId, int row, int col);
    bool canMoveCannons(int moveId, int killId, int row, int col);
    bool canMoveRooks(int moveId, int killId, int row, int col);
    bool canMoveKnights(int moveId, int killId, int row, int col);
    bool canMoveElephant(int moveId, int killId, int row, int col);
    bool canMoveGuard(int moveId, int killId, int row, int col);
    bool canMoveKing(int moveId, int killId, int row, int col);

    void init();

    //移动
    virtual void mouseReleaseEvent(QMouseEvent *ev);                                      //鼠标释放事件
    void click(QPoint pt);                                                                //点击转换像素
    virtual void clickPieces(int id, int row, int col);                                   //点击
    void trySelectStone(int id);                                                          //尝试选择棋子
    void tryMoveStone(int killid, int row, int col);                                      //尝试移动
    void doMoveStone(int moveid, int killid, int row, int col);                           //执行移动棋子
    void saveStep(int moveid, int killid, int row, int col, QVector<ChessStep *> &steps); //保存步数
    QString textStep(int moveid, int row, int col);                                       //文本棋谱

    void backOne();             //悔棋1次
    void back(ChessStep *step); //悔棋到指定步数
    virtual void back();        //悔棋

    ChessPieces c_allPawns[32];          //所有的棋子
    QVector<ChessStep *> repentanceStep; //悔棋步数
    ChessVoice c_ChessVoice;             //下棋音效

    int pawnRadius;       //棋子半径
    int interfaceMargins; //界面边距
    int spacing;          //间距
    int selectID;         //选中棋子， 非-1 ： 走棋 | -1 ：选择棋子
    int checkID;          //将要击杀棋子ID
    bool m_isRed;         //是否红方回合
    bool m_isTcpServer;
    bool m_isOver;          //是否结束
    bool m_displaysSteps;   //显示步数
    QString textStepRecord; //文本棋谱字符串

private slots:
    void updateTime();

    void on_Btn_start_clicked();
    void on_Btn_reset_clicked();
    void on_Btn_restart_clicked();
    void on_Btn_back_clicked();
    void on_Btn_showstep_clicked();
    void on_Btn_showmenu_clicked();
    void on_Btn_about_clicked();

private:
    Ui::ChessBoard *ui;

    //定时器
    QTimer *m_timer;
    QTime *m_timeRecord; //记录时间
    bool m_isStart;      //是否开始
    AboutAuthor *m_about;

signals:
    void toMenu();
};

#endif // CHESSBOARD_H
