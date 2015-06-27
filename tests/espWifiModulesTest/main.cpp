#include "espWifiModulesTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    espWifiModulesTest w;
    w.show();
    
    return a.exec();
}
