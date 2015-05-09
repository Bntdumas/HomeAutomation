#include "moduleWizard.h"

#include "moduleWizardPages.h"

#include <QWizard>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QToolButton>
#include <QStringListModel>
#include <QComboBox>
#include <QLabel>
#include <QTextStream>
#include <QMap>
#include <QPair>
#include <QMapIterator>

#include <QDebug>

moduleWizard::moduleWizard(QMap<QString, QString> availableTypes, QWidget *parent) :
    QWizard(parent)
{
    m_configureModulePage = new configureModulePage(availableTypes, this);
    m_saveModuleSourcePage = new setModuleSourceDirectoryPage(this);
    m_selectProgrammerPage = new selectProgrammerPage(this);
    m_reviewInfosPage = new reviewInfosPage(this);
    m_donePage = new donePage(this);

    setPage(Page_ConfigureModule, m_configureModulePage);
    setPage(Page_SaveModuleSource, m_saveModuleSourcePage);
    setPage(Page_SelectProgrammerPage, m_selectProgrammerPage);
    setPage(Page_ReviewInfosPage, m_reviewInfosPage);
    setPage(Page_DonePage, m_donePage);

    setWindowTitle(tr("Atmel module creation wizard"));
    setGeometry(0, 0, 500, 600);
}

QString moduleWizard::settingsFile() const
{
    return field("moduleSourcePath").toString();
}

bool moduleWizard::uploadDirectly() const
{
    return field("uploadDirectly").toBool();
}
