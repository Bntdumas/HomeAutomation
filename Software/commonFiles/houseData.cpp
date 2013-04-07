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

void houseData::setPowerPlugs(int flags)
{
    PlugFlags plugs = static_cast<PlugFlags>(flags);
    if (m_powerPlugs == plugs) {
        return;
    }
    m_powerPlugs = plugs;
    emit lightningDeskChanged();
    emit lightningBedChanged();
    emit lightningTableChanged();
    emit lightningWorkshopChanged();
    emit powerScreenChanged();
    emit powerSpeakersChanged();
}

void houseData::setPowerSpeakers(bool state)
{
    if (state) {
        m_powerPlugs |= Speakers;
    }
    else {
        m_powerPlugs &= ~Speakers;
    }
    emit powerPlugsChanged();
    emit powerSpeakersChanged();
}

void houseData::setLightningBed(bool state)
{
    if (state) {
        m_powerPlugs |= Bed;
    }
    else {
        m_powerPlugs &= ~Bed;
    }
    emit powerPlugsChanged();
    emit lightningBedChanged();
}

void houseData::setLightningDesk(bool state)
{
    if (state) {
        m_powerPlugs |= Desk;
    }
    else {
        m_powerPlugs &= ~Desk;
    }
    emit powerPlugsChanged();
    emit lightningDeskChanged();
}

void houseData::setLightningTable(bool state)
{
    if (state) {
        m_powerPlugs |= Table;
    }
    else {
        m_powerPlugs &= ~Table;
    }
    emit powerPlugsChanged();
    emit lightningTableChanged();
}

void houseData::setLightningWorkshop(bool state)
{
    if (state) {
        m_powerPlugs |= Workshop;
    }
    else {
        m_powerPlugs &=~Workshop;
    }
    emit powerPlugsChanged();
    emit lightningWorkshopChanged();
}

void houseData::setPowerScreen(bool state)
{
    if (state) {
        m_powerPlugs |= Screen;
    }
    else {
        m_powerPlugs &=~Screen;
    }
    emit powerPlugsChanged();
    emit powerScreenChanged();
}
void houseData::requestUpdatePowerPlugs(houseData::PlugFlags plugs)
{
    emit requestUpdatePowerPlugs(plugs);
}

void houseData::requestUpdatePowerPlugs(PlugFlag plug, bool state)
{
    PlugFlags currentPlugs = m_powerPlugs;
    if (state) {
        currentPlugs |= plug;
    }
    else {
        currentPlugs &=~plug;
    }
    emit requestUpdatePowerPlugs(currentPlugs);
}

void houseData::requestUpdateLightningBed(bool state)
{
    requestUpdatePowerPlugs(Bed, state);
}

void houseData::requestUpdateLightningDesk(bool state)
{
    requestUpdatePowerPlugs(Desk, state);
}

void houseData::requestUpdateLightningTable(bool state)
{
    requestUpdatePowerPlugs(Table, state);
}

void houseData::requestUpdateLightningWorkshop(bool state)
{
    requestUpdatePowerPlugs(Workshop, state);
}

void houseData::requestUpdatePowerScreen(bool state)
{
    requestUpdatePowerPlugs(Screen, state);
}

void houseData::requestUpdatePowerSpeakers(bool state)
{
    requestUpdatePowerPlugs(Speakers, state);
}
