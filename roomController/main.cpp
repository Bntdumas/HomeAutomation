#include <QCoreApplication>
#include "QRF24Network.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "starting radio module...";
    QRF24Network device();
    return a.exec();
}
