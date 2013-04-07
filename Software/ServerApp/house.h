#ifndef HOUSE_H
#define HOUSE_H

#include <QObject>

class tcpServer;
class serverLocalClient;
class lightningController;
class mediaPlayer;
class sensors;
class houseData;

class QTcpSocket;
class QTimer;

/**
 * @brief Handle the real house
 */
class House : public QObject
{
    Q_OBJECT
    
public:
    explicit House(QObject *parent = 0);
    ~House();

private:
        /**
       * @brief initialize the house
       */
      void init();
      lightningController *m_houseLights;
      mediaPlayer *m_player;
      sensors *m_houseSensors;

      houseData *m_houseData;

      tcpServer *m_server;
       serverLocalClient *m_serverLocalClient;
};

#endif // HOUSE_H
