#include "chessboard.h"
#include "ui_chessboard.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChessBoard)
{
    init();
    //计时器
    m_timer = new QTimer; //初始化
    m_timeRecord = new QTime(0, 0, 0);
    m_isStart = false;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    m_about = new AboutAuthor();
    this->setWindowIcon(QIcon(":/resources/images/chess.svg"));

    ui->setupUi(this);
}

ChessBoard::~ChessBoard()
{
    delete ui;
}

void ChessBoard::init()
{
    for (int i = 0; i < 32; i++)
    {
        c_allPawns[i].init(i);
    }
    repentanceStep.clear(); //重置步骤
    selectID = -1;
    checkID = -1;
    m_isRed = true;
    m_isOver = false;
    m_displaysSteps = true;
}

void ChessBoard::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton || m_isOver == true)
        return;

    QPoint pt = ev->pos(); //获取当前鼠标坐标
    pt = getRealPoint(pt); //转换实际像素坐标
    click(pt);
}

void ChessBoard::click(QPoint pt)
{
    int row, col;
    bool Clicked = isCheck(pt, row, col);

    if (!Clicked)
    {
        return;
    }
    int id = getStoneId(row, col);
    clickPieces(id, row, col);
}

void ChessBoard::clickPieces(int id, int row, int col)
{
    if (this->selectID == -1) //如果选中棋子之前未被操作
        trySelectStone(id);
    else
        tryMoveStone(id, row, col);
}

void ChessBoard::trySelectStone(int id)
{
    if (id == -1)
        return;

    if (!canSelect(id))
        return;

    selectID = id;
    update();
    c_ChessVoice.voiceSelect();
}

void ChessBoard::tryMoveStone(int killid, int row, int col)
{
    if (killid != -1 && sameColor(killid, selectID))
    {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(selectID, killid, row, col);
    if (ret)
    {
        doMoveStone(selectID, killid, row, col);
        selectID = -1;
        update();
    }
}

void ChessBoard::doMoveStone(int moveid, int killid, int row, int col)
{
    saveStep(moveid, killid, row, col, repentanceStep);

    killStone(killid);
    moveStone(moveid, row, col);
    whowin();

    if (killid == -1)
        c_ChessVoice.voiceMove();
    else
        c_ChessVoice.voiceEat();

    if (isGeneral())
        c_ChessVoice.voiceGeneral();
}

void ChessBoard::saveStep(int moveid, int killid, int row, int col, QVector<ChessStep *> &steps)
{
    ChessStep *step = new ChessStep;
    step->c_originalCol = c_allPawns[moveid].c_COL;
    step->c_targetCol = col;
    step->c_originalRow = c_allPawns[moveid].c_ROW;
    step->c_targetRow = row;
    step->c_chessmanMoveId = moveid;
    step->c_chessmanKillId = killid;

    steps.append(step);
    textStepRecord = textStep(moveid, row, col);
}

QString ChessBoard::textStep(int moveid, int row, int col)
{
    int originalRow = c_allPawns[moveid].c_ROW;
    int targetRow = row;
    int originalCol = c_allPawns[moveid].c_COL;
    int targetCol = col;

    QString temp = "";
    QString name = c_allPawns[moveid].getName(c_allPawns[moveid].c_Red);
    QString textCol = c_allPawns[moveid].getColText(originalCol);
    QString textRow = c_allPawns[moveid].getRowText(originalRow);
    temp.append(name).append(textCol).append(textRow);

    if (c_allPawns[moveid].c_chessType == 6 || c_allPawns[moveid].c_chessType == 5 || c_allPawns[moveid].c_chessType == 4 || c_allPawns[moveid].c_chessType == 0)
    {
        //行相等
        if (originalRow == targetRow)
        {
            temp.append(c_allPawns[moveid].getColText(targetCol));
            return temp;
        }
        //移动格数
        temp.append(c_allPawns[moveid].getMoveText(originalRow, targetRow));
    }
    else
    {
        temp.append(c_allPawns[moveid].getColText(targetCol));
    }
    return temp;
}

void ChessBoard::backOne()
{
    if (this->repentanceStep.size() == 0 || m_isOver)
        return;

    ChessStep *step = this->repentanceStep.last();
    repentanceStep.removeLast();
    back(step);
    update();
    delete step;
    c_ChessVoice.voiceBack();
}

void ChessBoard::back(ChessStep *step)
{
    rollbackStone(step->c_chessmanKillId);
    moveStone(step->c_chessmanMoveId, step->c_originalRow, step->c_originalCol);
}

void ChessBoard::back()
{
    backOne();
}

void ChessBoard::updateTime()
{
    *m_timeRecord = m_timeRecord->addSecs(1);
    ui->lcdNumber->display(m_timeRecord->toString("hh:mm:ss"));

    if (m_isStart == false)
        ui->Btn_start->setText("开始");
    else if (m_isStart == true)
        ui->Btn_start->setText("暂停");
}

bool ChessBoard::isRed(int id)
{
    return c_allPawns[id].c_Red;
}

bool ChessBoard::isDead(int id)
{
    if (id == -1)
        return true;

    return c_allPawns[id].c_Dead;
}

void ChessBoard::killStone(int id)
{
    if (id == 1)
    {
        return;
    }
    c_allPawns[id].c_Dead = true;
}

void ChessBoard::rollbackStone(int id)
{
    if (id == -1)
    {
        return;
    }
    c_allPawns[id].c_Dead = false;
}

void ChessBoard::moveStone(int moveid, int row, int col)
{
    c_allPawns[moveid].c_ROW = row;
    c_allPawns[moveid].c_COL = col;
    m_isRed = !m_isRed;
}

bool ChessBoard::sameColor(int moveid, int killId)
{
    if (moveid == -1 || killId == -1)
        return false;

    return isRed(moveid) == isRed(killId);
}

int ChessBoard::getStoneId(int row, int col)
{
    for (int i = 0; i < 32; i++)
    {
        if (c_allPawns[i].c_ROW == row && c_allPawns[i].c_COL == col && !isDead(i))
            return i;
    }
    return -1;
}

int ChessBoard::getStoneAliveCount(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if (row1 != row2 && col1 != col2)
    {
        return -1;
    }
    if (row1 == row2 && col1 == row2)
    {
        return -1;
    }
    if (row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for (int col = min + 1; col < max; col++)
        {
            if (getStoneId(row1, col) != -1)
            {
                ret++;
            }
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for (int row = min + 1; row < max; row++)
        {
            if (getStoneId(row, col1) != -1)
            {
                ret++;
            }
        }
    }
    return ret;
}

void ChessBoard::whowin()
{
    if (c_allPawns[4].c_Dead && !c_allPawns[20].c_Dead)
    {
        reset();
        victoryMessageBox("提示", "本局游戏结束，红方胜利！");
    }
    if (!c_allPawns[4].c_Dead && c_allPawns[20].c_Dead)
    {
        reset();
        victoryMessageBox("提示", "本局游戏结束，黑方胜利！");
    }
}

bool ChessBoard::isCheck(QPoint pt, int &row, int &col)
{
    for (row = 0; row <= 9; row++)
    {
        for (col = 0; col <= 8; col++)
        {
            QPoint temp = center(row, col);
            int x = temp.x() - pt.x();
            int y = temp.y() - pt.y();

            if (x * x + y * y < pawnRadius * pawnRadius)
                return true;
        }
    }
    return false;
}

int ChessBoard::relation(int row1, int col1, int row2, int col2)
{
    return abs(row1 - row2) * 10 + abs(col1 - col2);
}

QPoint ChessBoard::getRealPoint(QPoint pt)
{
    int side = qMin(int((ui->centralwidget->width() - ui->verticalWidget->width()) / 0.9), ui->label->height());
    QPoint ret;
    ret.setX(pt.x() / double(side) * 960.0);
    ret.setY(pt.y() / double(side) * 960.0);
    return ret;
}

bool ChessBoard::isGeneral()
{
    int generalID = 20; //当前回合方将军ID
    if (!m_isRed)
    {
        generalID = 4;
    }
    int row = c_allPawns[generalID].c_ROW; //当前回合方的将军row
    int col = c_allPawns[generalID].c_COL; //当前回合方的将军col

    for (int i = 0; i < 32; i++)
    {
        if (i >= 16 && m_isRed)
            break;
        if (canMove(i, generalID, row, col) && c_allPawns[i].c_Dead)
        {
            return true;
        }
    }
    return false;
}

// true 产生"对将" 情景了；false 无"对将"情况
bool ChessBoard::encounterGenerals()
{
    if (c_allPawns[4].c_Dead || c_allPawns[20].c_Dead)
        return false;

    int colBlack = c_allPawns[4].c_COL;
    int rowBlack = c_allPawns[4].c_ROW;
    int colRed = c_allPawns[20].c_COL;
    int rowRed = c_allPawns[20].c_ROW;

    bool bcolEmpty = true;
    if (colBlack == colRed)
    {
        for (int row = rowBlack + 1; row < rowRed; row++)
        {
            if (havePieces(row, colBlack))
                bcolEmpty = false; //将之间是有棋子的
        }
    }
    else
    {
        bcolEmpty = false;
    }
    return bcolEmpty;
}

// 判断某格子是否有棋子在其上
bool ChessBoard::havePieces(int row, int col)
{
    for (auto pieces : c_allPawns)
    {
        if (pieces.c_Dead)
            continue;

        if (pieces.c_ROW == row && pieces.c_COL == col)
            return true;
    }
    return false;
}

void ChessBoard::reset()
{
    c_ChessVoice.voiceWin();
    m_isOver = true;
    //游戏结束 则计时停止 & 计时控制按钮不再可用 直到用户重新游戏
    if (m_isStart)
    {
        m_timer->stop();
        m_isStart = false;
    }
    ui->Btn_start->setEnabled(false);
}

void ChessBoard::victoryMessageBox(QString title, QString msg)
{
    QMessageBox message(QMessageBox::Information, title, msg);
    message.setIconPixmap(QPixmap(":/resources/images/win.jpg"));
    message.setFont(QFont("FangSong", 16, QFont::Bold));
    message.exec();
}

QPoint ChessBoard::center(int row, int col)
{
    QPoint rePoint;
    rePoint.ry() = row * spacing + interfaceMargins;
    rePoint.rx() = col * spacing + interfaceMargins;
    return rePoint;
}

QPoint ChessBoard::center(int id)
{
    return center(c_allPawns[id].c_ROW, c_allPawns[id].c_COL);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(int((ui->centralwidget->width() - ui->verticalWidget->width()) / 0.9), ui->label->height());
    painter.scale(side / 960.0, side / 960.0);

    interfaceMargins = 60;    //界面边距
    spacing = 90;             //间距
    pawnRadius = spacing / 2; //棋子半径

    /*----- 绘画棋盘 -----*/
    //绘画10条横线
    for (int i = 0; i <= 9; i++)
    {
        painter.drawLine(QPoint(interfaceMargins, interfaceMargins + i * spacing), QPoint(interfaceMargins + 8 * spacing, interfaceMargins + i * spacing));
    }
    // 9条竖线
    for (int j = 0; j <= 8; j++)
    {
        if (j == 0 || j == 8)
        {
            painter.drawLine(QPoint(interfaceMargins + j * spacing, interfaceMargins), QPoint(interfaceMargins + j * spacing, interfaceMargins + 9 * spacing));
        }
        else
        {
            painter.drawLine(QPoint(interfaceMargins + j * spacing, interfaceMargins), QPoint(interfaceMargins + j * spacing, interfaceMargins + 4 * spacing));
            painter.drawLine(QPoint(interfaceMargins + j * spacing, interfaceMargins + 5 * spacing), QPoint(interfaceMargins + j * spacing, interfaceMargins + 9 * spacing));
        }
    }

    //画4条斜线
    painter.drawLine(QPoint(interfaceMargins + 3 * spacing, interfaceMargins), QPoint(interfaceMargins + 5 * spacing, interfaceMargins + 2 * spacing));
    painter.drawLine(QPoint(interfaceMargins + 3 * spacing, interfaceMargins + 2 * spacing), QPoint(interfaceMargins + 5 * spacing, interfaceMargins));
    painter.drawLine(QPoint(interfaceMargins + 3 * spacing, interfaceMargins + 7 * spacing), QPoint(interfaceMargins + 5 * spacing, interfaceMargins + 9 * spacing));
    painter.drawLine(QPoint(interfaceMargins + 3 * spacing, interfaceMargins + 9 * spacing), QPoint(interfaceMargins + 5 * spacing, interfaceMargins + 7 * spacing));

    QRect rect1(interfaceMargins + spacing, interfaceMargins + 4 * spacing, spacing, spacing);
    QRect rect2(interfaceMargins + 2 * spacing, interfaceMargins + 4 * spacing, spacing, spacing);
    QRect rect3(interfaceMargins + 5 * spacing, interfaceMargins + 4 * spacing, spacing, spacing);
    QRect rect4(interfaceMargins + 6 * spacing, interfaceMargins + 4 * spacing, spacing, spacing);

    painter.setFont(QFont("FangSong", spacing * 5 / 6, 800));
    painter.drawText(rect1, "楚", QTextOption(Qt::AlignCenter));
    painter.drawText(rect2, "河", QTextOption(Qt::AlignCenter));
    painter.drawText(rect3, "汉", QTextOption(Qt::AlignCenter));
    painter.drawText(rect4, "界", QTextOption(Qt::AlignCenter));

    /*----- 绘画棋子 -----*/
    //绘画上次移动棋子的起止位置
    if (m_displaysSteps)
    {
        drawLastStep(painter, repentanceStep);
    }
    for (int i = 0; i < 32; i++)
    {
        drawChessPieces(painter, i);
    }
    //绘制文本棋谱
    drawTextStep();
}

void ChessBoard::drawChessPieces(QPainter &painter, int id)
{
    if (c_allPawns[id].c_Dead)
        return;

    QPoint temp = center(id);
    QRect rect(temp.x() - pawnRadius, temp.y() - pawnRadius, spacing, spacing);

    if (selectID == id)
        painter.setBrush(QBrush(QColor(64, 64, 196, 80)));
    else
        painter.setBrush(QBrush(QColor(64, 64, 196, 10)));

    painter.setPen(QColor(0, 0, 0));
    painter.drawEllipse(center(id), pawnRadius, pawnRadius);
    painter.setFont(QFont("FangSong", pawnRadius * 5 / 6, 2700));

    if (id < 16)
        painter.setPen(QColor(0, 0, 0));
    else
        painter.setPen(QColor(255, 0, 0));

    painter.drawText(rect, c_allPawns[id].getName(c_allPawns[id].c_Red), QTextOption(Qt::AlignCenter)); //绘画圆形里面的汉字
}

void ChessBoard::drawLastStep(QPainter &painter, QVector<ChessStep *> &steps)
{
    if (this->repentanceStep.size() == 0)
        return;

    QPoint stepFrom = center(steps.last()->c_originalRow, steps.last()->c_originalCol);
    QRect rectFrom(stepFrom.x() - pawnRadius, stepFrom.y() - pawnRadius, spacing, spacing);
    painter.setBrush(QColor(0, 0, 0, 0.3 * 255));
    QPen pen(Qt::SolidLine);
    painter.setPen(Qt::black);
    painter.drawRect(rectFrom);

    QPoint stepTo = center(steps.last()->c_targetRow, steps.last()->c_targetCol);
    QRect rectTo(stepTo.x() - pawnRadius, stepTo.y() - pawnRadius, spacing, spacing);
    painter.setBrush(QColor(0, 0, 0, 0.2 * 255));
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(rectTo);
}

void ChessBoard::drawTextStep()
{
    ui->label_3->setText(textStepRecord);
}

bool ChessBoard::canMove(int moveId, int killId, int row, int col)
{
    //选择棋子ID和吃棋同色，则选择其他棋子返回
    if (sameColor((moveId), killId))
    {
        //换棋子
        selectID = killId;
        update();
        return false;
    }

    switch (c_allPawns[moveId].c_chessType)
    {
    case ChessPieces::PAWN:
        return canMovePawn(moveId, killId, row, col);

    case ChessPieces::CANNONS:
        return canMoveCannons(moveId, killId, row, col);

    case ChessPieces::ROOKS:
        return canMoveRooks(moveId, killId, row, col);

    case ChessPieces::ELEPHANT:
        return canMoveElephant(moveId, killId, row, col);

    case ChessPieces::GUARD:
        return canMoveGuard(moveId, killId, row, col);

    case ChessPieces::KNIGHTS:
        return canMoveKnights(moveId, killId, row, col);

    case ChessPieces::KING:
        return canMoveKing(moveId, killId, row, col);
    default:
        break;
    }
    return true;
}

bool ChessBoard::canSelect(int id)
{
    return m_isRed == c_allPawns[id].c_Red;
}

bool ChessBoard::canMovePawn(int moveId, int killId, int row, int col)
{
    Q_UNUSED(killId);
    int d = relation(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (d != 1 && d != 10)
        return false;

    if (isRed(moveId)) //红
    {
        //兵不可以后退
        if (row > c_allPawns[moveId].c_ROW)
            return false;

        //兵没过河不可以横着走
        if (c_allPawns[moveId].c_ROW >= 5 && c_allPawns[moveId].c_ROW == row)
            return false;

    } //黑色
    else
    {
        if (row < c_allPawns[moveId].c_ROW)
            return false;
        if (c_allPawns[moveId].c_ROW <= 4 && c_allPawns[moveId].c_ROW == row)
            return false;
    }
    return true;
}

bool ChessBoard::canMoveCannons(int moveId, int killId, int row, int col)
{
    int ret = getStoneAliveCount(row, col, c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL);
    if (killId != -1)
    {
        if (ret == 1)
            return true;
    }
    else
    {
        if (ret == 0)
            return true;
    }
    return false;
}

bool ChessBoard::canMoveRooks(int moveId, int killId, int row, int col)
{
    Q_UNUSED(killId);
    int ret = getStoneAliveCount(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (ret == 0)
        return true;

    return false;
}

bool ChessBoard::canMoveKnights(int moveId, int killId, int row, int col)
{
    Q_UNUSED(killId);
    int d = relation(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (d != 12 && d != 21)
        return false;

    //憋马脚
    if (d == 12)
    {
        if (getStoneId(c_allPawns[moveId].c_ROW, (c_allPawns[moveId].c_COL + col) / 2) != -1)
            return false;
    }
    else
    {
        if (getStoneId((c_allPawns[moveId].c_ROW + row) / 2, c_allPawns[moveId].c_COL) != -1)
            return false;
    }
    return true;
}

bool ChessBoard::canMoveElephant(int moveId, int killId, int row, int col)
{
    Q_UNUSED(killId);
    int d = relation(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (d != 22)
        return false;

    int row_eye = (c_allPawns[moveId].c_ROW + row) / 2;
    int col_eye = (c_allPawns[moveId].c_COL) / 2;

    //堵象眼
    if (getStoneId(row_eye, col_eye) != -1)
        return false;

    //象不过河
    if (isRed(moveId))
    {
        if (row < 4)
            return false;
    }
    else
    {
        if (row > 5)
            return false;
    }
    return true;
}

bool ChessBoard::canMoveGuard(int moveId, int killId, int row, int col)
{
    Q_UNUSED(killId);
    if (isRed(moveId))
    {
        if (row < 7 || col < 3 || col > 5)
            return false;
    }
    else
    {
        if (row > 2 || col < 3 || col > 5)
            return false;
    }

    int d = relation(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (d == 11)
        return true;

    return false;
}

bool ChessBoard::canMoveKing(int moveId, int killId, int row, int col)
{
    if(killId != -1 && c_allPawns[killId].c_chessType == ChessPieces::KING)
    {
        if(encounterGenerals())
        {
            victoryMessageBox("提示","对将,重新开始");
            init();
        }

    }


    if (isRed(moveId))
    {
        if (row < 7 || col < 3 || col > 5)
            return false;
    }
    else
    {
        if (row > 2 || col < 3 || col > 5)
            return false;
    }
    int d = relation(c_allPawns[moveId].c_ROW, c_allPawns[moveId].c_COL, row, col);
    if (d == 1 || d == 10)
        return true;

    return false;
}

void ChessBoard::on_Btn_start_clicked()
{
    if (!m_isStart)
    {
        m_timer->start(1000);
        ui->Btn_start->setText("暂停");
    }
    else
    {
        m_timer->stop();
        ui->Btn_start->setText("继续");
    }
    m_isStart = !m_isStart;
}

void ChessBoard::on_Btn_reset_clicked()
{
    m_timer->stop();
    m_timeRecord->setHMS(0, 0, 0);
    ui->lcdNumber->display(m_timeRecord->toString("hh:mm:ss"));
    m_isStart = false;
    ui->Btn_start->setText("开始");
    ui->Btn_start->setEnabled(true);
}

void ChessBoard::on_Btn_restart_clicked()
{
    init();
    on_Btn_reset_clicked();
    update();
}

void ChessBoard::on_Btn_back_clicked()
{
    back();
    update();
}

void ChessBoard::on_Btn_showstep_clicked()
{
    m_displaysSteps = !m_displaysSteps;
    update();
}

void ChessBoard::on_Btn_showmenu_clicked()
{
    emit this->toMenu();
}

void ChessBoard::on_Btn_about_clicked()
{
    m_about->setWindowTitle("关于作者");
    m_about->show();
}
