#include <QCoreApplication>
#include "QRF24NetworkLib
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "starting radio module...";
    QHFDevice device;
    return a.exec();
}
