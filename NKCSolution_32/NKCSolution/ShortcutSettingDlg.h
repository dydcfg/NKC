#pragma once
#include <QtWidgets>
#include <Base.h>

class NKCSolution;

class ShortcutSettingDlg : public QDialog
{
    Q_OBJECT 

public: 
    ShortcutSettingDlg(NKCSolution *parent = 0); 
    ~ShortcutSettingDlg(); 

    Shortcut getShortcut(){return shortcut;}
private:
    QListWidget     *scList;
    QComboBox       *scFirst_First;
    QComboBox       *scFirst_Second;
    QComboBox       *scFirst_Third;
    
    QCheckBox       *scIsUsingSecond;
    QCheckBox       *scIsUsingAllKeys;

    QComboBox       *scSecond_First;
    QComboBox       *scSecond_Second;
    QComboBox       *scSecond_Third;

    QLabel          *scName;
    QLineEdit       *scEdit;

    QPushButton     *scAdd;
    QPushButton     *scCancel;
    QPushButton     *scOK;

    Shortcut        shortcut;
private:
    void reloadList(QListWidget *list);
    void initKeyCombo(QComboBox *box);
    void initAssisCombo(QComboBox *box);

    void initSecondAssisCombo(QComboBox *fbox,QComboBox *box);

    void loadNormalKey(bool isFull = false);
    void loadAssistKey(bool isFull = false);
private slots:
    void clickComboBox();
    void clickOK();
    void clickSecondCombineCheckBox();
    void clickAllKeyCheckBox();
    void addShortcut();

};

