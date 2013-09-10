#include "ShortcutSettingDlg.h"
#include "windows.h"

struct ComboKey
{
    int index;
    int key;
    QString name;
    ComboKey(int i,int k,QString n):index(i),key(k),name(n){}
};

std::vector<struct ComboKey> g_assistKey;
std::vector<struct ComboKey> g_normalKey;

ShortcutSettingDlg::ShortcutSettingDlg(NKCSolution *)
{
    setWindowTitle(tr("Shortcut Setting Dialog")); 
    setModal(true);

    loadAssistKey();
    loadNormalKey();

    scName = new QLabel(tr("Shortcut Name:"));
    scEdit = new QLineEdit;
    scAdd  = new QPushButton(tr("Add"));
    connect(scAdd, SIGNAL(click()), this, SLOT(addShortcut())); 
    QHBoxLayout *editLayout = new QHBoxLayout; 
    editLayout->addWidget(scName); 
    editLayout->addWidget(scEdit); 
    editLayout->addWidget(scAdd); 

    scFirst_First = new QComboBox;
    scFirst_Second = new QComboBox;
    scFirst_Third = new QComboBox;
    scFirst_Second->setEnabled(false);

    initAssisCombo(scFirst_First);
    initAssisCombo(scFirst_Second);
    initKeyCombo(scFirst_Third);
    initSecondAssisCombo(scFirst_First,scFirst_Second);

    connect(scFirst_First, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(scFirst_Second, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(scFirst_Third, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 

    QHBoxLayout *fcombLayout = new QHBoxLayout; 
    fcombLayout->addWidget(scFirst_First); 
    fcombLayout->addWidget(scFirst_Second); 
    fcombLayout->addWidget(scFirst_Third); 

    QVBoxLayout *firstLayout = new QVBoxLayout; 
    //firstLayout->addLayout(editLayout); 
    firstLayout->addLayout(fcombLayout); 

    scIsUsingSecond = new QCheckBox(tr("Using combine shortcut:"));
    connect(scIsUsingSecond, SIGNAL(clicked()), this, SLOT(clickSecondCombineCheckBox())); 
    
    scIsUsingAllKeys = new QCheckBox(tr("Using all keys"));
    connect(scIsUsingAllKeys, SIGNAL(clicked()), this, SLOT(clickAllKeyCheckBox())); 
    
    QHBoxLayout *checkboxLayout = new QHBoxLayout; 
    checkboxLayout->addWidget(scIsUsingSecond); 
    checkboxLayout->addWidget(scIsUsingAllKeys); 

    scSecond_First = new QComboBox;
    scSecond_Second = new QComboBox;
    scSecond_Third = new QComboBox;
    scSecond_First->setEnabled(false);
    scSecond_Second->setEnabled(false);
    scSecond_Third->setEnabled(false);

    initAssisCombo(scSecond_First);
    initAssisCombo(scSecond_Second);
    initKeyCombo(scSecond_Third);
    initSecondAssisCombo(scSecond_First,scSecond_Second);

    connect(scSecond_First, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(scSecond_Second, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 
    connect(scSecond_Third, SIGNAL(activated(QString)), this, SLOT(clickComboBox())); 

    QHBoxLayout *scombLayout = new QHBoxLayout; 
    scombLayout->addWidget(scSecond_First); 
    scombLayout->addWidget(scSecond_Second); 
    scombLayout->addWidget(scSecond_Third); 

    QVBoxLayout *secondLayout = new QVBoxLayout; 
    secondLayout->addLayout(checkboxLayout);  
    secondLayout->addLayout(scombLayout); 


    scList = new QListWidget;
    reloadList(scList);

    QVBoxLayout *inputLayout = new QVBoxLayout; 
    inputLayout->addLayout(firstLayout); 
    inputLayout->addLayout(secondLayout); 
    //inputLayout->addWidget(scList); 

    scOK = new QPushButton(tr("OK"));
    scCancel = new QPushButton(tr("Cancel"));
    scOK->setEnabled(false);
    connect(scOK, SIGNAL(clicked()), this, SLOT(clickOK())); 
    connect(scCancel, SIGNAL(clicked()), this, SLOT(close())); 


    QHBoxLayout *buttonLayout = new QHBoxLayout; 
    buttonLayout->addWidget(scOK); 
    buttonLayout->addWidget(scCancel); 

    QVBoxLayout *mainLayout = new QVBoxLayout; 
    mainLayout->addLayout(inputLayout); 
    mainLayout->addLayout(buttonLayout); 


    setLayout(mainLayout);
}


ShortcutSettingDlg::~ShortcutSettingDlg(void)
{
    delete     scList;
    delete     scFirst_First;
    delete     scFirst_Second;
    delete     scFirst_Third;

    delete     scIsUsingSecond;

    delete     scSecond_First;
    delete     scSecond_Second;
    delete     scSecond_Third;

    delete     scEdit;

    delete     scCancel;
    delete     scOK;
}

void ShortcutSettingDlg::loadNormalKey(bool isFull)
{
    g_normalKey.clear();
    int count = 0;
    g_normalKey.push_back(ComboKey(count,0,        appState.keyMap[0]));
    count++;
    for(int i = 0; i < 26; ++i)
    {
        g_normalKey.push_back(ComboKey(count,0x41 + i,  appState.keyMap[0x41 + i]));
        count++;
    }
    for(int i = 0; i < 10; ++i)
    {
        g_normalKey.push_back(ComboKey(count,0x30 + i,  appState.keyMap[0x30 + i]));
        count++;
    }
    g_normalKey.push_back(ComboKey(count,VK_SPACE,  appState.keyMap[VK_SPACE]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_TAB,    appState.keyMap[VK_TAB]));       count++;
    g_normalKey.push_back(ComboKey(count,VK_RETURN, appState.keyMap[VK_RETURN]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_ESCAPE, appState.keyMap[VK_ESCAPE]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_BACK,   appState.keyMap[VK_BACK]));      count++;

    g_normalKey.push_back(ComboKey(count,VK_UP,      appState.keyMap[VK_UP]));       count++;
    g_normalKey.push_back(ComboKey(count,VK_RIGHT,   appState.keyMap[VK_RIGHT]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_DOWN,    appState.keyMap[VK_DOWN]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_LEFT,    appState.keyMap[VK_LEFT]));     count++;

    if (isFull == true)
    {
    g_normalKey.push_back(ComboKey(count,VK_ADD,         appState.keyMap[VK_ADD]));         count++;
    g_normalKey.push_back(ComboKey(count,VK_SUBTRACT,    appState.keyMap[VK_SUBTRACT]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_MULTIPLY,    appState.keyMap[VK_MULTIPLY]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_DIVIDE,      appState.keyMap[VK_DIVIDE]));      count++;
    g_normalKey.push_back(ComboKey(count,VK_DECIMAL,     appState.keyMap[VK_DECIMAL]));     count++;

    g_normalKey.push_back(ComboKey(count,VK_NUMPAD0,   appState.keyMap[VK_NUMPAD0]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD1,   appState.keyMap[VK_NUMPAD1]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD2,   appState.keyMap[VK_NUMPAD2]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD3,   appState.keyMap[VK_NUMPAD3]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD4,   appState.keyMap[VK_NUMPAD4]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD5,   appState.keyMap[VK_NUMPAD5]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD6,   appState.keyMap[VK_NUMPAD6]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD7,   appState.keyMap[VK_NUMPAD7]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD8,   appState.keyMap[VK_NUMPAD8]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_NUMPAD9,   appState.keyMap[VK_NUMPAD9]));    count++;

    
    g_normalKey.push_back(ComboKey(count,VK_F1,    appState.keyMap[VK_F1]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F2,    appState.keyMap[VK_F2]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F3,    appState.keyMap[VK_F3]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F4,    appState.keyMap[VK_F4]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F5,    appState.keyMap[VK_F5]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F6,    appState.keyMap[VK_F6]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F7,    appState.keyMap[VK_F7]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F8,    appState.keyMap[VK_F8]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F9,    appState.keyMap[VK_F9]));     count++;
    g_normalKey.push_back(ComboKey(count,VK_F10,   appState.keyMap[VK_F10]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_F11,   appState.keyMap[VK_F11]));    count++;
    g_normalKey.push_back(ComboKey(count,VK_F12,   appState.keyMap[VK_F12]));    count++;
    }
}

void ShortcutSettingDlg::loadAssistKey(bool isFull)
{
    g_assistKey.clear();
    g_assistKey.push_back(ComboKey(0,0,            appState.keyMap[0]));
    g_assistKey.push_back(ComboKey(1,VK_CONTROL,   appState.keyMap[VK_CONTROL]));
    g_assistKey.push_back(ComboKey(2,VK_SHIFT,     appState.keyMap[VK_SHIFT]));
    g_assistKey.push_back(ComboKey(3,VK_MENU,      appState.keyMap[VK_MENU]));
}
void ShortcutSettingDlg::reloadList(QListWidget *list)
{
}

void ShortcutSettingDlg::initKeyCombo(QComboBox *box)
{
    box->clear();
    for(unsigned int i = 0; i < g_normalKey.size(); ++i)
    {
        box->addItem((g_normalKey[i].name));
    }
}
void ShortcutSettingDlg::initAssisCombo(QComboBox *box)
{
    box->clear();

    for(unsigned int i = 0; i < g_assistKey.size(); ++i)
    {
        box->addItem((g_assistKey[i].name));
    }
}


void ShortcutSettingDlg::clickComboBox()
{
    scFirst_Second->setEnabled(scFirst_First->currentIndex() != 0);
    scSecond_Second->setEnabled(scSecond_First->currentIndex() != 0);
    initSecondAssisCombo(scFirst_First,scFirst_Second);
    initSecondAssisCombo(scSecond_First,scSecond_Second);

    if (scFirst_Second->currentIndex() == scFirst_First->currentIndex() || !scFirst_Second->isEnabled())
        scFirst_Second->setCurrentIndex(0);

    if (scSecond_Second->currentIndex() == scSecond_First->currentIndex() || !scSecond_Second->isEnabled())
        scSecond_Second->setCurrentIndex(0);


    scOK->setEnabled(scFirst_First->currentIndex() != 0 || scFirst_Third->currentIndex() != 0);

}

void ShortcutSettingDlg::clickAllKeyCheckBox()
{
    loadAssistKey(scIsUsingAllKeys->isChecked());
    loadNormalKey(scIsUsingAllKeys->isChecked());
    
    initAssisCombo(scFirst_First);
    initAssisCombo(scFirst_Second);
    initKeyCombo(scFirst_Third);
    initSecondAssisCombo(scFirst_First,scFirst_Second);
    initAssisCombo(scSecond_First);
    initAssisCombo(scSecond_Second);
    initKeyCombo(scSecond_Third);
    initSecondAssisCombo(scSecond_First,scSecond_Second);

    repaint();
}
void ShortcutSettingDlg::clickSecondCombineCheckBox()
{
    scSecond_First->setEnabled(scIsUsingSecond->isChecked());
    scSecond_Third->setEnabled(scIsUsingSecond->isChecked());
    initSecondAssisCombo(scSecond_First,scSecond_Second);
}

void ShortcutSettingDlg::clickOK()
{
    shortcut.fvalid = true;
    shortcut.ffirst = g_assistKey[scFirst_First->currentIndex()].key;
    shortcut.fsecond = g_assistKey[scFirst_Second->currentIndex()].key;
    shortcut.fthird = g_normalKey[scFirst_Third->currentIndex()].key;

    shortcut.svalid = scIsUsingSecond->isChecked();
    shortcut.sfirst = g_assistKey[scSecond_First->currentIndex()].key;
    shortcut.ssecond = g_assistKey[scSecond_Second->currentIndex()].key;
    shortcut.sthird = g_normalKey[scSecond_Third->currentIndex()].key;

    close();
}


void ShortcutSettingDlg::initSecondAssisCombo(QComboBox *fbox,QComboBox *box)
{
    if (!box->isEnabled()) return;

    // enable item
    for(int i = 0; i < box->count(); ++i)
    {
        box->setItemData(i, 33, Qt::UserRole - 1);
    }
    // disable item
    box->setItemData(fbox->currentIndex(), 0, Qt::UserRole - 1);
}

void ShortcutSettingDlg::addShortcut()
{
}