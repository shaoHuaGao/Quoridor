#include "widget.h"
#include <QApplication>
#include "MCTS.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MCTS w;
    w.show();

    return a.exec();
}
