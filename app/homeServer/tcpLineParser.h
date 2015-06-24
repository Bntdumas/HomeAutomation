#ifndef TCPLINEPARSER_H
#define TCPLINEPARSER_H

#include <QObject>

class tcpLineParser : public QObject
{
    Q_OBJECT
public:
    explicit tcpLineParser(QObject *parent = 0);

};

#endif // TCPLINEPARSER_H
