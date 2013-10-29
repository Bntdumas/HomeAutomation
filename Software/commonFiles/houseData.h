#ifndef HOUSEDATA_H
#define HOUSEDATA_H

#include <QObject>

/**
 * @brief Holds and allow manipulation of the house data. Recieves information from the network and from user input.
 */

class houseData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit houseData(QObject *parent = 0);   


    /**
     * @brief Defines a Power Plug
     */
    struct powerPlug {
        QUuid ID;
        QString readableName;
        bool state;
    };

    /**
     * @brief set individual states of the house plugs.
     * @param the boolean state. (true: on, false: off)
     */
    void setPowerPlug(powerPlug plug);

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
    QString ip() const { return m_IP; }
    int port() const { return m_port; }

Q_SIGNALS:
    /**
     * @brief signals related to defined Q_PROPERTIES, they are used by QtQuick
     */
    void powerPlugChanged(PlugFlag plug);

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

private:

    QString m_IP;
    int m_port;

   PlugFlags m_powerPlugs;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(houseData::PlugFlags)
#endif // HOUSEDATA_H
