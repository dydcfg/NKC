#pragma once
#include <QtWidgets> 
#include "Base.h"
#include "ShortcutSettingDlg.h"

class QCheckBox; 
class QLabel; 
class QLineEdit; 
class QPushButton; 
class NKCSolution;
class ControlPanelDlg;

class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralTab( NKCSolution *parent = 0);
    ~GeneralTab();

    void update();
    private slots: 
        void clickControlButton();  
        void clickStartCheckBox();
        void clickWarningCheckBox();
        void clickSoundCheckBox();
        void clickShowStatusCheckBox();

private:
    QCheckBox       *showStatusCheckBox;
    QCheckBox       *startCheckBox;     
    QCheckBox       *soundCheckBox;     
    QCheckBox       *warningCheckBox; 
    QPushButton     *controlButton; 

    NKCSolution     *nsParent;
};

class ShortcutsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ShortcutsTab( NKCSolution *parent = 0);
    ~ShortcutsTab();

    void update();
signals:

    public slots: 
        void clickComboBox();
        void clickAddButton();
        void clickDeleteButton();
        void clickActive();


private:
    void resetTable();
    void loadOptions();
    void saveOptions();
    void addRowByOption(ShortcutOption &option, bool isActive = false);
    QString getStringByShortcut(Shortcut &shortcut);
private:
    int                           activeOption;
    std::vector<ShortcutOption>   shortcutOptions;
    QString                       editString;
    QLabel*                       label; 
    QTableWidget*                 optionTable;
    NKCSolution*                  nsParent;
    QPushButton*                  addButton;
    QPushButton*                  deleteButton;
    ShortcutSettingDlg*           shortcutSettingDlg;
};

class SoundWarningTab : public QWidget
{
    Q_OBJECT

public:
    explicit SoundWarningTab( NKCSolution *parent = 0);
    ~SoundWarningTab();

    void update();
    private slots: 
        void clickControlCheckBox();  
        void clickSitwarningCheckBox();
        void clickAdvanceWarningCheckBox();
        
        void clickLangtimeUsingCheckBox();
        void clickLangtimeSittingCheckBox();
private:
    QCheckBox       *controlCheckBox;
    QCheckBox       *sitwarningCheckBox;     
    QCheckBox       *advancewarningCheckBox;     
    
    QCheckBox       *langtimeUsingCheckBox;     
    QCheckBox       *langtimeSittingCheckBox;     
    
    QGroupBox       *leftgroupBox;
    QGroupBox       *rightgroupBox;
    NKCSolution     *nsParent;
};


class SkinSettingTab : public QWidget
{
    Q_OBJECT

public:
    explicit SkinSettingTab( NKCSolution *parent = 0);
    ~SkinSettingTab();

    void update();
    private slots: 
private:
    
    NKCSolution     *nsParent;
};

class ControlPanelDlg : public QDialog
{
    Q_OBJECT 

public: 
    ControlPanelDlg(NKCSolution *parent = 0); 
    ~ControlPanelDlg(); 

    void show();
    void update();
protected:
    void closeEvent(QCloseEvent *event);

private: 
    QTabWidget      *tabWidget;
    NKCSolution     *nsParent;

    QPushButton     *closeButton; 
    ShortcutsTab    *shortcutsTab;
    GeneralTab      *generalTab;
    SoundWarningTab *soundwarningTab;
    SkinSettingTab  *skinTab;
};

