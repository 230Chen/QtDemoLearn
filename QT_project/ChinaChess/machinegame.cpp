
#include "machinegame.h"

MachineGame::MachineGame()
{
}
MachineGame::~MachineGame()
{
}
void MachineGame::chooseOrMovePieces(int tempID, int &row, int &col)
{
    if (selectID == -1)
    {
        if (checkID != -1)
        {
            if(c_allPawns[checkID].c_Red)
                selectID = tempID;
            else
            {
                selectID = -1;
                return;
            }
        }

    }
    else
    {
        if (canMove(selectID, checkID, row, col))
        {
            // selectid 为第一次点击选中的棋子
            // checkid 是第二次点击 击杀的棋子id
            c_allPawns[selectID].c_ROW = row;
            c_allPawns[selectID].c_COL = col;

            if (checkID != -1)
                c_allPawns[checkID].c_Dead = true;

            selectID = -1;
            m_isRed = !m_isRed;
        }
    }

    whowin();
    update();
}

void MachineGame::saveStep(int selectid, int checkedID, int row, int col, QVector<ChessStep *> &steps)
{
    ChessStep *step = new ChessStep;
    step->c_originalRow = c_allPawns[selectid].c_ROW;
    step->c_originalCol = c_allPawns[selectid].c_COL;
    step->c_targetRow = row;
    step->c_targetCol = col;
    step->c_chessmanMoveId = selectid;
    step->c_chessmanKillId = checkedID;

    steps.append(step);
}

void MachineGame::getAllPossibleMoveStep(QVector<ChessStep *> &steps)
{
    for (int id = 0; id < 16; id++)
    {
        if (c_allPawns[id].c_Dead)
            continue;

        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                int i = 16;
                for (; i < 32; i++)
                {
                    if (c_allPawns[i].c_ROW == row && c_allPawns[i].c_COL == col && c_allPawns[i].c_Dead == false)
                        break;
                }
                if (i != 32)
                {
                    if (canMove(id, i, row, col))
                        saveStep(id, i, row, col, steps);
                }
            }
        }
    }
}

void MachineGame::getAllPossibleMoveStepAndNoKill(QVector<ChessStep *> &steps)
{
    for (int id = 0; id < 16; id++)
    {
        if (c_allPawns[id].c_Dead)
            continue;

        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                int i = 0;
                for (; i <= 31; i++)
                {
                    if (c_allPawns[i].c_ROW == row && c_allPawns[i].c_COL == col && c_allPawns[i].c_Dead == false)
                        break;
                }

                if (id < 16 && i == 32)
                {
                    if (canMove(id, -1, row, col))
                        saveStep(id, -1, row, col, steps);
                }
            }
        }
    }
}

void MachineGame::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton)
        return;

    int row, col;
    if (!isCheck(getRealPoint(ev->pos()), row, col))
        return;

    checkID = -1;

    int i = 0;
    for (; i < 32; i++)
    {
        if (c_allPawns[i].c_ROW == row && c_allPawns[i].c_COL == col && c_allPawns[i].c_Dead == false)
            break;
    }

    if (0 <= i && i < 32)
        checkID = i;

    clickPieces(checkID, row, col);

    if (m_isRed)
    {
        chooseOrMovePieces(i, row, col);
        if (!m_isRed)
        {
            machineChooseAndMovePieces();
        }
    }
}

void MachineGame::clickPieces(int checkedID, int &row, int &col)
{
    if (m_isRed)
    {
        chooseOrMovePieces(checkedID, row, col);

        if (!m_isRed)
        {
            machineChooseAndMovePieces();
        }
    }
}

//假装移动棋子
void MachineGame::fakeMove(ChessStep *step)
{
    if (step->c_chessmanKillId != -1)
        c_allPawns[step->c_chessmanKillId].c_Dead = true;

    c_allPawns[step->c_chessmanMoveId].c_ROW = step->c_targetRow;
    c_allPawns[step->c_chessmanMoveId].c_COL = step->c_targetCol;
    m_isRed = !m_isRed;
}

void MachineGame::unFakeMove(ChessStep *step)
{
    if (step->c_chessmanKillId != -1)
        c_allPawns[step->c_chessmanKillId].c_Dead = false;

    c_allPawns[step->c_chessmanMoveId].c_ROW = step->c_originalRow;
    c_allPawns[step->c_chessmanMoveId].c_COL = step->c_originalCol;
    m_isRed = !m_isRed;
}

int MachineGame::calcScore()
{
    int redPiecesScores = 0;
    int blackPiecesScores = 0;

    /*
        PAWN,
        CANNONS,    炮
        ROOKS,      车
        ELEPHANT,   象
        GUARD,      仕
        KNIGHTS,    马
        KING        将
    */

    static int chessScore[] = {10, 50, 60, 30, 20, 40, 100};

    for (int i = 0; i < 16; i++)
    {
        if (c_allPawns[i].c_Dead)
            continue;
        blackPiecesScores += chessScore[c_allPawns[i].c_chessType];
    }

    for (int j = 16; j < 32; j++)
    {
        if (c_allPawns[j].c_Dead)
            continue;
        redPiecesScores += chessScore[c_allPawns[j].c_chessType];
    }
    return (blackPiecesScores - redPiecesScores);
}

//获得最好的移动步骤
ChessStep *MachineGame::getBestMove()
{

    int maxScore = -10000;
    ChessStep *retStep = NULL;
    // 1.检查所有可以走的步骤
    QVector<ChessStep *> steps;
    getAllPossibleMoveStep(steps);
    //如果没有可以击杀的红棋子，就走最后一步
    QVector<ChessStep *> stepsAndNoKill;
    getAllPossibleMoveStepAndNoKill(stepsAndNoKill);

    // 2.尝试开走
    for (QVector<ChessStep *>::Iterator it = steps.begin(); it != steps.end(); it++)
    {
        ChessStep *step = *it;
        fakeMove(step);
        int score = calcScore();
        unFakeMove(step);

        if (score > maxScore)
        {
            maxScore = score;
            retStep = step;
        }
    }

    if (retStep != NULL)
        return retStep;

    // 3./从这种不击杀红棋子，只是单纯移动黑棋steps里面，随机抽选一种进行下棋
    int stepsCount = stepsAndNoKill.count();

    srand(QTime(0, 0, 0).secsTo(QTime::currentTime())); //随机数种子MAX
    int temp = rand() % stepsCount;
    QVector<ChessStep *>::Iterator it = stepsAndNoKill.begin();
    retStep = it[temp];

    if (retStep == NULL)
        whowin();

    return retStep;
}

void MachineGame::machineChooseAndMovePieces()
{
    ChessStep *step = getBestMove();

    if (step->c_chessmanKillId == -1)
    {
        c_allPawns[step->c_chessmanMoveId].c_ROW = step->c_targetRow;
        c_allPawns[step->c_chessmanMoveId].c_COL = step->c_targetCol;
    }
    else
    {
        c_allPawns[step->c_chessmanKillId].c_Dead = true;
        c_allPawns[step->c_chessmanMoveId].c_ROW = c_allPawns[step->c_chessmanKillId].c_ROW;
        c_allPawns[step->c_chessmanMoveId].c_COL = c_allPawns[step->c_chessmanKillId].c_COL;
        selectID = -1;
    }
    m_isRed = !m_isRed;
}
