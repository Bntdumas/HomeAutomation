#ifndef HOUSEDATA_H
#define HOUSEDATA_H

#include <QObject>

/**
 * @brief Holds and allow manipulation of the house data. Recieves information from the network and from user input.
 */

class houseData : public QObject
{
    Q_OBJECT
    /**
      * @brief properties for QML communication.
      */
    Q_PROPERTY(bool lightningDesk READ lightningDesk WRITE requestUpdateLightningDesk NOTIFY lightningDeskChanged)
    Q_PROPERTY(bool lightningBed READ lightningBed WRITE requestUpdateLightningBed NOTIFY lightningBedChanged)
    Q_PROPERTY(bool lightningTable READ lightningTable WRITE requestUpdateLightningTable NOTIFY lightningTableChanged)
    Q_PROPERTY(bool lightningWorkshop READ lightningWorkshop WRITE requestUpdateLightningWorkshop NOTIFY lightningWorkshopChanged)
    Q_PROPERTY(bool powerScreen READ powerScreen WRITE requestUpdatePowerScreen NOTIFY powerScreenChanged)
    Q_PROPERTY(bool powerSpeakers READ powerSpeakers WRITE requestUpdatePowerSpeakers NOTIFY powerSpeakersChanged)

    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit houseData(QObject *parent = 0);

    /**
     * @brief Describe the different state of the house plugs, as flags.
     */
    enum PlugFlag {
        Start = 1
        ,Bed = 2
        ,Table = 4
        ,Workshop = 8
        ,Screen = 16
        ,Speakers = 32
        ,Desk = 64
    };
    Q_DECLARE_FLAGS(PlugFlags, PlugFlag)

    /**
     * @brief set the entire house ligthning state using the @ref PlugFlag QFlag
     * @param A PlugFlag QFlag
     */
    void setPowerPlugs(int lights);

    /**
     * @brief set individual states of the house plugs.
     * @param the boolean state. (true: on, false: off)
     */
    void setLightningDesk(bool state);
    void setLightningBed(bool state);
    void setLightningTable(bool state);
    void setLightningWorkshop(bool state);
    void setPowerScreen(bool state);
    void setPowerSpeakers(bool state);

   /**
      * @brief saves the server IP and Port.
      * @note  If the socket exists, it is resetted with the new IP/PORT
      **/
    Q_INVOKABLE void setIpAndPort(const QString& ip, int port);

    /**
     * @brief allow individual changes of IP/Port.
     * @note internally calls setIpAndPort()
     */
    void setIp( const QString& );
    void setPort( int );

    /**
     * @brief getters
     */
   PlugFlags powerPlugs() const {return m_powerPlugs;}
    bool lightningDesk() const {return m_powerPlugs & Desk;}
    bool lightningBed() const {return m_powerPlugs & Bed;}
    bool lightningTable() const {return m_powerPlugs & Table;}
    bool lightningWorkshop() const {return m_powerPlugs & Workshop;}
    bool powerScreen() const {return m_powerPlugs & Screen;}
    bool powerSpeakers() const {return m_powerPlugs & Speakers;}

    QString ip() const { return m_IP; }
    int port() const { return m_port; }

Q_SIGNALS:
    /**
     * @brief signals related to defined Q_PROPERTIES, they are used by QtQuick
     */
    void lightningDeskChanged();
    void lightningBedChanged();
    void lightningTableChanged();
    void lightningWorkshopChanged();
    void powerScreenChanged();
    void powerSpeakersChanged();

    void ipChanged();
    void portChanged();

    /**
     * @brief emitted when the different parts changed (more generic than the above signals).
     */
    void powerPlugsChanged();

    /**
     * @brief emits change requests with the new values.
     */
    void requestUpdatePowerPlugsData(houseData::PlugFlags lightning);
    
public Q_SLOTS:
    /**
     * @brief Request a change in the house.
     */
    void requestUpdatePowerPlugs(houseData::PlugFlags lights) ;
    void requestUpdatePowerPlugs(houseData::PlugFlag plug, bool state);

    void requestUpdateLightningDesk(bool state);
    void requestUpdateLightningBed(bool state);
    void requestUpdateLightningTable(bool state);
    void requestUpdateLightningWorkshop(bool state);
    void requestUpdatePowerScreen(bool state);
    void requestUpdatePowerSpeakers(bool state);

private:

    QString m_IP;
    int m_port;

   PlugFlags m_powerPlugs;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(houseData::PlugFlags)
#endif // HOUSEDATA_H
