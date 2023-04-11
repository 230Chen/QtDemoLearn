#include "chesspieces.h"

struct POS
{
    int t_row;
    int t_col;
    ChessPieces::c_chessTYPE t_chessType;
};

//定义基础的16棋子 [预定作为上方使用，黑棋使用]
POS tPos[16] = {
    {0, 0, ChessPieces::ROOKS},
    {0, 1, ChessPieces::KNIGHTS},
    {0, 2, ChessPieces::ELEPHANT},
    {0, 3, ChessPieces::GUARD},
    {0, 4, ChessPieces::KING},
    {0, 5, ChessPieces::GUARD},
    {0, 6, ChessPieces::ELEPHANT},
    {0, 7, ChessPieces::KNIGHTS},
    {0, 8, ChessPieces::ROOKS},

    {2, 1, ChessPieces::CANNONS},
    {2, 7, ChessPieces::CANNONS},
    {3, 0, ChessPieces::PAWN},
    {3, 2, ChessPieces::PAWN},
    {3, 4, ChessPieces::PAWN},
    {3, 6, ChessPieces::PAWN},
    {3, 8, ChessPieces::PAWN}};

ChessPieces::ChessPieces()
{
}

ChessPieces::~ChessPieces()
{
}

//对每一个棋子进行检验判断而后赋相应的值
void ChessPieces::init(int id)
{
    if (id < 16)
    {
        c_ROW = tPos[id].t_row;
        c_COL = tPos[id].t_col;
        c_chessType = tPos[id].t_chessType;
        c_Red = false;
    }
    else
    {
        c_ROW = 9 - tPos[id - 16].t_row;
        c_COL = 8 - tPos[id - 16].t_col;
        c_chessType = tPos[id - 16].t_chessType;
        c_Red = true;
    }
    c_Dead = false;
}

QString ChessPieces::getName(bool isRedSide)
{
    if (isRedSide)
    {
        switch (c_chessType)
        {
        case PAWN:
            return "卒";
        case CANNONS:
            return "炮";
        case ROOKS:
            return "俥";
        case ELEPHANT:
            return "相";
        case GUARD:
            return "仕";
        case KNIGHTS:
            return "傌";
        case KING:
            return "帥";
        }
    }
    else
    {
        switch (c_chessType)
        {
        case PAWN:
            return "兵";
        case CANNONS:
            return "炮";
        case ROOKS:
            return "车";
        case ELEPHANT:
            return "象";
        case GUARD:
            return "士";
        case KNIGHTS:
            return "马";
        case KING:
            return "帅";
        }
    }
    return "ERROR";
}

// 获取棋子所在列 返回文本棋谱
QString ChessPieces::getColText(int col)
{
    QString colText;
    if (c_Red)
    {
        colText = colTextRed[col];
    }
    else
    {
        colText = colTextBlack[col];
    }
    return colText;
}

//// 获取棋子所在行 返回文本棋谱
QString ChessPieces::getRowText(int row)
{
    QString temp = "";
    if (c_ROW == row)
    {
        temp.append("平");
        return temp;
    }
    if (c_ROW > row)
    {
        if (c_Red)
        {
            temp.append("进");
        }
        else
        {
            temp.append("退");
        }
    }
    else
    {
        if (c_Red)
        {
            temp.append("退");
        }
        else
        {
            temp.append("进");
        }
    }
    return temp;
}

//获取棋子移动的格数 返回文本棋谱
QString ChessPieces::getMoveText(int rowFrow, int rowto)
{
    QString temp = "";
    if (c_Red)
    {
        temp.append(colTextRedTurn[abs(rowFrow - rowto) - 1]);
    }
    else
    {
        temp.append(colTextBlack[abs(rowFrow - rowto) - 1]);
    }
    return temp;
}
