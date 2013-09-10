
#include <QtGui>
#include <QtWidgets>
#include <QLabel> 
#include <fstream>
#include "ControlPanelDlg.h"
#include "nkcsolution.h"

int g_panelWidth = 700;
int g_panelHeight = 600;

/////////////////////////////////////////////////////////////////////////////////////////////////
//

GeneralTab::GeneralTab(NKCSolution *parent)
    : QWidget(parent)
{
    nsParent = parent;

    controlButton = new QPushButton(tr("Start"));
    startCheckBox = new QCheckBox(tr("Run on Windows Startup"));

    showStatusCheckBox = new QCheckBox(tr("Show Status")); 
    soundCheckBox = new QCheckBox(tr("Turn the sound on")); 
    warningCheckBox = new QCheckBox(tr("Activity Warning")); 

    connect(controlButton, SIGNAL(clicked()), this, SLOT(clickControlButton())); 
    connect(startCheckBox, SIGNAL(clicked()), this, SLOT(clickStartCheckBox())); 
    connect(soundCheckBox, SIGNAL(clicked()), this, SLOT(clickSoundCheckBox())); 
    connect(warningCheckBox, SIGNAL(clicked()), this, SLOT(clickWarningCheckBox())); 
    connect(showStatusCheckBox, SIGNAL(clicked()), this, SLOT(clickShowStatusCheckBox())); 

    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addWidget(startCheckBox); 
    mainLayout->addWidget(showStatusCheckBox); 
    mainLayout->addWidget(soundCheckBox); 
    mainLayout->addWidget(warningCheckBox); 
    mainLayout->addWidget(controlButton,6,Qt::AlignLeft); 

    setLayout(mainLayout); 
    update();
}

GeneralTab::~GeneralTab() 
{ 
    delete controlButton;
    delete startCheckBox;
    delete showStatusCheckBox;
    delete soundCheckBox;
    delete warningCheckBox;
} 

void GeneralTab::clickSoundCheckBox() 
{  

    appState.isSoundOn = !appState.isSoundOn;
    nsParent->update();
} 

void GeneralTab::clickWarningCheckBox() 
{  

    appState.isWarning = !appState.isWarning;
    nsParent->update();
} 
void GeneralTab::clickControlButton() 
{  
    appState.isRunning = !appState.isRunning;
    nsParent->update();
} 

void GeneralTab::clickShowStatusCheckBox() 
{  

    appState.isShowingStatus = !appState.isShowingStatus;
    nsParent->update();
} 

void GeneralTab::clickStartCheckBox() 
{ 
    if (startCheckBox->isChecked())
    {
    }
    else
    {
    }
} 

void GeneralTab::update()
{
    QWidget::update();
    if (appState.isRunning)
    {
        controlButton->setText(tr("Stop"));
    } 
    else
    {
        controlButton->setText(tr("Start"));
    }
    showStatusCheckBox->setChecked(appState.isShowingStatus);  
    soundCheckBox->setChecked(appState.isSoundOn);
    warningCheckBox->setChecked(appState.isWarning);  
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
ShortcutsTab::ShortcutsTab(NKCSolution *parent)
    : QWidget(parent)
{
    nsParent = parent;

    editString = tr("<Edit...>");
    shortcutSettingDlg = new ShortcutSettingDlg(parent);


    label = new QLabel(tr("Action List:")); 

    optionTable = new QTableWidget(this); 
    optionTable->resize(g_panelWidth,g_panelHeight);
    optionTable->setColumnCount(5); 
    float clos[5] = {0.15,0.25,0.25,0.25,0.1};
    for(int i = 0; i < 5; ++i)
    {        
        optionTable->setColumnWidth(i,clos[i] * g_panelWidth * 0.8);
    }
    optionTable->setRowCount(0); 

    optionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    optionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    //optionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    optionTable->verticalHeader()->setVisible(false);
    optionTable->setShowGrid(true);
    optionTable->setStyleSheet("QTableView {selection-background-color: gray;}");
    optionTable->setGeometry(QApplication::desktop()->screenGeometry());


    QStringList headers; 
    headers << tr("Action") <<  tr("Turn Left") <<  tr("Turn Right")  <<  tr("Head Nod") <<  tr("Active"); 
    optionTable->setHorizontalHeaderLabels(headers); 
    loadOptions();

    addButton = new QPushButton(tr("Add"));
    deleteButton = new QPushButton(tr("Delete"));

    connect(addButton, SIGNAL(clicked()), this, SLOT(clickAddButton())); 
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(clickDeleteButton())); 


    QHBoxLayout *buttonLayout = new QHBoxLayout; 
    buttonLayout->addWidget(addButton); 
    buttonLayout->addWidget(deleteButton); 

    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addWidget(label); 
    mainLayout->addWidget(optionTable); 
    mainLayout->addLayout(buttonLayout); 

    setLayout(mainLayout);    
}


ShortcutsTab::~ShortcutsTab() 
{ 
    saveOptions();
    delete shortcutSettingDlg;
    delete optionTable;
    delete addButton;
    delete deleteButton;
} 

void ShortcutsTab::saveOptions()
{
    std::ofstream fout("config/options.cfg");

    fout << appState.hasShortcutOption << " " << activeOption << std::endl;
    for(unsigned int i = 0; i < shortcutOptions.size(); ++i)
    {
        if (shortcutOptions[i].name.size() == 0) continue;
        fout << shortcutOptions[i].name.toStdString() << " ";
        for(int k = 0; k < ShortcutOption::ActionNum; ++k)
        {
            Shortcut &shortcut = shortcutOptions[i].shortcuts[k];
            fout << shortcut.fvalid << " " << shortcut.ffirst  << " " << shortcut.fsecond  << " " << shortcut.fthird  << " "
                << shortcut.svalid  << " " << shortcut.sfirst  << " " << shortcut.ssecond  << " " << shortcut.sthird  << " " << std::endl;
        }
    }
    fout.close();
}


void ShortcutsTab::loadOptions()
{
    std::ifstream fin("config/options.cfg");
    if (!fin) return;
    fin >> appState.hasShortcutOption >> activeOption;

    while (!fin.eof())
    {
        ShortcutOption shortcutOption;
        char str[MAX_PATH];
        fin >> str;
        if (fin.eof()) 
            break;

        shortcutOption.name = str;
        for(int k = 0; k < ShortcutOption::ActionNum; ++k)
        {
            Shortcut &shortcut = shortcutOption.shortcuts[k];
            fin >> shortcut.fvalid >> shortcut.ffirst >> shortcut.fsecond >> shortcut.fthird 
                >> shortcut.svalid >> shortcut.sfirst >> shortcut.ssecond >> shortcut.sthird;
        }
        shortcutOptions.push_back(shortcutOption);
    }
    fin.close();

    if (appState.hasShortcutOption)
        appState.shortcutOption =  shortcutOptions[activeOption];  

    resetTable();
}

void ShortcutsTab::resetTable()
{
    int count = optionTable->rowCount();

    for(int i = 0; i < count; ++i)
        optionTable->removeRow(0);

    for(unsigned int i = 0; i < shortcutOptions.size(); ++i)
    {
        addRowByOption(shortcutOptions[i], i == activeOption);
    }
}

void ShortcutsTab::addRowByOption(ShortcutOption &option,bool isActive)
{
    int curRow = optionTable->rowCount();
    optionTable->setRowCount(optionTable->rowCount() + 1); 
    char name[3] = {'0','0','\0'};
    name[0] = curRow + '0';

    name[1] = '0';
    QTableWidgetItem *nameinput = new QTableWidgetItem(option.name);

    name[1] = '1';
    QComboBox  *leftCombobox = new QComboBox;
    leftCombobox->setObjectName(name);

    QString qstr = leftCombobox->objectName();

    if (option.shortcuts[LEFT_TURN].fvalid == true)
        leftCombobox->addItem(getStringByShortcut(option.shortcuts[LEFT_TURN]));
    leftCombobox->addItem(editString);
    leftCombobox->setCurrentIndex(0);

    name[1] = '2';
    QComboBox  *rightCombobox = new QComboBox;
    rightCombobox->setObjectName(name);
    if (option.shortcuts[RIGHT_TURN].fvalid == true)
        rightCombobox->addItem(getStringByShortcut(option.shortcuts[RIGHT_TURN]));
    rightCombobox->addItem(editString);
    rightCombobox->setCurrentIndex(0);

    name[1] = '3';
    QComboBox  *nodCombobox = new QComboBox;
    nodCombobox->setObjectName(name);
    if (option.shortcuts[NOD_HEAD].fvalid == true)
        nodCombobox->addItem(getStringByShortcut(option.shortcuts[NOD_HEAD]));
    nodCombobox->addItem(editString);
    nodCombobox->setCurrentIndex(0);

    name[1] = '4';
    QCheckBox *checkbox = new QCheckBox;
    checkbox->setObjectName(name);
    checkbox->setChecked(isActive);
    connect(checkbox, SIGNAL(clicked()), this, SLOT(clickActive())); 
    QWidget* checkwidget = new QWidget;

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(checkbox);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignCenter);
    checkwidget->setLayout(layout);

    optionTable->setItem      (curRow, 0, nameinput);
    optionTable->setCellWidget(curRow, 1, leftCombobox);
    optionTable->setCellWidget(curRow, 2, rightCombobox);
    optionTable->setCellWidget(curRow, 3, nodCombobox);
    optionTable->setCellWidget(curRow, 4, checkwidget);

    connect(leftCombobox, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(rightCombobox, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(nodCombobox, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 

    optionTable->selectRow(curRow); 
}

void ShortcutsTab::clickAddButton()
{
    shortcutOptions.push_back(ShortcutOption(QString::number(shortcutOptions.size()) ) );
    addRowByOption(shortcutOptions[shortcutOptions.size() - 1]);
}

void ShortcutsTab::clickActive()
{    
    QString name = sender()->objectName();
    std::string strname = name.toStdString();
    const char *str = strname.c_str();

    if (str != NULL && str[0] > 0)
    {
        if (activeOption != str[0] - '0')
        {
            activeOption = str[0] - '0';
            appState.hasShortcutOption = true;
            appState.shortcutOption =  shortcutOptions[activeOption];            
        }
        else
        {
            appState.hasShortcutOption = false;
            activeOption = -1;
        }
    }

}

void ShortcutsTab::clickDeleteButton()
{
    if (optionTable->currentRow() >= 0)
    {
        std::vector<ShortcutOption>::iterator begin = shortcutOptions.begin();
        shortcutOptions.erase(begin + optionTable->currentRow());
        optionTable->removeRow(optionTable->currentRow());
    }
}

QString ShortcutsTab::getStringByShortcut(Shortcut &shortcut)
{
    QString str;
    if (shortcut.ffirst != 0)
    {
        str.append(appState.keyMap[shortcut.ffirst]);
    }
    if (shortcut.fsecond != 0)
    {
        str.append("+");
        str.append(appState.keyMap[shortcut.fsecond]);
    }
    if (shortcut.fthird != 0)
    {
        if (shortcut.ffirst != 0)
        {
            str.append("+");
        }
        str.append(appState.keyMap[shortcut.fthird]);
    }

    if (shortcut.svalid)
    {
        str.append("\n\r");
        if (shortcut.sfirst != 0)
        {
            str.append(appState.keyMap[shortcut.sfirst]);
        }
        if (shortcut.ssecond != 0)
        {
            str.append("+");
            str.append(appState.keyMap[shortcut.ssecond]);
        }
        if (shortcut.sthird != 0)
        {
            if (shortcut.sfirst != 0)
            {
                str.append("+");
            }
            str.append(appState.keyMap[shortcut.sthird]);
        }
    }
    return str;
}

void ShortcutsTab::clickComboBox()
{
    QComboBox  * box = (QComboBox  *)sender();
    if (box->currentText() == editString)
    {
        shortcutSettingDlg->exec();
        Shortcut shortcut = shortcutSettingDlg->getShortcut();
        if (shortcut.fvalid == false) return;

        QString name = sender()->objectName();
        std::string strname = name.toStdString();
        const char *str = strname.c_str();

        if (str != NULL && str[1] > 0)
        {
            shortcutOptions[str[0] - '0'].shortcuts[str[1] - '1'] = shortcut;

            if (activeOption == (str[0] - '0'))
            {
                appState.shortcutOption =  shortcutOptions[activeOption];  
            }
        }
        box->insertItem(0,getStringByShortcut(shortcut) );
        box->setCurrentIndex(0);
    }
}

void ShortcutsTab::update()
{
    QWidget::update();
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//

SoundWarningTab::SoundWarningTab(NKCSolution *parent)
    : QWidget(parent)
{
    nsParent = parent;     

    controlCheckBox = new QCheckBox(tr("Activity Sound")); 
    sitwarningCheckBox = new QCheckBox(tr("Sit Warning Sound")); 
    advancewarningCheckBox = new QCheckBox(tr("Advance Warning Sound")); 

    langtimeUsingCheckBox = new QCheckBox(tr("Lang time Using Computer"));
    langtimeSittingCheckBox = new QCheckBox(tr("Lang time don't move"));

    connect(controlCheckBox, SIGNAL(clicked()), this, SLOT(clickControlCheckBox())); 
    connect(sitwarningCheckBox, SIGNAL(clicked()), this, SLOT(clickSitwarningCheckBox())); 
    connect(advancewarningCheckBox, SIGNAL(clicked()), this, SLOT(clickAdvanceWarningCheckBox())); 
    connect(langtimeUsingCheckBox, SIGNAL(clicked()), this, SLOT(clickLangtimeUsingCheckBox())); 
    connect(langtimeSittingCheckBox, SIGNAL(clicked()), this, SLOT(clickLangtimeSittingCheckBox())); 

    QVBoxLayout *lay1 = new QVBoxLayout; 
    lay1->addWidget(controlCheckBox); 
    lay1->addWidget(sitwarningCheckBox); 
    lay1->addWidget(advancewarningCheckBox); 
    
    leftgroupBox = new QGroupBox(tr("Sound Control:"));
    leftgroupBox->setLayout(lay1);

    QVBoxLayout *lay2 = new QVBoxLayout; 
    lay2->addWidget(langtimeUsingCheckBox); 
    lay2->addWidget(langtimeSittingCheckBox); 
    
    rightgroupBox = new QGroupBox(tr("Warning Control:"));
    rightgroupBox->setLayout(lay2);

    QHBoxLayout *mainLayout = new QHBoxLayout; 
    mainLayout->addWidget(leftgroupBox);
    mainLayout->addWidget(rightgroupBox);
    setLayout(mainLayout); 
    
    update();
}

SoundWarningTab::~SoundWarningTab() 
{  
    delete controlCheckBox;
    delete sitwarningCheckBox;
    delete advancewarningCheckBox;
    delete langtimeUsingCheckBox;
    delete langtimeSittingCheckBox;
    delete leftgroupBox;
    delete rightgroupBox;
} 

void SoundWarningTab::clickControlCheckBox() 
{  
    appState.soundState.isPlayControlSound = !appState.soundState.isPlayControlSound;
    nsParent->update();
} 

void SoundWarningTab::clickSitwarningCheckBox() 
{  
    appState.soundState.isPlaySitSound = !appState.soundState.isPlaySitSound;
    nsParent->update();
} 
void SoundWarningTab::clickAdvanceWarningCheckBox() 
{  
    appState.soundState.isPlayWarningSound = !appState.soundState.isPlayWarningSound;
    nsParent->update();
} 

void SoundWarningTab::clickLangtimeUsingCheckBox() 
{  
    appState.warningState.isWarnLangTimeUsing = !appState.warningState.isWarnLangTimeUsing;
    nsParent->update();
} 

void SoundWarningTab::clickLangtimeSittingCheckBox() 
{  
    appState.warningState.isWarnLangTimeStay = !appState.warningState.isWarnLangTimeStay;
    nsParent->update();
} 


void SoundWarningTab::update()
{
    QWidget::update();
    leftgroupBox->setEnabled(appState.isSoundOn);
    rightgroupBox->setEnabled(appState.isWarning);

    controlCheckBox->setChecked(appState.soundState.isPlayControlSound);  
    sitwarningCheckBox->setChecked(appState.soundState.isPlaySitSound);
    advancewarningCheckBox->setChecked(appState.soundState.isPlayWarningSound);  
    langtimeUsingCheckBox->setChecked(appState.warningState.isWarnLangTimeUsing);
    langtimeSittingCheckBox->setChecked(appState.warningState.isWarnLangTimeStay);  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//

SkinSettingTab::SkinSettingTab(NKCSolution *parent)
    : QWidget(parent)
{
    nsParent = parent;     
    QGroupBox *groupBox = new QGroupBox(tr("Choose your favorite skin:"));

    QRadioButton *defaultSkin = new QRadioButton(tr("Default Skin"));
    QRadioButton *radio2 = new QRadioButton(tr("Modern Skin"));
    QRadioButton *radio3 = new QRadioButton(tr("Comic Skin"));

    defaultSkin->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(defaultSkin);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);


    QHBoxLayout *mainLayout = new QHBoxLayout; 
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout); 
    update();
}

SkinSettingTab::~SkinSettingTab() 
{  
} 

void SkinSettingTab::update()
{
    QWidget::update();
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
ControlPanelDlg::ControlPanelDlg(NKCSolution *parent) 
{ 
    nsParent = parent;
    resize(g_panelWidth,g_panelHeight);

    generalTab = new GeneralTab(parent);
    shortcutsTab = new ShortcutsTab(parent);
    soundwarningTab = new SoundWarningTab(parent);
    skinTab = new SkinSettingTab(parent);

    closeButton = new QPushButton(tr("Close")); 
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close())); 

    tabWidget = new QTabWidget;
    tabWidget->addTab(generalTab, tr("General"));
    tabWidget->addTab(shortcutsTab, tr("Shortcut Setting"));
    tabWidget->addTab(soundwarningTab, tr("Sound/Warning Setting"));
    tabWidget->addTab(skinTab, tr("Skin Setting"));

    QVBoxLayout *topLeftLayout = new QVBoxLayout; 
    topLeftLayout->addWidget(tabWidget);

    QHBoxLayout *buttomtLayout = new QHBoxLayout; 
    buttomtLayout->addWidget(closeButton,6,Qt::AlignRight); 
    buttomtLayout->addStretch(); 
    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addLayout(topLeftLayout); 
    mainLayout->addLayout(buttomtLayout); 
    setLayout(mainLayout); 

    setWindowTitle(tr("Control Panel")); 
    setFixedHeight(sizeHint().height()); 

    update();
} 

ControlPanelDlg::~ControlPanelDlg() 
{ 
    delete generalTab;
    delete shortcutsTab;
    delete soundwarningTab;
    delete skinTab;
    delete tabWidget;
} 


void ControlPanelDlg::show()
{
    QDialog::show();
    update();
}

void ControlPanelDlg::update()
{
    QDialog::update();
    generalTab->update();
    shortcutsTab->update();
    soundwarningTab->update();
    skinTab->update();
}

void ControlPanelDlg::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}