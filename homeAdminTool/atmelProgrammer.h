#ifndef ATMELPROGRAMMER_H
#define ATMELPROGRAMMER_H

#include "adminToolItem.h"
#include <QDir>

/**
 * @brief The atmelProgrammer class compiles an arduino file and upload it to the atmel board via avr-dragon ISP
 * @note the compilation uses "ino": http://inotool.org/, please install it.
 */

class atmelProgrammer : public adminToolItem
{
    Q_OBJECT
public:
    explicit atmelProgrammer(const QDir &workingDirectory, QObject *parent = 0);

Q_SIGNALS:

public Q_SLOTS:
    /**
     * @brief Compiles the sketch in m_workingDirectory
     */
    void compileSketch();

    /**
     * @brief Uploads the sketch to the board
     */
    void uploadSketch();

private Q_SLOTS:
    /**
     * @brief reached wether a QProcess have some new data
     */
    void processInfoDataAvailable();
    void processErrorDataAvailable();

    void processEnd();

private:
    QDir m_workingDirectory;
};

#endif // ATMELPROGRAMMER_H
