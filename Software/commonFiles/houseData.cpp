#include "houseData.h"

houseData::houseData(QObject *parent) :
    QObject(parent),
    m_powerPlugs(m_powerPlugs |= houseData::Start)
{
}

void houseData::setIpAndPort(const QString& ip, int port)
{
    m_IP = ip;
    m_port= port;
    emit ipChanged();
    emit portChanged();
}

void houseData::setIp(const QString& ip)
{
    setIpAndPort(ip, m_port);
}

void houseData::setPort(int port)
{
    setIpAndPort(m_IP, port);
}
