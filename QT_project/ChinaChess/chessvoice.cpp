#include "chessvoice.h"

ChessVoice::ChessVoice(QObject *parent)
    : QObject{parent}
{
}

ChessVoice::~ChessVoice()
{
}

void ChessVoice::voiceWin()
{
    if (win != nullptr)
    {
        delete win;
        win = nullptr;
    }
    win = new QSoundEffect;
    win->setSource(QUrl::fromLocalFile(":/resources/sound/WinSound.wav"));
    win->play();
}

void ChessVoice::voiceSelect()
{
    if (select != nullptr)
    {
        delete select;
        select = nullptr;
    }
    select = new QSoundEffect;
    select->setSource(QUrl::fromLocalFile(":/resources/sound/selectChess.wav"));
    select->play();
}

void ChessVoice::voiceMove()
{
    if (move != nullptr)
    {
        delete move;
        move = nullptr;
    }
    move = new QSoundEffect;
    move->setSource(QUrl::fromLocalFile(":/resources/sound/moveChess.wav"));
    move->play();
}

void ChessVoice::voiceEat()
{
    if (eat != nullptr)
    {
        delete eat;
        eat = nullptr;
    }
    eat = new QSoundEffect;
    eat->setSource(QUrl::fromLocalFile(":/resources/sound/eatChess.wav"));
    eat->play();
}

void ChessVoice::voiceBack()
{
    if (back != nullptr)
    {
        delete back;
        back = nullptr;
    }
    back = new QSoundEffect;
    back->setSource(QUrl::fromLocalFile(":/resources/sound/backChess.wav"));
    back->play();
}

void ChessVoice::voiceGeneral()
{
    if (isgeneral != nullptr)
    {
        delete isgeneral;
        isgeneral = nullptr;
    }
    isgeneral = new QSoundEffect;
    isgeneral->setSource(QUrl::fromLocalFile(":/resources/sound/generalSound.wav"));
    isgeneral->play();
}
