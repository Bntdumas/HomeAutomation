#ifndef MODULEWIZARD_H
#define MODULEWIZARD_H

#include <QObject>
#include <QStringList>

class QWizardPage;

/**
 * @brief A wizard that allow you to configure a new Atmel module and burn it into the Atmega chip.
 */

class moduleWizard : public QObject
{
    Q_OBJECT
public:
    explicit moduleWizard(QStringList availableTypes, QObject *parent = 0);

protected:
    QWizardPage *createModulePage();
    QWizardPage* createConfigureModulePage();
    QWizardPage* createSaveModuleSourcePage();
    QWizardPage* createSelectCompilerPage();
    QWizardPage* createReviewInfosPage();

private:
    QStringList m_deviceTypes;
    QString m_sourceFile;
};

#endif // MODULEWIZARD_H
