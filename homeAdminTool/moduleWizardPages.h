#pragma once

#include <QWizardPage>
#include  "atmelModuleConfigurator.h"

class QLineEdit;
class QLabel;

/**
 * @brief Wizard pages for the module wizard
 */

/**
 * @brief First wizard page: configure the Atmel module (GPIO ports, embedded sensors, etc)
 */
class configureModulePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit configureModulePage(QMap<QString, QString> deviceTypes, QWidget *parent = 0);
    ~configureModulePage()  { delete m_configurator; }
    virtual void initializePage();
    virtual bool validatePage();

private:
    atmelModuleConfigurator *m_configurator;

};

/**
 * @brief Second wizard page: Select where the source file will be saved
 */
class setModuleSourceDirectoryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit setModuleSourceDirectoryPage(QWidget *parent = 0);
    ~setModuleSourceDirectoryPage()  {}
    virtual void initializePage();

private Q_SLOTS:
    void selectDirectory();

protected:
    void updateDirectory();

private:
    QString m_sourceFilePath;
    QLineEdit *m_leModuleSourcePath;
};

/**
 * @brief forth wizard page: Select the programmer to program the chip on the module
 */
class selectProgrammerPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit selectProgrammerPage(QWidget *parent = 0);
    ~selectProgrammerPage()  {}
    virtual void initializePage();

private:
    QComboBox *m_comboBox;
};


/**
 * @brief fifth wizard page: Review informations before starting process.
 */
class reviewInfosPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit reviewInfosPage(QWidget *parent = 0);
    ~reviewInfosPage()  {}
    virtual void initializePage();
    virtual bool validatePage();

private:
    QLabel *m_label;

};

/**
 * @brief last wizard page: select if you want to program the chip directly or not.
 */
class donePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit donePage(QWidget *parent = 0);
    ~donePage()  {}
};

