#ifndef MODULEWIZARD_H
#define MODULEWIZARD_H

#include <QWizard>
#include <QStringList>
#include <QMap>

class QWizardPage;
class QLineEdit;
class QComboBox;
class QWizard;

class configureModulePage;
class setModuleSourceDirectoryPage;
class selectProgrammerPage;
class reviewInfosPage;
class donePage;


/**
 * @brief A wizard that allow you to configure a new Atmel module and burn it into the Atmega chip.
 */

class moduleWizard : public QWizard
{
    Q_OBJECT
public:
    explicit moduleWizard(QMap<QString, QString> availableTypes, QWidget *parent = 0);

    enum {
        Page_ConfigureModule,
        Page_SaveModuleSource,
        Page_SelectProgrammerPage,
        Page_ReviewInfosPage,
        Page_DonePage
    };

    QString settingsFile() const;
    bool uploadDirectly() const;

private:
    /**
     * @brief The wizard pages
     */
    configureModulePage *m_configureModulePage;
    setModuleSourceDirectoryPage *m_saveModuleSourcePage;
    selectProgrammerPage *m_selectProgrammerPage;
    reviewInfosPage *m_reviewInfosPage;
    donePage *m_donePage;
};

#endif // MODULEWIZARD_H
