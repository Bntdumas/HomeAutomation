#include "adminWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    adminWindow w;
    w.show();
    
    return a.exec();
}
