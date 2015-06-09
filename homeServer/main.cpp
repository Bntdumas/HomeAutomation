#include <QCoreApplication>
#include "homeNetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    homeNetwork net;
    return a.exec();
}
