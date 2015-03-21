#include "moduleWizardPages.h"

#include "atmelModuleConfigurator.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QStringListModel>
#include <QTextStream>
#include <QToolButton>
#include <QVariant>
#include <QVBoxLayout>

#include <QDebug>

/// configureModulePage
configureModulePage::configureModulePage(QMap<QString, QString> deviceTypes, QWidget *parent) :
    QWizardPage(parent), m_configurator(new atmelModuleConfigurator(deviceTypes))
{
    setTitle(tr("Customize your module"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_configurator);
    setLayout(layout);

    for ( int i =1;  i<9 ; ++i)
        registerField(qPrintable(QString("GPIO%1").arg(i)), m_configurator, qPrintable(QString("GPIO%1").arg(i)));
    registerField("allGPIO", m_configurator, "allGPIO");
    registerField("LEDsEnabled", m_configurator, "LEDsEnabled");
    registerField("tempSensorEnabled", m_configurator, "tempSensorEnabled");
    registerField("lightSensorEnabled", m_configurator, "lightSensorEnabled");
    registerField("readWriteCycle", m_configurator, "readWriteCycle");
    registerField("moduleName", m_configurator, "moduleName");
}

void configureModulePage::initializePage()
{
    // TODO load previous settings
}

bool configureModulePage::validatePage()
{
    return m_configurator->validate();
}

/// setModuleSourceDirectoryPage
setModuleSourceDirectoryPage::setModuleSourceDirectoryPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Select the directory where the config file will be created"));

    m_leModuleSourcePath = new QLineEdit();
    m_sourceFilePath = QDir::currentPath();
    updateDirectory();

    QToolButton *btOpenDir = new QToolButton(this);
    btOpenDir->setText("...");
    connect(btOpenDir, SIGNAL(clicked()), this, SLOT(selectDirectory()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_leModuleSourcePath);
    layout->addWidget(btOpenDir);
    setLayout(layout);

    registerField("moduleSourcePath", m_leModuleSourcePath, "text");
}

void setModuleSourceDirectoryPage::initializePage()
{
    updateDirectory();
}

void setModuleSourceDirectoryPage::selectDirectory()
{
    m_sourceFilePath = QFileDialog::getExistingDirectory(0, tr("Select directory for the config file"), QDir::currentPath(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    updateDirectory();
}

void setModuleSourceDirectoryPage::updateDirectory()
{
    QString fileName(field("moduleName").toString());
    m_leModuleSourcePath->setText(m_sourceFilePath + '/' + fileName + ".ini");
}

/// selectProgrammerPage
selectProgrammerPage::selectProgrammerPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Select the programmer connected the chip module"));

    QFile file(":/AVR/otherFiles/AVRProgrammers");
    QStringListModel *mdl = new QStringListModel(this);
    QStringList programmers;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line(file.readLine());
            programmers.append(line.remove('\n'));
        }
    }
    mdl->setStringList(programmers);
    m_comboBox = new QComboBox();
    m_comboBox->setModel(mdl);
    m_comboBox->setCurrentText("dragon_isp");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_comboBox);
    setLayout(layout);

    registerField("programmer", m_comboBox, "currentText");
}

void selectProgrammerPage::initializePage()
{
}

/// reviewInfosPage
reviewInfosPage::reviewInfosPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Review the informations you entered"));
    m_label = new QLabel();
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
    setLayout(layout);
}

void reviewInfosPage::initializePage()
{
    // Gather module info
    QString moduleInfo;
    QTextStream infoStream(&moduleInfo);
    infoStream << " Module configuration" << endl
               << "Name: " << field("moduleName").toString()<< endl;

    Q_FOREACH( const atmelModuleConfigurator::GPIOPin currentPin,  field("allGPIO").value<  QList<atmelModuleConfigurator::GPIOPin> >()) {
        infoStream << "GPIO " << currentPin.index << ": "  <<  currentPin.deviceType
                   << " (" << currentPin.pinDirection << "). name: " << currentPin.name << endl;
    }
    infoStream << "LEDS are " << (field("LEDsEnabled").toBool()? "enabled.":"disabled.") << endl;
    infoStream << "Temperature sensor is " << (field("tempSensorEnabled").toBool()? "enabled.":"disabled.") << endl;
    infoStream << "Light sensor is " << (field("lightSensorEnabled").toBool()? "enabled.":"disabled.") << endl;
    infoStream << "Read/Write cycle is " << field("readWriteCycle").toInt() << " ms." << endl;

    // Path for source file
    infoStream << "Source file is saved as: " << field("moduleSourcePath").toString() << endl;

    // Programmer
    infoStream << "Programmer is: " << field("programmer").toString() << endl;

    m_label->setText(moduleInfo);
}

bool reviewInfosPage::validatePage()
{
    QSettings settings(field("moduleSourcePath").toString(),  QSettings::IniFormat);
    settings.setValue("board/name", field("moduleName").toString());

    QMapIterator<int, QPair<QString, QString> > mapIterator(field("allGPIO").value< QMap<int, QPair<QString, QString> > >());
    while (mapIterator.hasNext()) {
        mapIterator.next();
        QPair<QString, QString> currentGPIOState = mapIterator.value();
        settings.setValue(QString("board/GPIO%1/device").arg(mapIterator.key()), currentGPIOState.first);
        settings.setValue(QString("board/GPIO%1/deviceType").arg(mapIterator.key()), currentGPIOState.second );
    }
    settings.setValue("board/LEDS", field("LEDsEnabled").toBool());
    settings.setValue("board/temperatureSensor", field("tempSensorEnabled").toBool());
    settings.setValue("board/lightSensor", field("lightSensorEnabled").toBool());

    settings.setValue("board/readWriteCycle", field("readWriteCycle").toInt());
    settings.setValue("board/programmer", field("programmer").toString());

    settings.sync();

    return (settings.status() == QSettings::NoError );
}


donePage::donePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Done!"));
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setText("Compile and upload the software directly.");
    checkBox->setChecked(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(checkBox);
    setLayout(layout);
    registerField("uploadDirectly", checkBox, "checked");
}
