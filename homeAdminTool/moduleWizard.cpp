#include "moduleWizard.h"

#include "atmelModuleConfigurator.h"

#include <QWizard>
#include <QWizardPage>
#include <QVBoxLayout>

moduleWizard::moduleWizard(QStringList availableTypes, QObject *parent) :
    QObject(parent),
    m_deviceTypes(availableTypes)
{
    QWizard wizard;
    wizard.addPage(createConfigureModulePage());
    wizard.addPage(createSaveModuleSourcePage());
    wizard.addPage(createSelectCompilerPage());
    wizard.addPage(createReviewInfosPage());
    wizard.addPage(createModulePage());

    wizard.setWindowTitle(tr("Atmel module creation wizard"));
    wizard.exec();
}


QWizardPage* moduleWizard::createConfigureModulePage()
{
    QWizardPage *wizardPage  = new QWizardPage();
    wizardPage->setTitle(tr("Customize your module"));
    atmelModuleConfigurator *configurator = new atmelModuleConfigurator(m_deviceTypes);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(configurator);
    wizardPage->setLayout(layout);

    return wizardPage;
}

QWizardPage* moduleWizard::createSaveModuleSourcePage()
{
    QWizardPage *wizardPage  = new QWizardPage();
    wizardPage->setTitle(tr("Select the directory where the source file will be created"));

    return wizardPage;
}

QWizardPage* moduleWizard::createSelectCompilerPage()
{
    QWizardPage *wizardPage  = new QWizardPage();
    wizardPage->setTitle(tr("Select the compiler and AVR programmer to program the chip."));

    return wizardPage;
}

QWizardPage* moduleWizard::createReviewInfosPage()
{
    QWizardPage *wizardPage  = new QWizardPage();
    wizardPage->setTitle(tr("Review the informations you entered"));

    return wizardPage;
}

QWizardPage* moduleWizard::createModulePage()
{
    QWizardPage *wizardPage  = new QWizardPage();
    wizardPage->setTitle(tr("Creating the module, hold on"));

    return wizardPage;
}

