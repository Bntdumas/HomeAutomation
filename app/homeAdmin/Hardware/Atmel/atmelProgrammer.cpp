#include "atmelProgrammer.h"
#include <QProcess>
#include <QDebug>
atmelProgrammer::atmelProgrammer(const QDir &workingDirectory, QObject *parent) :
    adminToolItem(parent),
    m_workingDirectory(workingDirectory)
{
}

void atmelProgrammer::compileSketch()
{
    //  ino build -d /home/benoit/Downloads/arduino-1.0.5

    const QString arduinoIDEPath(qgetenv("ARDUINOPATH").constData());
    if (arduinoIDEPath.isEmpty()) {
        Q_EMIT message(tr("The arduino IDE path is missing. Please set ARDUINOPATH"), SoftwareError);
    }

    const QString program = "ino";
    QStringList arguments;
    arguments << "build" //sub module of ino for compilation
                     << "-d" << arduinoIDEPath; //Path for arduino IDE

    QProcess *compilationProcess = new QProcess(this);
    compilationProcess->setWorkingDirectory(m_workingDirectory.absolutePath());
    Q_EMIT message(tr("Compiling Arduino sketch in %1 ").arg(m_workingDirectory.absolutePath()), Info);
    Q_EMIT message(tr("$ino build -d %1").arg(arduinoIDEPath), Info);
    compilationProcess->start(program, arguments);
    connect (compilationProcess, SIGNAL(finished(int)), compilationProcess, SLOT(deleteLater()));
    connect (compilationProcess, SIGNAL(finished(int)), this, SLOT(processEnd()));
    connect (compilationProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processInfoDataAvailable()));
    connect (compilationProcess, SIGNAL(readyReadStandardError()), this, SLOT(processErrorDataAvailable()));
}

void atmelProgrammer::uploadSketch(const QString &programmer)
{
    //sudo avrdude -p m328p -c dragon_isp -P usb -e -U flash:w:firmware.hex

    // we need to go where the file was compiled.
    const QString buildTopDir = m_workingDirectory.absolutePath() + "/.build/";
    const QDir buildWorkingDirectory(buildTopDir);
    const QDir hexFileDirectory(buildTopDir + buildWorkingDirectory.entryList(QStringList() << "uno-*", QDir::Dirs).first());

    const QString program = "avrdude";
    QStringList arguments;
    arguments << "-pm328p" // chip, an ATMEGA328p
                     << QString("-c%1").arg(programmer) // programmer (I use dragon_isp)
                     << "-Pusb" // port
                     << "-e" // Erase chip
                     << "-Uflash:w:firmware.hex"; // file to write
    QProcess *uploadProcess = new QProcess(this);
    uploadProcess->setWorkingDirectory(hexFileDirectory.absolutePath());
    Q_EMIT message(tr("Uploading hex file from %1 ").arg(hexFileDirectory.absolutePath()), Info);
    Q_EMIT message(tr("$sudo avrdude -p m328p -c %1 -P usb -e -U flash:w:firmware.hex").arg(programmer), Info);
    uploadProcess->start(program, arguments);

    connect (uploadProcess, SIGNAL(finished(int)), uploadProcess, SLOT(deleteLater()));
    connect (uploadProcess, SIGNAL(finished(int)), this, SLOT(processEnd()));
    connect (uploadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processInfoDataAvailable()));
    connect (uploadProcess, SIGNAL(readyReadStandardError()), this, SLOT(processErrorDataAvailable()));
}

void atmelProgrammer::processInfoDataAvailable()
{
    if (QProcess *senderProcess = qobject_cast<QProcess*>(sender())) {
        Q_EMIT message(senderProcess->readAllStandardOutput(), Info);
    }
}

void atmelProgrammer::processErrorDataAvailable()
{
    if (QProcess *senderProcess = qobject_cast<QProcess*>(sender())) {
        Q_EMIT message(senderProcess->readAllStandardError(), SoftwareError);
    }
}

void atmelProgrammer::processEnd()
{
    if (QProcess *senderProcess = qobject_cast<QProcess*>(sender())) {
        Q_EMIT message(tr("Process '%1'' terminated.").arg(senderProcess->program()), Success);
    }
}

