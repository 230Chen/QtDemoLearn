#ifndef CHESSSTEP_H
#define CHESSSTEP_H

#include <QObject>

class ChessStep : public QObject
{
    Q_OBJECT
public:
    explicit ChessStep(QObject *parent = nullptr);
    ~ChessStep();

    int c_chessmanMoveId; //移动棋子ID
    int c_chessmanKillId; //击杀棋子ID
    int c_originalRow;    //原棋子位置行
    int c_originalCol;    //原棋子位置列
    int c_targetRow;      //目标位置行
    int c_targetCol;      //目标位置列

signals:
};

#endif // CHESSSTEP_H
