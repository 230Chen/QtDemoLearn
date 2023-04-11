#ifndef CHESSPIECES_H
#define CHESSPIECES_H

#include <QString>

/*
    棋子
*/
class ChessPieces
{
public:
    ChessPieces();
    ~ChessPieces();

    void init(int id);               //初始化
    QString getName(bool isRedSide); //棋子对应汉字
    QString getColText(int col);
    QString getRowText(int row);
    QString getMoveText(int rowFrow, int rowto);

    enum c_chessTYPE
    {
        PAWN,
        CANNONS,
        ROOKS,
        ELEPHANT,
        GUARD,
        KNIGHTS,
        KING
    };

    int c_ROW;               //行
    int c_COL;               //列
    int c_ID;                // ID
    bool c_Dead;             //死亡状态
    bool c_Red;              //是否是红方
    c_chessTYPE c_chessType; //具体棋子

    QString colTextRed[9] = {"九", "八", "七", "六", "五", "四", "三", "二", "一"};
    QString colTextBlack[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    QString colTextRedTurn[9] = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};
};

#endif // CHESSPIECES_H
