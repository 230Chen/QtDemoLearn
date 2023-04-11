#ifndef CHESSVOICE_H
#define CHESSVOICE_H

#include <QObject>
#include <QSoundEffect>

class ChessVoice : public QObject
{
    Q_OBJECT
public:
    explicit ChessVoice(QObject *parent = nullptr);
    ~ChessVoice();

    QSoundEffect *win = nullptr;
    QSoundEffect *select = nullptr;
    QSoundEffect *move = nullptr;
    QSoundEffect *eat = nullptr;
    QSoundEffect *back = nullptr;
    QSoundEffect *isgeneral = nullptr;

    void voiceWin();     //胜利音效
    void voiceSelect();  //选棋音效
    void voiceMove();    //移动音效
    void voiceEat();     //吃子音效
    void voiceBack();    //悔棋音效
    void voiceGeneral(); //将军音效
};

#endif // CHESSVOICE_H
