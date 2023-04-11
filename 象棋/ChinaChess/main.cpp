#include "chessboard.h"
#include "chessmenumain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //    ChessBoard w;

    ChessMenuMain w;
    w.show();
    return a.exec();
}
