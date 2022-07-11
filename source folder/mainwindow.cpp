#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    qApp->setStyle(QStyleFactory::create("fusion"));
    int fontID=QFontDatabase::addApplicationFont(":/rec/font/gandom/Gandom-Bold.ttf");
    QFont font=QFontDatabase::applicationFontFamilies(fontID).at(0);
    font.setPointSize(9);
    qApp->setFont(font);


//    QPalette palette;
//    palette.setColor(QPalette::Window, QColor(53,53,53));
//    palette.setColor(QPalette::WindowText, Qt::white);
//    palette.setColor(QPalette::Base, QColor(15,15,15));
//    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
//    palette.setColor(QPalette::ToolTipBase, Qt::white);
//    palette.setColor(QPalette::ToolTipText, Qt::white);
//    palette.setColor(QPalette::Text, Qt::white);
//    palette.setColor(QPalette::Button, QColor(53,53,53));
//    palette.setColor(QPalette::ButtonText, Qt::white);
//    palette.setColor(QPalette::BrightText, Qt::red);

//    palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
//    palette.setColor(QPalette::HighlightedText, Qt::black);
//    qApp->setPalette(palette);



    QCoreApplication::setApplicationName("Abyaz");
    QCoreApplication::setOrganizationName("Abyaz-co");
    qApp->setWindowIcon(QIcon(":/rec/icon.ico"));
    config();
    loadSetting();

    ui->pushButton_infinityRepeat->setHidden(true);



}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::config()
{

    srand((int)time(0));

    setDefultSetting();
    setShortcuts();
    downloader=new Downloader();
    downloadControl=new DownloadControl(this);
    db=new Database;
    allQuran=db->getListAllSore();
    langTransList=db->getLangTranList();
    qary=db->getQary();

    createPages();
    createJuzList();
    createBookmarkList();


    currentAyeOBJ=allQuran->value(currentSore)->getAye().at(currentAye-1);

    scenePage=new ScenePage(ui->graphicsView);

    ui->graphicsView->setScene(scenePage);
    ui->graphicsView->setSceneRect(scenePage->sceneRect());
    scenePage->paintPage(pages.at(currentPage-1),currentAyeOBJ->getAyeInTwoPage());

    searchText=db->getSearchText();
    ui->treeWidget_index->setAllQuran(allQuran);
    ui->treeWidget_index->setSearchText(searchText);
    ui->treeWidget_index->showList();
    ui->treeWidget_index->setTypeTreeWidget(Index::TreeIndex);
    setComboFilterIndex();


    ui->tableWidget_bookmark->setBookmarks(bookmarks);


//    pageCombo=new QComboBox(this);
//    soreCombo=new QComboBox(this);
//    ayeCombo=new QComboBox(this);
//    juzCombo=new QComboBox(this);
//    qaryCombo=new QComboBox(this);

//    pageComboLabel=new QLabel("page",this);
//    soreComboLabel=new QLabel("sore",this);
//    ayeComboLabel=new QLabel("aye",this);
//    juzComboLabel=new QLabel("juz",this);
//    qaryComboLabel=new QLabel("qary",this);


//    ui->toolButton_aye_next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
//    ui->toolButton_pre_aye->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
//    ui->toolButton_forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
//    ui->toolButton_backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

//    ui->toolButton_download->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));

    setComboSoundPanelItem();
    setSoundPanelDirection();


//    ui->combo_trans_lang=new QComboBox(this);
//    ui->combo_trans_size=new QComboBox(this);
//    ui->combo_trans_trans=new QComboBox(this);
//    ui->label_trans_lang=new QLabel("lang",this);
//    ui->label_trans_size=new QLabel("size",this);
//    ui->label_trans_trans=new QLabel("trans",this);

//    ui->button_trans_color=new QPushButton("color",this);
    setComboTransLangItem();
    setTranslateWidget();

    player=new Audio();
    handleQaryCombo(0);


    setting=new Setting(this);
    setting->setQary(qary);



    ui->treeWidget_search->setAllQuran(allQuran);
    ui->treeWidget_search->setSearchText(searchText);
    ui->treeWidget_search->setTypeTreeWidget(Index::TreeSearch);

    setComboLangSearch();//call after setCombo_lang_trans becuse for langtransList

    setComboLangBookmark();
    setComboFilterBookmark();


    soreDialog=new SoreDialog(this);

    addRead=new AddRead(this);

    helpDialog=new Help(this);

    setExamCombos();



    examAye=NULL;
    examAyeList=new QList<Aye*>();

    ui->radioButton_examSound->setChecked(true);

    connect(addRead,SIGNAL(pageRead(int,int,int)),this,SLOT(handleAyeRead(int,int,int)));

    connect(scenePage,SIGNAL(itemClicked(int,int)),this,SLOT(clickedAye(int,int)));

    connect(ui->comboBox_page,SIGNAL(currentIndexChanged(int)),this,SLOT(handlePageCombo(int)));
    connect(ui->comboBox_sore,SIGNAL(currentIndexChanged(int)),this,SLOT(handleSoreCombo(int)));
    connect(ui->comboBox_aye,SIGNAL(currentIndexChanged(int)),this,SLOT(handleAyeCombo(int)));
    connect(ui->comboBox_juz,SIGNAL(currentIndexChanged(int)),this,SLOT(handleJuzCombo(int)));
    connect(ui->comboBox_qary,SIGNAL(currentIndexChanged(int)),this,SLOT(handleQaryCombo(int)));



    connect(player,SIGNAL(duration(int)),this,SLOT(handleDurationPlayer(int)));
    connect(player,SIGNAL(changePosition(int)),this,SLOT(handlePlayerPosition(int)));
    connect(ui->horizontalSlider_Main,SIGNAL(changedValue(int)),this,SLOT(handleChangeValueMainSlider(int)));
    connect(ui->slider_volum,SIGNAL(sliderMoved(int)),player,SLOT(setVolum(int)));
    connect(ui->slider_volum,SIGNAL(changedValue(int)),player,SLOT(setVolum(int)));
    connect(ui->slider_volum,SIGNAL(valueChanged(int)),player,SLOT(setVolum(int)));

    connect(player,SIGNAL(volumChange(int)),ui->slider_volum,SLOT(setValue(int)));

    connect(player,SIGNAL(endSound()),this,SLOT(endSoundHandle()));
    connect(this,SIGNAL(endBookmarkPlay()),ui->tableWidget_bookmark,SLOT(handleEndBookmarkPlay()));

    connect(player,SIGNAL(fadeEnd()),this,SLOT(handleFadeEnd()));



    connect(downloader,SIGNAL(report(QString)),this,SLOT(handleDownloadReport(QString)));
    connect(downloader,SIGNAL(downloaded(DownloadInfo*)),this,
            SLOT(handleDownloadedFile(DownloadInfo*)));
    connect(downloader,SIGNAL(updatedDownloadList(QQueue<DownloadInfo*>*)),
            downloadControl,SLOT(setDownloadList(QQueue<DownloadInfo*>*)));
    connect(downloader,SIGNAL(bytesReceived(qint64,qint64)),
            downloadControl,SLOT(reciveCurrentDownload(qint64,qint64)));

    connect(downloadControl,SIGNAL(stop()),downloader,SLOT(stopDownload()));

    connect(ui->tableWidget_bookmark,SIGNAL(playBookmark(Aye*)),
            this,SLOT(playBookmark(Aye*)));

    connect(ui->tableWidget_bookmark,SIGNAL(updateBoomark(int,int,QString)),
            ui->treeWidget_index,SLOT(changeBackgroundColor(int,int,QString)));
    connect(ui->tableWidget_bookmark,SIGNAL(deleteBookmark(int,int,QString)),
            ui->treeWidget_index,SLOT(changeBackgroundColor(int,int,QString)));

    connect(ui->tableWidget_bookmark,SIGNAL(updateBoomark(int,int,QString)),
            ui->treeWidget_search,SLOT(changeBackgroundColor(int,int,QString)));
    connect(ui->tableWidget_bookmark,SIGNAL(deleteBookmark(int,int,QString)),
            ui->treeWidget_search,SLOT(changeBackgroundColor(int,int,QString)));

    connect(ui->tableWidget_bookmark,SIGNAL(updateBoomark(int,int,QString)),
            ui->treeWidget_search,SLOT(changeBackgroundColor(int,int,QString)));
    connect(ui->tableWidget_bookmark,SIGNAL(deleteBookmark(int,int,QString)),
            ui->treeWidget_search,SLOT(changeBackgroundColor(int,int,QString)));

    connect(scenePage,SIGNAL(clickedContext(int,int,QString)),this,
            SLOT(bookmarkHandle(int,int,QString)));
    connect(scenePage,SIGNAL(clickedContext(int,int,QString)),
            ui->treeWidget_index,SLOT(changeBackgroundColor(int,int,QString)));
    connect(scenePage,SIGNAL(clickedContext(int,int,QString)),
            ui->treeWidget_search,SLOT(changeBackgroundColor(int,int,QString)));

    connect(ui->tableWidget_bookmark,SIGNAL(updateBoomark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));
    connect(ui->tableWidget_bookmark,SIGNAL(deleteBookmark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));
    connect(ui->treeWidget_index,SIGNAL(updateBookmark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));
    connect(ui->treeWidget_index,SIGNAL(deleteBookmark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));

    connect(ui->treeWidget_search,SIGNAL(updateBookmark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));
    connect(ui->treeWidget_search,SIGNAL(deleteBookmark(int,int,QString)),
            this,SLOT(bookmarkHandle(int,int,QString)));

    connect(ui->tableWidget_bookmark,SIGNAL(clickedDoubleItem(Aye*)),
            this,SLOT(itemsClickHandle(Aye*)));
    connect(ui->treeWidget_index,SIGNAL(clickedItem(Aye*)),
            this,SLOT(itemsClickHandle(Aye*)));
    connect(ui->treeWidget_search,SIGNAL(clickedItem(Aye*)),this,
            SLOT(itemsClickHandle(Aye*)));

    connect(setting,SIGNAL(downloadSetting(Global::DownloadState)),this,
            SLOT(setDownloadState(Global::DownloadState)));

    connect(setting,SIGNAL(addQarySetting(Qary*)),this,SLOT(handleAddQary(Qary*)));
    connect(setting,SIGNAL(editQarySetting(Qary*)),this,SLOT(handleEditQary(Qary*)));
    connect(setting,SIGNAL(deleteQarySetting(Qary*)),this,SLOT(handleDeleteQary(Qary*)));

    connect(setting,SIGNAL(languageSetting(QString)),this,SLOT(handleLanguageSetting(QString)));
    connect(setting,SIGNAL(resetDocks()),this,SLOT(handleResetSetting()));

    connect(ui->combo_trans_lang,SIGNAL(currentTextChanged(QString)),
            this,SLOT(setComboTrans_trans(QString)));
    connect(ui->combo_trans_size,SIGNAL(currentTextChanged(QString))
            ,this,SLOT(setSizeTrans(QString)));
    connect(ui->button_trans_color,SIGNAL(clicked(bool)),
            this,SLOT(choosetransColor()));
    connect(ui->combo_trans_trans,SIGNAL(currentTextChanged(QString)),
            this,SLOT(setCurrentTranslation(QString)));
    connect(ui->combo_trans_trans,SIGNAL(currentTextChanged(QString)),
            this,SLOT(setCurrentTranslation(QString)));
    connect(ui->combo_trans_size,SIGNAL(currentTextChanged(QString)),
            this,SLOT(setSizeTrans(QString)));

    connect(ui->comboBox_lang_search,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(setComboTransSearch(QString)));
    connect(ui->comboBox_lang_book,SIGNAL(currentTextChanged(QString)),
            this,SLOT(setComboTransBookmark(QString)));

    connect(ui->dockWidget_index_2,SIGNAL(close()),
            ui->btn_index,SLOT(trigger()));
    connect(ui->dockWidget_search_2,SIGNAL(close()),
            ui->btn_search,SLOT(trigger()));
    connect(ui->dock_bookmark_2,SIGNAL(close()),
            ui->btn_bookmark,SLOT(trigger()));
    connect(ui->dock_trans,SIGNAL(close()),
            ui->btn_translate,SLOT(trigger()));
    connect(ui->dockWidget_exam,SIGNAL(close()),
            ui->btn_exam,SLOT(trigger()));

    connect(ui->label_examProperty,SIGNAL(clicked()),this,SLOT(handleDoubleClickExamLabel()));



    handleLanguageSetting(Language::getCurrentLanguage());

    setCentrMainWindow();
    showNormal();
    adjustSize();
    showMaximized();

    setFocus();



}

void MainWindow::setDefultSetting()
{

    Language::setCurrentLanguage("persian");
    Language::loadLanguages();

    playerState=Global::NormalPlay;
    direction=Qt::RightToLeft;
    translatioColor=Qt::black;
    translationFontSize="10";
    combosChanged=false;
    repeatState=Global::NoRepeat;
    downloadStat=Global::DownloadAye;
    currentSore=1;
    currentAye=1;
    currentPage=1;
    hiddenPage=false;
}
void MainWindow::loadSetting()
{

    if(setting->settingExist()){
        downloadStat=static_cast<Global::DownloadState>(setting->loadSetting("downloadState").toUInt());
        ui->comboBox_qary->setCurrentText(setting->loadSetting("qary").toString());
        ui->comboBox_sore->setCurrentText(setting->loadSetting("sore").toString());
        ui->comboBox_aye->setCurrentText(QString::number(setting->loadSetting("aye").toInt()));

        Language::setCurrentLanguage(setting->loadSetting("language").toString());
        handleLanguageSetting(Language::getCurrentLanguage());

        if(setting->loadSetting("indexDock").toBool()){
            ui->btn_index->setChecked(true);
            ui->dockWidget_index_2->setHidden(false);
        }
        else{
            ui->btn_index->setChecked(false);
            ui->dockWidget_index_2->setHidden(true);
        }

        if(setting->loadSetting("searchDock").toBool()){
            ui->btn_search->setChecked(true);
            ui->dockWidget_search_2->setHidden(false);
        }
        else{
            ui->btn_search->setChecked(false);
            ui->dockWidget_search_2->setHidden(true);
        }


        if(setting->loadSetting("bookmarkDock").toBool()){
            ui->btn_bookmark->setChecked(true);
            ui->dock_bookmark_2->setHidden(false);
        }
        else{
            ui->btn_bookmark->setChecked(false);
            ui->dock_bookmark_2->setHidden(true);
        }


        if(setting->loadSetting("transDock").toBool()){
            ui->btn_translate->setChecked(true);
            ui->dock_trans->setHidden(false);
        }
        else{
            ui->btn_translate->setChecked(false);
            ui->dock_trans->setHidden(true);
        }

        if(setting->loadSetting("examDock").toBool()){
            ui->btn_exam->setChecked(true);
            ui->dockWidget_exam->setHidden(false);
        }
        else{
            ui->btn_exam->setChecked(false);
            ui->dockWidget_exam->setHidden(true);
        }

        setColorTrans(QColor(setting->loadSetting("colorTrans").toInt()));
        ui->combo_trans_size->setCurrentText(setting->loadSetting("sizeTrans").toString());

        this->restoreState(setting->loadSetting("mainWindowState").toByteArray());
        window->restoreState(setting->loadSetting("centerWindowState").toByteArray());


        ui->combo_trans_lang->setCurrentText(setting->loadSetting("lang_trans").toString());
        ui->combo_trans_trans->setCurrentText(setting->loadSetting("trans_trans").toString());

        player->setVolum(setting->loadSetting("volum").toInt());
        ui->slider_volum->setValue(setting->loadSetting("volum").toInt());


        QString typeExam=setting->loadSetting("type_exam").toString();

        if(QString::compare(typeExam,"aye")==0){

            ui->radioButton_examAye->setChecked(true);
            ui->radioButton_examAye->toggled(true);
        }
        else if(QString::compare(typeExam,"sound")==0){
            ui->radioButton_examSound->setChecked(true);
            ui->radioButton_examSound->toggled(true);
        }
        else if(QString::compare(typeExam,"text")==0){
            ui->radioButton_examText->setChecked(true);
            ui->radioButton_examText->toggled(true);
        }
        else if(QString::compare(typeExam,"page")==0){
            ui->radioButton_examPage->setChecked(true);
            ui->radioButton_examPage->toggled(true);
        }

        ui->checkBox_examLimitPage->setChecked(setting->loadSetting("limit_page").toBool());
        ui->checkBox_examLimitSore->setChecked(setting->loadSetting("limit_sore").toBool());
        ui->checkBox_examLimitAye->setChecked(setting->loadSetting("limit_aye").toBool());

        ui->checkBox_examLimitPage->toggled(setting->loadSetting("limit_page").toBool());
        ui->checkBox_examLimitSore->toggled(setting->loadSetting("limit_sore").toBool());
        ui->checkBox_examLimitAye->toggled(setting->loadSetting("limit_aye").toBool());

        ui->comboBox_examQary->setCurrentText(setting->loadSetting("qary_exam").toString());

        ui->comboBox_examSore->setCurrentText(setting->loadSetting("examSore").toString());
        ui->comboBox_examFromPage->setCurrentText(setting->loadSetting("examPageFrom").toString());
        ui->comboBox_examToPage->setCurrentText(setting->loadSetting("examPageTo").toString());
        ui->comboBox_examFromAye->setCurrentText(setting->loadSetting("examAyeFrom").toString());
        ui->comboBox_examFromAye->setCurrentText(setting->loadSetting("examAyeTo").toString());



    }else{
        ui->comboBox_qary->setCurrentIndex(1);
    }

    setCurrentStates(currentAyeOBJ);
    showNormal();
    adjustSize();
    showMaximized();

}

void MainWindow::saveSetting()
{

    setting->saveSetting("apprun",1);

    setting->saveSetting("downloadState",downloadStat);
    setting->saveSetting("qary",currentQary->getName());
    setting->saveSetting("sore",currentAyeOBJ->getSoreName());
    setting->saveSetting("aye",currentAye);

    setting->saveSetting("language",Language::getCurrentLanguage());
    setting->saveSetting("indexDock",ui->btn_index->isChecked());
    setting->saveSetting("bookmarkDock",ui->btn_bookmark->isChecked());
    setting->saveSetting("searchDock",ui->btn_search->isChecked());
    setting->saveSetting("transDock",ui->btn_translate->isChecked());
    setting->saveSetting("examDock",ui->btn_exam->isChecked());

    setting->saveSetting("centerWindowState",window->saveState());
    setting->saveSetting("mainWindowState",this->saveState());

    setting->saveSetting("colorTrans",ui->textEdit_trans->textColor().rgb());
    setting->saveSetting("sizeTrans",ui->combo_trans_size->currentText());

    setting->saveSetting("trans_trans",ui->combo_trans_trans->currentText());
    setting->saveSetting("lang_trans",ui->combo_trans_lang->currentText());

    setting->saveSetting("volum",player->getVolum());


    if(ui->radioButton_examAye->isChecked())
        setting->saveSetting("type_exam","aye");
    else if(ui->radioButton_examPage->isChecked())
        setting->saveSetting("type_exam","page");
    else if(ui->radioButton_examSound->isChecked())
        setting->saveSetting("type_exam","sound");
    else if(ui->radioButton_examText->isChecked())
        setting->saveSetting("type_exam","text");
    setting->saveSetting("limit_page",ui->checkBox_examLimitPage->isChecked());
    setting->saveSetting("limit_sore",ui->checkBox_examLimitSore->isChecked());
    if(ui->checkBox_examLimitAye->isEnabled())
        setting->saveSetting("limit_aye",ui->checkBox_examLimitAye->isChecked());
    else
        setting->saveSetting("limit_aye",false);

    setting->saveSetting("qary_exam",ui->comboBox_examQary->currentText());

    setting->saveSetting("examSore",ui->comboBox_examSore->currentText());
    setting->saveSetting("examPageFrom",ui->comboBox_examFromPage->currentText());
    setting->saveSetting("examPageTo",ui->comboBox_examToPage->currentText());
    setting->saveSetting("examAyeFrom",ui->comboBox_examFromAye->currentText());
    setting->saveSetting("examAyeTo",ui->comboBox_examToAye->currentText());


}

void MainWindow::show()
{
   // setTaskbarWidgets();
    QMainWindow::show();

}

QMainWindow *MainWindow::getCentralWindow()
{
    return window;
}




void MainWindow::setTaskbarWidgets()
{


    taskbar=new QWinThumbnailToolBar(this);
    taskbar->setWindow(this->windowHandle());

    tskNext=new QWinThumbnailToolButton(taskbar);
    tskNext->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    tskPlay=new QWinThumbnailToolButton(taskbar);
    tskPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    tskPause=new QWinThumbnailToolButton(taskbar);
    tskPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    tskPre=new QWinThumbnailToolButton(taskbar);
    tskPre->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

    connect(tskNext,SIGNAL(clicked()),this,SLOT(nextAye()));
    connect(tskPlay,SIGNAL(clicked()),this,SLOT(on_pushButton_playStop_clicked()));
    connect(tskPause,SIGNAL(clicked()),this,SLOT(on_pushButton_pausePlay_clicked()));
    connect(tskPre,SIGNAL(clicked()),this,SLOT(preAye()));

    tskPause->setEnabled(false);


    taskbar->addButton(tskPre);
    taskbar->addButton(tskPause);
    taskbar->addButton(tskPlay);
    taskbar->addButton(tskNext);


}



void MainWindow::setTranslateWidget()
{
    ui->horizontalLayout_translate->removeWidget(ui->button_trans_color);
    ui->horizontalLayout_translate->removeWidget(ui->combo_trans_size);
    ui->horizontalLayout_translate->removeWidget(ui->label_trans_size);
    ui->horizontalLayout_translate->removeWidget(ui->combo_trans_trans);
    ui->horizontalLayout_translate->removeWidget(ui->label_trans_trans);
    ui->horizontalLayout_translate->removeWidget(ui->combo_trans_lang);
    ui->horizontalLayout_translate->removeWidget(ui->label_trans_lang);
    ui->horizontalLayout_translate->removeItem(ui->horizontalSpacer_trans_lang);


    ui->horizontalLayout_translate->addItem(ui->horizontalSpacer_trans_lang);

    if(direction==Qt::RightToLeft){
        ui->horizontalLayout_translate->addWidget(ui->label_trans_lang);
        ui->horizontalLayout_translate->addWidget(ui->combo_trans_lang);
        ui->horizontalLayout_translate->addWidget(ui->label_trans_trans);
        ui->horizontalLayout_translate->addWidget(ui->combo_trans_trans);
        ui->horizontalLayout_translate->addWidget(ui->label_trans_size);
        ui->horizontalLayout_translate->addWidget(ui->combo_trans_size);
        ui->horizontalLayout_translate->addWidget(ui->button_trans_color);
    }
    else if(direction==Qt::LeftToRight){


        ui->horizontalLayout_translate->addWidget(ui->combo_trans_lang);
        ui->horizontalLayout_translate->addWidget(ui->label_trans_lang);
        ui->horizontalLayout_translate->addWidget(ui->combo_trans_trans);
        ui->horizontalLayout_translate->addWidget(ui->label_trans_trans);
        ui->horizontalLayout_translate->addWidget(ui->combo_trans_size);
        ui->horizontalLayout_translate->addWidget(ui->label_trans_size);
        ui->horizontalLayout_translate->addWidget(ui->button_trans_color);

    }


}

void MainWindow::setComboTransLangItem()
{

    ui->combo_trans_lang->addItems(QStringList(langTransList->keys()));
    setComboTrans_trans(ui->combo_trans_lang->currentText());

    for(int i=8;i<=32;i++)
        ui->combo_trans_size->addItem(QString::number(i).replace(QChar(10),' '));//remove next line



}

void MainWindow::showTranslation()
{
    if(!currentTranslation.isEmpty()){
        if(currentAyeOBJ->getChapter()!=1 && currentAyeOBJ->getChapter()!=9)
            ui->textEdit_trans->setText(currentTranslation.value(currentAyeOBJ->getChapter()).at(currentAyeOBJ->getVerse()));
        else
            ui->textEdit_trans->setText(currentTranslation.value(currentAyeOBJ->getChapter()).at(currentAyeOBJ->getVerse()-1));

        }
}


void MainWindow::setComboTrans_trans(QString l)
{
    ui->combo_trans_trans->clear();

    QList<Translation> *list=langTransList->value(l);
    Translation t;
    for(int i=0;i<list->count();i++){
        t=list->at(i);
        ui->combo_trans_trans->addItem(t.getName().replace(QChar(10),' '));//remove next line
    }

    setCurrentTranslation(ui->combo_trans_trans->currentText());

}

void MainWindow::setComboTransSearch(QString l)
{
    ui->comboBox_trans_search->clear();

    QList<Translation> *list=langTransList->value(l);
    Translation t;
    for(int i=0;i<list->count();i++){
        t=list->at(i);
        ui->comboBox_trans_search->addItem(t.getName().replace(QChar(10),' '));//remove next line
    }
    //ui->comboBox_trans_search->addItems(QStringList(*langTransList->value(l)));
}

void MainWindow::setComboTransBookmark(QString l)
{
    ui->comboBox_trans_book->clear();

    QList<Translation> *list=langTransList->value(l);
    Translation t;
    for(int i=0;i<list->count();i++){
        t=list->at(i);
        ui->comboBox_trans_book->addItem(t.getName().replace(QChar(10),' '));//remove next line
    }


    //ui->comboBox_trans_book->addItems(QStringList(*langTransList->value(l)));
}

void MainWindow::setCurrentTranslation(QString s)
{
    if(QString::compare(s,"")!=0){
        currentTranslation=db->getTranslation(langTransList->value(ui->combo_trans_lang->currentText())->at(ui->combo_trans_trans->currentIndex()));
        showTranslation();
    }
}

void MainWindow::setSizeTrans(QString s)
{


    ui->textEdit_trans->selectAll();
    ui->textEdit_trans->setFontPointSize(s.toInt());
    QTextCursor c=ui->textEdit_trans->textCursor();
    c.clearSelection();
    ui->textEdit_trans->setTextCursor(c);
    translationFontSize=s;

}

void MainWindow::choosetransColor()
{
    QColorDialog color;
    setColorTrans(color.getColor());

}

void MainWindow::handleDownloadReport(QString s)
{
    ui->label_download->setText(Language::translate(s));
}

void MainWindow::setColorTrans(QColor c)
{

    if(c!=QColor::Invalid)
        translatioColor=c;

    ui->textEdit_trans->setTextColor(c);
    ui->textEdit_trans->setText(ui->textEdit_trans->toPlainText());

    setSizeTrans(translationFontSize);


}
void MainWindow::createPages()
{
    int p1,p2,indexsore=1,indexaye=1;
    Page *temp=new Page();
    QList<Aye*> ayesP1,ayesP2,tempaye;
    Sore *tempsore;
    Aye *ayetemp;
    for(int i=1;i<=302;i++){
        if(i!=1){
            p1=i*2-1;
            p2=i*2;
        }
        else if(i==1){
            p1=1;
            p2=2;

        }

//        for(indexsore=indexsore;indexsore<=allQuran->count();indexsore++){

if(p1==187)
    qDebug()<<" ";
        tempsore=allQuran->value(indexsore);
            tempaye=tempsore->getAye();
            for(indexaye=indexaye;indexaye<=tempaye.count()+1;indexaye++){


                if(indexaye>tempaye.count()){
                    if(indexsore==114){
                        temp->setAyePages(ayesP1,ayesP2);
                        temp->setNumber(i);

                        ayesP1=QList<Aye*>();
                        ayesP2=QList<Aye*>();
                        pages.append(temp);
                        break;
                    }
                    indexsore++;
                    indexaye=1;
                    tempsore=allQuran->value(indexsore);
                    tempaye=tempsore->getAye();


                }
                ayetemp=tempaye.at(indexaye-1);
                if(ayetemp->getPage()>p2){
                    temp->setAyePages(ayesP1,ayesP2);
                    temp->setNumber(i);

                    ayesP1=QList<Aye*>();
                    ayesP2=QList<Aye*>();
                    pages.append(temp);
                    temp=new Page();
                    break;
                }


                if(ayetemp->getPage()==p1){
                    ayesP1.append(tempaye.at(indexaye-1));

                }
                else if(ayetemp->getPage()==p2){
                    ayesP2.append(tempaye.at(indexaye-1));
                }



            }

        //}


    }




}

void MainWindow::createJuzList()
{
    juzList=new QList<Aye*>();
    QMap<int, QMap<QString, int> > list=db->getJuz();
    QMap<QString, int> temp;


    for(int i=1;i<=list.count();i++){
        temp=list.value(i);
        juzList->append(allQuran->value(temp.value("sore"))->getAye().at(temp.value("aye")));
    }

}

void MainWindow::createBookmarkList()
{
    QList<BookmarkInfo> b=db->getBookmarks();

    bookmarks=new QList<Aye*>();

    BookmarkInfo tmp;
    for(int i=0;i<b.count();i++){
        tmp=b.at(i);
        if(tmp.getSore()!=1 && tmp.getSore()!=9)
            bookmarks->append(allQuran->value(tmp.getSore())->getAye().at(tmp.getAye()));
        else
            bookmarks->append(allQuran->value(tmp.getSore())->getAye().at(tmp.getAye()-1));
    }

}

void MainWindow::setCentrMainWindow()
{
        window=new QMainWindow(this);

        window->setMinimumSize(0,0);
        window->setParent(this);

        window->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        ui->graphicsView->setParent(window);
        ui->graphicsView->setMinimumSize(0,0);

        window->setCentralWidget(ui->graphicsView);
        QVBoxLayout *verlay=new QVBoxLayout(window);
        window->setLayout(verlay);
        ui->verticalLayout_mianwindow->addWidget(window);
        ui->dockWidget_sound_2->setTitleBarWidget(new QWidget);
        ui->dockWidget_sound_2->titleBarWidget()->setMinimumSize(0,0);

        setDockToCenterMainWindow();
}

void MainWindow::setDockToCenterMainWindow()
{

    window->setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks);
    ui->dock_bookmark_2->setParent(window);
    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dock_bookmark_2);

    ui->dock_trans->setParent(window);
    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dock_trans);


    ui->dockWidget_exam->setParent(window);
    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dockWidget_exam);


    window->tabifyDockWidget(ui->dockWidget_exam,ui->dock_trans);
    window->tabifyDockWidget(ui->dock_trans,ui->dock_bookmark_2);


    ui->dockWidget_exam->close();


}

void MainWindow::refereshScenePage()
{
    scenePage->deleteLater();
    scenePage=new ScenePage(ui->graphicsView);
    scenePage->setHiddenPage(hiddenPage);
    ui->graphicsView->setScene(scenePage);

    connect(scenePage,SIGNAL(itemClicked(int,int)),this,SLOT(clickedAye(int,int)));
    connect(scenePage,SIGNAL(clickedContext(int,int,QString)),this,
            SLOT(bookmarkHandle(int,int,QString)));
    connect(scenePage,SIGNAL(clickedContext(int,int,QString)),
            ui->treeWidget_index,SLOT(changeBackgroundColor(int,int,QString)));
}

void MainWindow::setComboSoundPanelItem()
{

    ui->comboBox_page->clear();
    ui->comboBox_sore->clear();
    ui->comboBox_juz->clear();

    for(int i=0;i<pages.count();i++){
        ui->comboBox_page->addItem(QString::number(pages.at(i)->getNumPage1()),i);
        ui->comboBox_page->addItem(QString::number(pages.at(i)->getNumPage2()),i);
    }



    for(int i=0;i<juzList->count();i++){
        ui->comboBox_juz->addItem(QString::number(juzList->at(i)->getJuz()),i);
    }

    setComboAyeItem();
    setComboQaryItem();
}

void MainWindow::setComboSore()
{
    combosChanged=true;
    int index=currentSore-1;

    ui->comboBox_sore->clear();
    if(QString::compare(Language::getCurrentLanguage(),"persian")==0 || QString::compare(Language::getCurrentLanguage(),"arabic")==0){
        for(int i=1;i<=allQuran->count();i++){
            ui->comboBox_sore->addItem(allQuran->value(i)->getName(),allQuran->value(i)->getNumber());
        }

    }
    else{
        for(int i=1;i<=allQuran->count();i++){
            ui->comboBox_sore->addItem(allQuran->value(i)->getEnName(),allQuran->value(i)->getNumber());
        }

    }
    ui->comboBox_sore->setCurrentIndex(index);
    combosChanged=false;



    index=ui->comboBox_examSore->currentIndex();
    if(index<0)
        index=0;

    combosChanged=true;
    ui->comboBox_examSore->clear();
    if(QString::compare(Language::getCurrentLanguage(),"persian")==0 || QString::compare(Language::getCurrentLanguage(),"arabic")==0){
        for(int i=1;i<=allQuran->count();i++){
            ui->comboBox_examSore->addItem(allQuran->value(i)->getName(),allQuran->value(i)->getNumber());
        }

    }
    else{
        for(int i=1;i<=allQuran->count();i++){
            ui->comboBox_examSore->addItem(allQuran->value(i)->getEnName(),allQuran->value(i)->getNumber());
        }

    }
    ui->comboBox_examSore->setCurrentIndex(index);

    combosChanged=false;
}



void MainWindow::setComboAyeItem()
{
    combosChanged=true;

    ui->comboBox_aye->clear();
    QList<Aye*> a=allQuran->value(currentSore)->getAye();
    for(int i=0;i<a.count();i++)
        ui->comboBox_aye->addItem(QString::number(a.at(i)->getVerse()));

    combosChanged=false;
}

void MainWindow::setComboQaryItem()
{
    combosChanged=true;
    int currentIndex=ui->comboBox_qary->currentIndex();
    if(currentIndex==-1)
        currentIndex=0;
    ui->comboBox_qary->clear();
    for(int i=0;i<qary->count();i++){
        ui->comboBox_qary->addItem(qary->at(i)->getName(),i);
    }

    ui->comboBox_qary->setCurrentIndex(currentIndex);

    combosChanged=false;
}

void MainWindow::setComboLangSearch()
{
    ui->comboBox_lang_search->addItems(QStringList(langTransList->keys()));
    setComboTransSearch(ui->comboBox_lang_search->currentText());
}

void MainWindow::setComboLangBookmark()
{
    ui->comboBox_lang_book->addItems(QStringList(langTransList->keys()));
    setComboTransBookmark(ui->comboBox_lang_book->currentText());
}

void MainWindow::setComboFilterIndex()
{
    ui->comboBox_filter_index->clear();
    ui->comboBox_filter_index->addItem(QIcon(":/rec/icon/019_047_box_archive.png"),Language::translate("All"));
    ui->comboBox_filter_index->addItem(QIcon(":/rec/green.png"),Language::translate("green"));
    ui->comboBox_filter_index->addItem(QIcon(":/rec/yellow.png"),Language::translate("yellow"));
    ui->comboBox_filter_index->addItem(QIcon(":/rec/red.png"),Language::translate("red"));
}

void MainWindow::setComboFilterBookmark()
{
    ui->comboBox_filter_book->clear();
    ui->comboBox_filter_book->addItem(QIcon(":/rec/icon/019_047_box_archive.png"),Language::translate("All"));
    ui->comboBox_filter_book->addItem(QIcon(":/rec/green.png"),Language::translate("green"));
    ui->comboBox_filter_book->addItem(QIcon(":/rec/yellow.png"),Language::translate("yellow"));
    ui->comboBox_filter_book->addItem(QIcon(":/rec/red.png"),Language::translate("red"));
}

void MainWindow::updateCombos()
{
    combosChanged=true;

    ui->comboBox_page->setCurrentText(QString::number(currentAyeOBJ->getPage()));
    ui->comboBox_sore->setCurrentText(currentAyeOBJ->getSoreName());
    setComboAyeItem();
    combosChanged=true;
    ui->comboBox_aye->setCurrentText(QString::number(currentAyeOBJ->getVerse()));
    ui->comboBox_juz->setCurrentText(QString::number(currentAyeOBJ->getJuz()));


    combosChanged=false;
}

void MainWindow::setCurrentStates(Aye *a)
{
    currentAyeOBJ=a;
    currentAye=a->getVerse();

    currentSore=a->getChapter();
    currentJuz=a->getJuz();

    if(a->getPage()==1 || a->getPage()==2)
        currentPage=1;
    else
        currentPage=(a->getPage()+1)/2;

    //soreDialog->setSore(allQuran->value(currentSore));
    QMetaObject::invokeMethod(soreDialog,"setSore",Qt::AutoConnection,Q_ARG(Sore*,allQuran->value(currentSore)));


}

void MainWindow::setSoreProperty()
{
//    Sore *s=allQuran->value(currentSore);
//    ui->groupBox_sorePrperty->setTitle(Language::translate("soreProperty"));
//    ui->label_name_property->setText(Language::translate("name")+":  "+s->getName());
//    ui->label_number_property->setText(Language::translate("number")+":  "+QString::number(s->getNumber()));
//    ui->label_numAye_property->setText(Language::translate("ayeNumber")+":  "+QString::number(s->getAye().count()));


}

void MainWindow::setAyeReadDate()
{
    //ui->groupBox_readAye->setTitle(Language::translate("verse")+QString::number(currentAyeOBJ->getVerse())
                                   //+","+Language::translate("chapter")+QString::number(currentAyeOBJ->getChapter()));
    if(currentAyeOBJ->getReadDate()!=0)
        ui->label_latestRead->setText(Language::translate("read")+": "
                                      +QString::number(QDate::currentDate().toJulianDay()-currentAyeOBJ->getReadDate())
                                      +Language::translate("days")+"  "+Language::translate("ago"));
    else
        ui->label_latestRead->setText(Language::translate("read")+": "+Language::translate("unclear"));
    ui->label_readCount->setText(Language::translate("readCount")+": "+QString::number(currentAyeOBJ->getReadCount()));


//    if(QString::compare(Language::getCurrentLanguage(),"persian")==0)
//        ui->toolButton_readCount->setToolTip(Language::translate("from")+" "+PersianDate::getPersianDate(QDate::fromJulianDay(setting->loadSetting("appdate").toLongLong())).getDate());
//    else
        ui->toolButton_readCount->setToolTip(Language::translate("from")+" "+QDate::fromJulianDay(setting->loadSetting("appdate").toLongLong()).toString());
}



void MainWindow::doAllWork()
{


    refereshScenePage();
    scenePage->paintPage(pages.at(currentPage-1),currentAyeOBJ->getAyeInTwoPage());
    playSound();
    showTranslation();
    updateCombos();
    //setSoreProperty();
    setAyeReadDate();
    setGraphicViewScrollPoint();

}

void MainWindow::setGraphicViewScrollPoint()
{




        AyeCordinate cord=currentAyeOBJ->getCordinate().at(0);

        scenePage->width();

        if(!ui->dockWidget_index_2->isVisible() && !ui->dockWidget_search_2->isVisible())
            ui->graphicsView->horizontalScrollBar()->setValue(ui->graphicsView->horizontalScrollBar()->maximum()/2);
        else
            ui->graphicsView->horizontalScrollBar()->setValue(cord.getXb()/1.1);
        ui->graphicsView->verticalScrollBar()->setValue(cord.getyb()/1.1);
}

void MainWindow::nextPage()
{

    if(currentAyeOBJ->getAyeInTwoPage()<=pages.at(currentPage-1)->getNumAyePage1()){
        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(pages.at(currentPage-1)->getNumAyePage1()));
    }
    else if(currentAyeOBJ->getAyeInTwoPage()>pages.at(currentPage-1)->getNumAyePage1()){
        currentPage++;

        if(currentPage>pages.count())
            currentPage=1;

        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(0));

    }




    doAllWork();

}


//for pre aye when current ayePage==1 and prePage execute
void MainWindow::prePage(int ayePage)
{
    currentPage--;
    if(currentPage<1)
        currentPage=pages.count();


    if(ayePage==1)
        setCurrentStates(pages.at(currentPage-1)->getAyesPage().first());//scenePage->paintPage(pages.at(currentPage-1),ayePage);
    else
        setCurrentStates(pages.at(currentPage-1)->getAyesPage().last());//scenePage->paintPage(pages.at(currentPage-1),pages.at(currentPage-1)->getNumAye());



    doAllWork();
}

void MainWindow::prePage()
{
    if(currentAyeOBJ->getAyeInTwoPage()>pages.at(currentPage-1)->getNumAyePage1()){
        setCurrentStates(pages.at(currentPage-1)->getAyesPage().first());
    }
    else if(currentAyeOBJ->getAyeInTwoPage()<=pages.at(currentPage-1)->getNumAyePage1()){
        currentPage--;
        if(currentPage<1)
            currentPage=pages.count();


       setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(pages.at(currentPage-1)->getNumAyePage1()));
    }


    doAllWork();
}

void MainWindow::nextAye()
{
    Aye *a;

    if(currentSore!=1 && currentSore!=9){
        a=allQuran->value(currentSore)->getAye().at(currentAye);
    }
    else{
        a=allQuran->value(currentSore)->getAye().at(currentAye-1);
    }

    if(a->getAyeInTwoPage()==pages.at(currentPage-1)->getNumAye()){
        nextPage();
    }
    else{

        if(a->getVerse()==allQuran->value(currentSore)->getAye().last()->getVerse()){
            if(currentSore==114)
                currentSore=1;
            else
                currentSore++;

            currentAye=allQuran->value(currentSore)->getAye().first()->getVerse();
        }
        else
            currentAye=a->getVerse()+1;

        if(currentSore!=1 && currentSore!=9){
            setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye));
        }
        else{
            setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye-1));
        }

        doAllWork();

    }
}

void MainWindow::nextSore()
{

    if(currentSore==allQuran->keys().last())
        currentSore=allQuran->keys().first();
    else
        currentSore++;


    setCurrentStates(allQuran->value(currentSore)->getAye().at(0));

    doAllWork();

}

void MainWindow::preSore()
{
    if(currentSore==allQuran->keys().first())
        currentSore=allQuran->keys().last();
    else
        currentSore--;


    setCurrentStates(allQuran->value(currentSore)->getAye().at(0));

    doAllWork();
}


void MainWindow::preAye()
{

    Aye *a;

    if(currentSore!=1 && currentSore!=9){
        a=allQuran->value(currentSore)->getAye().at(currentAye);
    }
    else{
        a=allQuran->value(currentSore)->getAye().at(currentAye-1);
    }

    if(a->getAyeInTwoPage()==1){
        prePage(2);
    }
    else{


        if(a->getVerse()==allQuran->value(currentSore)->getAye().at(0)->getVerse()){
            if(currentSore==1)
                currentSore=114;
            else
                currentSore--;

            currentAye=allQuran->value(currentSore)->getAye().last()->getVerse();
        }
        else{
            currentAye=a->getVerse()-1;
        }

        if(currentSore!=1 && currentSore!=9)
            setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye));
        else
            setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye-1));


        doAllWork();

    }
}

void MainWindow::nextJuz()
{
    if(ui->comboBox_juz->itemData(ui->comboBox_juz->currentIndex()).toInt()==juzList->count()-1)
        setCurrentStates(juzList->first());
    else
        setCurrentStates(juzList->at(ui->comboBox_juz->currentIndex()+1));

    doAllWork();
}

void MainWindow::preJuz()
{
    if(ui->comboBox_juz->itemData(ui->comboBox_juz->currentIndex()).toInt()==0)
        setCurrentStates(juzList->last());
    else
        setCurrentStates(juzList->at(ui->comboBox_juz->currentIndex()-1));

    doAllWork();
}

void MainWindow::nextQary()
{
    if(qary->count()-1>ui->comboBox_qary->currentIndex())
        ui->comboBox_qary->setCurrentIndex(ui->comboBox_qary->currentIndex()+1);
}

void MainWindow::preQary()
{
    if(0<ui->comboBox_qary->currentIndex())
        ui->comboBox_qary->setCurrentIndex(ui->comboBox_qary->currentIndex()-1);
}

void MainWindow::zoomPlus()
{
    ui->graphicsView->scale(0.8,1.2);
    ui->graphicsView->adjustSize();
}

void MainWindow::zoomMinus()
{
    ui->graphicsView->scale(0.5,0.8);
}

void MainWindow::clapsToolbar()
{
    if(ui->mainToolBar->toolButtonStyle()==Qt::ToolButtonIconOnly){

        ui->btn_claps->setIcon(QIcon(":/rec/icon/024_085_scale_arrow_fullscreen_expand_zoom_collapse_2.png"));
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else{
        ui->btn_claps->setIcon(QIcon(":/rec/icon/024_084_scale_arrow_fullscreen_expand_zoom_2.png"));
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

        }
}



void MainWindow::setSoundPanelDirection()
{

    ui->horizontalLayout_page->removeWidget(ui->comboBox_page);
    ui->horizontalLayout_page->removeWidget(ui->label_page_combo);
    ui->horizontalLayout_page->removeWidget(ui->toolButton_prePage);
    ui->horizontalLayout_page->removeWidget(ui->toolButton_nextPage);

    ui->horizontalLayout_sore->removeWidget(ui->comboBox_sore);
    ui->horizontalLayout_sore->removeWidget(ui->label_sore_combo);
    ui->horizontalLayout_sore->removeWidget(ui->toolButton_preSore);
    ui->horizontalLayout_sore->removeWidget(ui->toolButton_nextSore);


    ui->horizontalLayout_aye->removeWidget(ui->comboBox_aye);
    ui->horizontalLayout_aye->removeWidget(ui->label_aye_combo);
    ui->horizontalLayout_aye->removeWidget(ui->toolButton_preAye);
    ui->horizontalLayout_aye->removeWidget(ui->toolButton_nextAye);

    ui->horizontalLayout_juz->removeWidget(ui->comboBox_juz);
    ui->horizontalLayout_juz->removeWidget(ui->label_juz_combo);
    ui->horizontalLayout_juz->removeWidget(ui->toolButton_nextJuz);
    ui->horizontalLayout_juz->removeWidget(ui->toolButton_preJuz);


    ui->horizontalLayout_qary->removeWidget(ui->comboBox_qary);
    ui->horizontalLayout_qary->removeWidget(ui->label_qary_combo);
    ui->horizontalLayout_qary->removeWidget(ui->toolButton_nextQary);
    ui->horizontalLayout_qary->removeWidget(ui->toolButton_preQary);



    //ui->horizontalLayout_repeat->removeWidget(ui->spinBox_numRpeatAye);
    //ui->horizontalLayout_repeat->removeWidget(ui->label_numRepeat);

    //ui->gridLayout_volum->removeWidget(ui->label_volum);
    //ui->gridLayout_volum->removeWidget(ui->slider_volum);
    //ui->gridLayout_volum->removeItem(ui->spacer_volum);
    if(direction==Qt::RightToLeft){

        ui->horizontalLayout_page->addWidget(ui->toolButton_nextPage);
        ui->horizontalLayout_page->addWidget(ui->toolButton_prePage);
        ui->horizontalLayout_page->addWidget(ui->comboBox_page);
        ui->horizontalLayout_page->addWidget(ui->label_page_combo);

        ui->horizontalLayout_sore->addWidget(ui->toolButton_nextSore);
        ui->horizontalLayout_sore->addWidget(ui->toolButton_preSore);
        ui->horizontalLayout_sore->addWidget(ui->comboBox_sore);
        ui->horizontalLayout_sore->addWidget(ui->label_sore_combo);


        ui->horizontalLayout_aye->addWidget(ui->toolButton_nextAye);
        ui->horizontalLayout_aye->addWidget(ui->toolButton_preAye);
        ui->horizontalLayout_aye->addWidget(ui->comboBox_aye);
        ui->horizontalLayout_aye->addWidget(ui->label_aye_combo);


        ui->horizontalLayout_juz->addWidget(ui->toolButton_nextJuz);
        ui->horizontalLayout_juz->addWidget(ui->toolButton_preJuz);
        ui->horizontalLayout_juz->addWidget(ui->comboBox_juz);
        ui->horizontalLayout_juz->addWidget(ui->label_juz_combo);

        ui->horizontalLayout_qary->addWidget(ui->toolButton_nextQary);
        ui->horizontalLayout_qary->addWidget(ui->toolButton_preQary);
        ui->horizontalLayout_qary->addWidget(ui->comboBox_qary);
        ui->horizontalLayout_qary->addWidget(ui->label_qary_combo);


        //ui->horizontalLayout_repeat->addWidget(ui->spinBox_numRpeatAye);
        //ui->horizontalLayout_repeat->addWidget(ui->label_numRepeat);

        //ui->gridLayout_volum->addWidget(ui->slider_volum,0,0);
        //ui->gridLayout_volum->addWidget(ui->label_volum,0,1);
        //ui->gridLayout_volum->addItem(ui->spacer_volum,0,3);
    }
    else if(direction==Qt::LeftToRight){

        ui->horizontalLayout_page->addWidget(ui->label_page_combo);
        ui->horizontalLayout_page->addWidget(ui->comboBox_page);
        ui->horizontalLayout_page->addWidget(ui->toolButton_nextPage);
        ui->horizontalLayout_page->addWidget(ui->toolButton_prePage);

        ui->horizontalLayout_sore->addWidget(ui->label_sore_combo);
        ui->horizontalLayout_sore->addWidget(ui->comboBox_sore);
        ui->horizontalLayout_sore->addWidget(ui->toolButton_nextSore);
        ui->horizontalLayout_sore->addWidget(ui->toolButton_preSore);

        ui->horizontalLayout_aye->addWidget(ui->label_aye_combo);
        ui->horizontalLayout_aye->addWidget(ui->comboBox_aye);
        ui->horizontalLayout_aye->addWidget(ui->toolButton_nextAye);
        ui->horizontalLayout_aye->addWidget(ui->toolButton_preAye);

        ui->horizontalLayout_juz->addWidget(ui->label_juz_combo);
        ui->horizontalLayout_juz->addWidget(ui->comboBox_juz);
        ui->horizontalLayout_juz->addWidget(ui->toolButton_nextJuz);
        ui->horizontalLayout_juz->addWidget(ui->toolButton_preJuz);

        ui->horizontalLayout_qary->addWidget(ui->label_qary_combo);
        ui->horizontalLayout_qary->addWidget(ui->comboBox_qary);
        ui->horizontalLayout_qary->addWidget(ui->toolButton_nextQary);
        ui->horizontalLayout_qary->addWidget(ui->toolButton_preQary);


        //ui->horizontalLayout_repeat->addWidget(ui->label_numRepeat);
        //ui->horizontalLayout_repeat->addWidget(ui->spinBox_numRpeatAye);

        //ui->gridLayout_volum->addWidget(ui->label_volum,0,0);
        //ui->gridLayout_volum->addWidget(ui->slider_volum,0,1);
        //ui->gridLayout_volum->addItem(ui->spacer_volum,0,3);
    }
}




void MainWindow::clickedAye(const int sore,const int ayeSore)
{

    if(sore!=1 && sore!=9)
        setCurrentStates(allQuran->value(sore)->getAye().at(ayeSore));
    else
        setCurrentStates(allQuran->value(sore)->getAye().at(ayeSore-1));

    doAllWork();

}

void MainWindow::handleAyeRead(int pageFrom, int pageTo, int difference)
{

    for(int i=pageFrom;i<=pageTo;i++)
        setReadDate(i,difference);
}

void MainWindow::setReadDate(int page,int defference)
{
    int pageIndex=(page+1)/2;

    QList<Aye*> list=pages.at(pageIndex-1)->getAyesPage();

    for(int i=0;i<list.count();i++)
        if(list.at(i)->getPage()==page)
            list.at(i)->setReadDate(QDate::currentDate().toJulianDay()-defference);
}



//void MainWindow::on_pushButton_nextpage_clicked()
//{
//    nextPage();
//}

//void MainWindow::on_pushButton_previouspage_clicked()
//{
//    prePage();
//}





void MainWindow::handlePageCombo(const int index)
{

    handleCombosSoundPanel(Global::comboPage,index);

}

void MainWindow::handleSoreCombo(const int index)
{

    handleCombosSoundPanel(Global::comboSore,index);

}

void MainWindow::handleAyeCombo(const int index)
{

    handleCombosSoundPanel(Global::comboAye,index);

}

void MainWindow::handleJuzCombo(const int index)
{

    handleCombosSoundPanel(Global::comboJuz,index);

}

void MainWindow::handleQaryCombo(const int index)
{
    if(!combosChanged){
        currentQary=qary->at(index);
        playSound();
    }
}

void MainWindow::handleAddQary(Qary *q)
{


    setComboQaryItem();
    setExamCombos();

    Thread::getGlobalThread()->setFunc(&Database::updateQary,q,Global::SaveQary,Global::DBQaryThreadOP);
    Thread::getGlobalThread()->start();
}

void MainWindow::handleEditQary(Qary *q)
{
    setComboQaryItem();

    Thread::getGlobalThread()->setFunc(&Database::updateQary,q,Global::UpdateQary,Global::DBQaryThreadOP);
    Thread::getGlobalThread()->start();
}

void MainWindow::handleDeleteQary(Qary *q)
{
    for(int i=0;i<qary->count();i++){
        if(QString::compare(qary->at(i)->getName(),q->getName())==0){
            delete qary->at(i);
            qary->removeAt(i);
        }
    }
    setComboQaryItem();

    Thread::getGlobalThread()->setFunc(&Database::updateQary,q,Global::DeleteQary,Global::DBQaryThreadOP);
    Thread::getGlobalThread()->start();


}

void MainWindow::handleLanguageSetting(QString l)
{
    Language::setCurrentLanguage(l);
    direction=Language::getDirection();

    ui->tableWidget_bookmark->showBookmark();

    setSoundPanelDirection();
    setTranslateWidget();

    setComboFilterIndex();
    setComboFilterBookmark();

    setSoreProperty();
    setTextWidgets();

    setting->setTextWidgets();
    downloadControl->setTextWidgets();

    addRead->setTextWidgets();
    soreDialog->setWidgetText();
    helpDialog->setWidgetText();

    setExamWidgets();

    setAyeReadDate();

    setComboSore();


}

void MainWindow::handleDurationPlayer(int d)
{

    ui->horizontalSlider_Main->setRange(0,d);



}

void MainWindow::playSound()
{

    if(playerState==Global::BookmarkPlay || playerState==Global::ExamPlay)
        player->stop();
    playerState=Global::NormalPlay;
    setPlayerSound();

    if(!player->isStop()){
        player->stop();
        player->play();
    }
}

void MainWindow::stopedSound()
{

    //ui->pushButton_playStop->setText(Language::translate("play"));
    ui->pushButton_playStop->setIcon(QIcon(":/rec/icon/024_020_play_button_circle.png"));
    ui->pushButton_pausePlay->setEnabled(false);
    //ui->pushButton_pausePlay->setText(Language::translate("pause"));
    ui->pushButton_pausePlay->setIcon(QIcon(":/rec/icon/024_021_pause_button_circle.png"));

    //tskPause->setEnabled(false);
    //tskPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->horizontalSlider_Main->setEnabled(false);
}

void MainWindow::setPlayerSound()
{

    QString dir;
    if(QString::compare(currentQary->getType(),"dir")==0)
        dir=currentQary->getDir();
    else
        dir="Audio//"+currentQary->getDir();

    if(currentSore!=1 && currentSore!=9)
        soundPath=dir+"//"+allQuran->value(currentSore)->getAye().at(currentAye)->getAudio();
    else
        soundPath=dir+"//"+allQuran->value(currentSore)->getAye().at(currentAye-1)->getAudio();
    soundPath=soundPath+".mp3";

    player->setSoundPath(soundPath);


    QDir d(dir);
    if(!d.exists(dir))
        QDir().mkpath(dir);

    if(!QFile::exists(soundPath) && QString::compare(currentQary->getType(),"dir")!=0){
        //threadFunc->setFunc(&downloadSound);

        downloadSound();
    }
    else
        downloader->report("");


}

void MainWindow::endSoundHandle()
{
    if(playerState==Global::BookmarkPlay){
        player->stop();
        playerState=Global::NormalPlay;

        endBookmarkPlay();
    }
    else if(playerState==Global::NormalPlay){
        if(repeatState==Global::NoRepeat)
            nextAye();

        else if(repeatState==Global::PageRepeat){
            if(currentAyeOBJ->getPage()%2==0){


                if(counterRepeat<numRepeat){
                    counterRepeat++;
                    if(currentAyeOBJ->getAyeInTwoPage()==pages.at(currentPage-1)->getNumAye())
                        setCurrentStates(pages.at(currentPage-1)->
                                         getAyesPage().at(pages.at(currentPage-1)->getNumAyePage1()));
                    else
                        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(currentAyeOBJ->getAyeInTwoPage()));//شماره آیه قبلی میشه ایندکس بعدی

                    doAllWork();
                }
                else if(counterRepeat==numRepeat){
                    counterRepeat=0;
                    nextPage();
                }

            }
            else{

                if(counterRepeat<numRepeat){
                    counterRepeat++;
                    if(currentAyeOBJ->getAyePage()==pages.at(currentPage-1)->getNumAyePage1())
                        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(0));
                    else
                        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(currentAyeOBJ->getAyeInTwoPage()));//شماره آیه قبلی میشه ایندکس بعدی

                    doAllWork();
                }
                else if(counterRepeat==numRepeat){
                    counterRepeat=0;
                    nextPage();
                }
            }
        }


        else if(repeatState==Global::LimitRepeat){
            if(counterRepeat<numRepeat){
                counterRepeat++;
                doAllWork();
            }
            else if(counterRepeat==numRepeat){
                counterRepeat=0;
                nextAye();
            }
        }

        else if(repeatState==Global::InfinityRepeat){
            doAllWork();
        }
    }


}

void MainWindow::handlePlayerPosition(int v)
{
    if(playerState==Global::NormalPlay)
        ui->horizontalSlider_Main->setValue(v);
        if(playerState==Global::ExamPlay)
            checkExamDuration(v);
}

void MainWindow::handleChangeValueMainSlider(int v)
{
    if(playerState==Global::NormalPlay)
        player->setPosition(v);
}



void MainWindow::handleCombosSoundPanel(Global::combosSignals cs, int index)
{


    if(cs==Global::comboPage && !combosChanged){

        int ayeInTwoPage;
        currentPage=ui->comboBox_page->itemData(index).toInt()+1;

        if(ui->comboBox_page->itemText(index).toInt()%2==0)
            ayeInTwoPage=pages.at(currentPage-1)->getNumAyePage1()+1;
        else
            ayeInTwoPage=1;
        setCurrentStates(pages.at(currentPage-1)->getAyesPage().at(ayeInTwoPage-1));

        doAllWork();

    }
    else if(cs==Global::comboSore && !combosChanged){


        int ayeInCombo=ui->comboBox_aye->currentText().toInt();

        currentSore=ui->comboBox_sore->itemData(index).toInt();

        if(allQuran->value(currentSore)->getAye().count()>ayeInCombo && ayeInCombo>0){
            if(currentSore!=1 && currentSore!=9)
                setCurrentStates(allQuran->value(currentSore)->getAye().at(ayeInCombo));
            else
                setCurrentStates(allQuran->value(currentSore)->getAye().at(ayeInCombo-1));
        }
        else
            setCurrentStates(allQuran->value(currentSore)->getAye().at(0));
        setComboAyeItem();



        doAllWork();

    }

    else if(cs==Global::comboAye && !combosChanged){
        combosChanged=true;
        currentAye=ui->comboBox_aye->currentText().toInt();
        if(currentSore!=1 && currentSore!=9)
            setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye));
        else
        setCurrentStates(allQuran->value(currentSore)->getAye().at(currentAye-1));

        doAllWork();

    }
    else if(cs==Global::comboJuz && !combosChanged){


        setCurrentStates(juzList->at(ui->comboBox_juz->itemData(index).toInt()));

        doAllWork();

    }


}




void MainWindow::on_pushButton_playStop_clicked()
{

    if(player->isStop() || playerState==Global::BookmarkPlay || playerState==Global::ExamPlay){
        if(playerState==Global::BookmarkPlay)
            emit endBookmarkPlay();

        playerState=Global::NormalPlay;
        setPlayerSound();
        player->play();
        //ui->pushButton_playStop->setText(Language::translate("stop"));
        ui->pushButton_playStop->setIcon(QIcon(":/rec/icon/024_022_stop_button_circle.png"));
        ui->pushButton_pausePlay->setEnabled(true);

        //tskPause->setEnabled(true);
        //tskPlay->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

        ui->horizontalSlider_Main->setEnabled(true);
    }
    else if(player->isPause() || player->isPlay()){
        player->stop();
        stopedSound();
    }

}

void MainWindow::on_pushButton_pausePlay_clicked()
{
    if(player->isPlay()){
        player->pause();
        //ui->pushButton_pausePlay->setText(Language::translate("play"));
        ui->pushButton_pausePlay->setIcon(QIcon(":/rec/icon/024_020_play_button_circle.png"));

        //tskPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else if(player->isPause()){
        player->play();
        //ui->pushButton_pausePlay->setText(Language::translate("pause"));
        ui->pushButton_pausePlay->setIcon(QIcon(":/rec/icon/024_021_pause_button_circle.png"));

        //tskPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}




void MainWindow::on_horizontalSlider_Main_sliderMoved(int position)
{
    player->setPosition(position);
}









void MainWindow::downloadSound()
{
    QList<DownloadInfo*> list;

    if(playerState==Global::BookmarkPlay){
        list.append(new DownloadInfoQuran(currentQary,bookmarkPlaying));
        downloader->addDownload(list);
    }
    else if(playerState==Global::ExamPlay){
        list.append(new DownloadInfoQuran(qary->at(ui->comboBox_examQary->currentIndex()),examAye));
        downloader->addDownload(list);
    }
    else if(downloadStat==Global::DownloadAye){
        if(currentSore!=1 && currentSore!=9)
            list.append(new DownloadInfoQuran(currentQary,allQuran->value(currentSore)->getAye().at(currentAye)));
        else
            list.append(new DownloadInfoQuran(currentQary,allQuran->value(currentSore)->getAye().at(currentAye-1)));
        downloader->addDownload(list);
    }
    else if(downloadStat==Global::DownloadPage){
        downloader->addDownload(createDownloadPageList());
    }
    else if(downloadStat==Global::DownloadSore){
        downloader->addDownload(createDownloadSoreList());
    }
    else if(downloadStat==Global::downloadOff){
        downloader->report("downloadOff");
    }
}





QList<DownloadInfo *> MainWindow::createDownloadPageList()
{
    QList<DownloadInfo *> list;
    QList<Aye*> a=pages.at(currentPage-1)->getAyesPage();

    for(int i=0;i<a.count();i++){
        list.append(new DownloadInfoQuran(currentQary,a.at(i)));
        if(QFile::exists(list.last()->getSavePath()))
            list.removeLast();
    }

    return list;
}

QList<DownloadInfo *> MainWindow::createDownloadSoreList()
{
    QList<DownloadInfo *> list;
    QList<Aye*> a=allQuran->value(currentSore)->getAye();

    for(int i=0;i<a.count();i++){
        list.append(new DownloadInfoQuran(currentQary,a.at(i)));
        if(QFile::exists(list.last()->getSavePath()))
            list.removeLast();
    }

    return list;
}

void MainWindow::handleDownloadedFile(DownloadInfo *d)
{

//    Aye *a=downloadedFile->getAye();
//    Qary *q=downloadedFile->getQary();

//    if(QString::compare(q->getName(),currentQary->getName())==0,
//            currentAyeOBJ->getChapter()==a->getChapter(),
//            currentAyeOBJ->getVerse()==a->getVerse() )
//    {

    if(QString::compare(d->getSavePath(),soundPath)==0)
        doAllWork();
   // }

    //delete downloadedFile;
}

void MainWindow::setDownloadState(Global::DownloadState s)
{
    downloadStat=s;
}

void MainWindow::bookmarkHandle(int sore, int aye, QString color)
{


    if(QString::compare(color,"copy")==0){
        QClipboard *clipboard=QApplication::clipboard();
        if(sore!=9 && sore!=1)
            clipboard->setText(allQuran->value(sore)->getAye().at(aye)->getText());
        else
            clipboard->setText(allQuran->value(sore)->getAye().at(aye-1)->getText());

    }
    else if(QString::compare(color,"read")==0){
        if(sore!=9 && sore!=1){
            allQuran->value(sore)->getAye().at(aye)->setReadDate(QDate::currentDate().toJulianDay());
            Thread::getGlobalThread()->setFunc(Database::saveReadStatus,allQuran->value(sore)->getAye().at(aye),Global::DBReadAye);
            Thread::getGlobalThread()->start();
        }
        else{
            allQuran->value(sore)->getAye().at(aye-1)->setReadDate(QDate::currentDate().toJulianDay());

            Thread::getGlobalThread()->setFunc(Database::saveReadStatus,allQuran->value(sore)->getAye().at(aye-1),Global::DBReadAye);
            Thread::getGlobalThread()->start();

        }

    }

    else{
        Aye *a=isExistBookmark(sore,aye);


        if(a==NULL){
            if(QString::compare(color,"delete")!=0){

                if(sore!=9 && sore!=1)
                    a=allQuran->value(sore)->getAye().at(aye);
                else
                    a=allQuran->value(sore)->getAye().at(aye-1);
                a->setColor(color);

                bookmarks->append(a);

                Thread::getGlobalThread()->setFunc(&Database::updateBookmark,a,
                                                   Global::SaveBookmark,Global::DBBookMarkThreadOP);
                Thread::getGlobalThread()->start();

            }


        }
        else{
            if(QString::compare(color,"delete")==0){
                bookmarks->removeOne(a);
                a->setColor("blue");

                Thread::getGlobalThread()->setFunc(&Database::updateBookmark,a,
                                                   Global::DeleteBookmark,Global::DBBookMarkThreadOP);
                Thread::getGlobalThread()->start();
            }

            else{
                a->setColor(color);
                Thread::getGlobalThread()->setFunc(&Database::updateBookmark,a,
                                                   Global::UpdateBookmark,Global::DBBookMarkThreadOP);
                Thread::getGlobalThread()->start();
            }



        }

    }

    ui->tableWidget_bookmark->showBookmark();


    refereshScenePage();
    scenePage->paintPage(pages.at(currentPage-1),currentAyeOBJ->getAyeInTwoPage());
    if(!player->isPlay())
        playSound();
    showTranslation();
    updateCombos();
    setSoreProperty();
    setAyeReadDate();
    setGraphicViewScrollPoint();

}

void MainWindow::itemsClickHandle(Aye *a)
{
    setCurrentStates(a);
    doAllWork();
}



void MainWindow::playBookmark(Aye *a)
{
    if(player->isPlay() && playerState==Global::BookmarkPlay)
    {

        if(a->getChapter()==bookmarkPlaying->getChapter()
                && a->getVerse()==bookmarkPlaying->getVerse()){
            player->stop();

            emit endBookmarkPlay();
        }
        else{
            playerState=Global::BookmarkPlay;
            player->stop();
            stopedSound();

            bookmarkPlaying=a;

            QString dir;
            if(QString::compare(currentQary->getType(),"dir")==0)
                dir=currentQary->getDir();
            else
                dir="Audio//"+currentQary->getDir();

            if(bookmarkPlaying->getChapter()!=1 && bookmarkPlaying->getChapter()!=9)
                soundPath=dir+"//"+allQuran->value(bookmarkPlaying->getChapter())->getAye().at(bookmarkPlaying->getVerse())->getAudio();
            else
                soundPath=dir+"//"+allQuran->value(bookmarkPlaying->getChapter())->getAye().at(bookmarkPlaying->getVerse()-1)->getAudio();
            soundPath=soundPath+".mp3";
            player->setSoundPath(soundPath);

            if(QFile::exists(soundPath))
                player->play();
            else if(QString::compare(currentQary->getType(),"dir")!=0){
                downloadSound();

                playerState=Global::NormalPlay;
                emit endBookmarkPlay();
            }
        }
    }
    else{
        playerState=Global::BookmarkPlay;
        player->stop();
        stopedSound();

        bookmarkPlaying=a;

        QString dir;
        if(QString::compare(currentQary->getType(),"dir")==0)
            dir=currentQary->getDir();
        else
            dir="Audio//"+currentQary->getDir();

        if(bookmarkPlaying->getChapter()!=1 && bookmarkPlaying->getChapter()!=9)
            soundPath=dir+"//"+allQuran->value(bookmarkPlaying->getChapter())->getAye().at(bookmarkPlaying->getVerse())->getAudio();
        else
            soundPath=dir+"//"+allQuran->value(bookmarkPlaying->getChapter())->getAye().at(bookmarkPlaying->getVerse()-1)->getAudio();
        soundPath=soundPath+".mp3";
        player->setSoundPath(soundPath);

        if(QFile::exists(soundPath))
            player->play();
        else if(QString::compare(currentQary->getType(),"dir")!=0){
            downloadSound();

            playerState=Global::NormalPlay;
            emit endBookmarkPlay();
        }
    }


}

Aye* MainWindow::isExistBookmark(int sore, int aye)
{
    for(int i=0;i<bookmarks->count();i++){
        if(bookmarks->at(i)->getChapter()==sore && bookmarks->at(i)->getVerse()==aye)
            return bookmarks->at(i);
    }

    return NULL;
}


void MainWindow::on_btn_setting_triggered()
{

    setting->show();

}




void MainWindow::on_toolButton_download_clicked()
{
    downloadControl->show();
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->treeWidget_search->search(arg1);
}

void MainWindow::on_lineEdit_search_index_textChanged(const QString &arg1)
{
    ui->treeWidget_index->search(arg1);
    ui->comboBox_filter_index->setCurrentIndex(0);
}


void MainWindow::getExport(QList<Aye *> list, QString fileName,Translation trans)
{


     if(list.count()!=0){


             QFile file(fileName+".rtf");
             if(!file.exists()){
                 file.open(QIODevice::WriteOnly);
                 QTextStream text(&file);
                 text.setCodec("UTF-8");
                 text.setFieldAlignment(QTextStream::AlignRight);

                 for(int i=0;i<list.count();i++){

                     text<<list.at(i)->getSoreName()<<"   "<<list.at(i)->getVerse();
                     text<<"\r\n"<<list.at(i)->getText();
                     if(QString::compare(trans.getLanguage(),"")!=0)
                         text<<"\r\n"<<Database::getTranslation(trans,list.at(i));

                     text<<"\r\n\r\n";
                 }

             file.close();
             }

             else{
                 qDebug()<<"Ffff";
             }
     }
}



void MainWindow::on_pushButton_export_search_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptSave);
    QString fileName=fd.getSaveFileName();
    if(!fileName.isEmpty()){
        if(ui->checkBox_langTrans_search->isChecked())
        Thread::getGlobalThread()->setFunc(getExport,ui->treeWidget_search->getSearchResult()
                                           ,fileName,langTransList->value(ui->comboBox_lang_search->currentText())->at(ui->comboBox_trans_search->currentIndex())
                                           ,Global::Export);
        else
            Thread::getGlobalThread()->setFunc(getExport,ui->treeWidget_search->getSearchResult()
                                               ,fileName,Translation("",""),Global::Export);
        Thread::getGlobalThread()->start();
    }

}

void MainWindow::on_pushButton_export_book_clicked()
{
    QString filter=Language::deTranslate(ui->comboBox_filter_book->currentText());
    QList<Aye*> exportList;
    if(QString::compare(filter,"All")==0)
        exportList=*bookmarks;
    else{
        for(int i=0;i<bookmarks->count();i++)
            if(QString::compare(bookmarks->at(i)->getColor(),filter)==0)
                exportList.append(bookmarks->at(i));
    }

    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptSave);
    QString fileName=fd.getSaveFileName();
    if(!fileName.isEmpty()){
        if(ui->checkBox_langTrans_book->isChecked())
        Thread::getGlobalThread()->setFunc(getExport,exportList
                                           ,fileName,langTransList->value(ui->comboBox_lang_book->currentText())->at(ui->comboBox_trans_book->currentIndex())
                                           ,Global::Export);
        else
            Thread::getGlobalThread()->setFunc(getExport,exportList
                                               ,fileName,Translation("",""),Global::Export);
        Thread::getGlobalThread()->start();
    }
}

void MainWindow::on_checkBox_langTrans_search_clicked(bool checked)
{
    if(checked){
        ui->comboBox_lang_search->setEnabled(true);
        ui->comboBox_trans_search->setEnabled(true);
    }
    else{
        ui->comboBox_lang_search->setEnabled(false);
        ui->comboBox_trans_search->setEnabled(false);
    }
}

//void MainWindow::on_toolButton_aye_next_clicked()
//{
//    nextAye();
//}

void MainWindow::on_toolButton_forward_clicked()
{
    player->forward();
}

void MainWindow::on_toolButton_backward_clicked()
{
    player->backward();
}

//void MainWindow::on_toolButton_pre_aye_clicked()
//{
//    preAye();
//}




// move volum slider by wheel of mouse
void MainWindow::wheelEvent(QWheelEvent *event){
    if(event->angleDelta().y()<0 && ui->slider_volum->value()>0){
        ui->slider_volum->setValue(ui->slider_volum->value()-3);
    }
    else if(event->angleDelta().y()>0 && ui->slider_volum->value()<100){
        ui->slider_volum->setValue(ui->slider_volum->value()+3);
    }
}

void MainWindow::closeEvent(QCloseEvent *e){

    saveSetting();

    close();
}

void MainWindow::on_pushButton_zoom_plus_clicked()
{
    zoomPlus();
}

void MainWindow::on_pushButton_zoom_minus_clicked()
{
    zoomMinus();
}

void MainWindow::on_btn_claps_triggered()
{
    clapsToolbar();

}

void MainWindow::on_btn_index_triggered(bool checked)
{
    if(checked)
        ui->dockWidget_index_2->setHidden(false);
    else
        ui->dockWidget_index_2->setHidden(true);
}

void MainWindow::on_btn_translate_triggered(bool checked)
{
    if(checked)
        ui->dock_trans->setHidden(false);
    else
        ui->dock_trans->setHidden(true);
}

void MainWindow::on_btn_search_triggered(bool checked)
{
    if(checked)
        ui->dockWidget_search_2->setHidden(false);
    else
        ui->dockWidget_search_2->setHidden(true);
}

void MainWindow::on_btn_bookmark_triggered(bool checked)
{
    if(checked)
        ui->dock_bookmark_2->setHidden(false);
    else
        ui->dock_bookmark_2->setHidden(true);
}

void MainWindow::on_btn_exam_triggered(bool checked)
{
    if(checked)
        ui->dockWidget_exam->setHidden(false);
    else
        ui->dockWidget_exam->setHidden(true);
}



void MainWindow::on_checkBox_langTrans_book_clicked(bool checked)
{
    if(checked){
        ui->comboBox_lang_book->setEnabled(true);
        ui->comboBox_trans_book->setEnabled(true);
    }
    else{
        ui->comboBox_lang_book->setEnabled(false);
        ui->comboBox_trans_book->setEnabled(false);
    }
}




void MainWindow::on_comboBox_filter_index_currentTextChanged(const QString &arg1)
{
    ui->treeWidget_index->showItemsByFilter(Language::deTranslate(arg1));
}



void MainWindow::on_pushButton_hidden_clicked()
{
    refereshScenePage();
    if(hiddenPage){
        hiddenPage=false;
        scenePage->setHiddenPage(false);
        //ui->pushButton_hidden->setText(Language::translate("hidden"));
    }
    else{
        hiddenPage=true;
        scenePage->setHiddenPage(true);
       // ui->pushButton_hidden->setText(Language::translate("noHidden"));
    }

    scenePage->paintPage(pages.at(currentPage-1),currentAyeOBJ->getAyeInTwoPage());
}




void MainWindow::setShortcuts()
{

    //index and bookmark set on designer


    QShortcut *nextPageShortcut=new QShortcut(Qt::Key_Left,this);
    QShortcut *prevPageShortcut=new QShortcut(Qt::Key_Right,this);
    QShortcut *tenNextPageShortcut=new QShortcut(Qt::CTRL+Qt::Key_Left,this);
    QShortcut *tenPrevPageShortcut=new QShortcut(Qt::CTRL+Qt::Key_Right,this);
    QShortcut *nextAyeShortcut=new QShortcut(Qt::Key_Down,this);
    QShortcut *prevAyeShortcut=new QShortcut(Qt::Key_Up,this);
    QShortcut *fullScreenShortcut=new QShortcut(Qt::Key_F,this);
    QShortcut *noFullScreenShortcut=new QShortcut(Qt::Key_Escape,this);
    QShortcut *showSettingShortcut=new QShortcut(Qt::CTRL+Qt::Key_P,this);
    QShortcut *stopAudioShortcut=new QShortcut(Qt::Key_0,this);
    QShortcut *pauseAndPlaySettingShortcut=new QShortcut(Qt::Key_Space,this);

    QShortcut *nextSoreShortcut=new QShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_Left,this);
    QShortcut *preSoreShortcut=new QShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_Right,this);




    connect(nextPageShortcut,SIGNAL(activated()),this,SLOT(nextPage()));
    connect(prevPageShortcut,SIGNAL(activated()),this,SLOT(prePage()));
    connect(tenNextPageShortcut,SIGNAL(activated()),this,SLOT(activeTenNextPageShortcut()));
    connect(tenPrevPageShortcut,SIGNAL(activated()),this,SLOT(activeTenPrevPageShortcut()));
    connect(fullScreenShortcut,SIGNAL(activated()),this,SLOT(activeFullScreenShortcut()));
    connect(noFullScreenShortcut,SIGNAL(activated()),this,SLOT(showMaximized()));
    connect(pauseAndPlaySettingShortcut,SIGNAL(activated()),this,SLOT(on_pushButton_pausePlay_clicked()));
    connect(stopAudioShortcut,SIGNAL(activated()),this,SLOT(on_pushButton_playStop_clicked()));
    connect(nextAyeShortcut,SIGNAL(activated()),this,SLOT(nextAye()));
    connect(prevAyeShortcut,SIGNAL(activated()),this,SLOT(preAye()));

    connect(showSettingShortcut,SIGNAL(activated()),this,SLOT(on_btn_setting_triggered()));

    connect(nextSoreShortcut,SIGNAL(activated()),this,SLOT(nextSore()));
    connect(preSoreShortcut,SIGNAL(activated()),this,SLOT(preSore()));


}


void MainWindow::activeFullScreenShortcut(){
    if(isFullScreen()){
        showNormal();
        adjustSize();
        showMaximized();
    }
    else
        showFullScreen();
}


void MainWindow::activeTenNextPageShortcut(){
    for(int i=0;i<10;i++){
        currentPage++;
        if(currentPage>pages.count())
            currentPage=1;
    }


    setCurrentStates(allQuran->value(currentSore)->getAye().at(0));

    doAllWork();
}

void MainWindow::activeTenPrevPageShortcut(){
    for(int i=10;i>0;i--){
        currentPage--;
        if(currentPage<1)
            currentPage=pages.count();
    }
    scenePage->paintPage(pages.at(currentPage-1),1);
}


void MainWindow::setTextWidgets(){
    this->setWindowTitle(Language::translate("abyaz"));
    ui->label_page_combo->setText(Language::translate("page"));
    ui->label_sore_combo->setText(Language::translate("sore"));
    ui->label_aye_combo->setText(Language::translate("aye"));
    ui->label_qary_combo->setText(Language::translate("qary"));
    ui->label_juz_combo->setText(Language::translate("juz"));

    //ui->label_volum->setText(Language::translate("volume"));
    //ui->label_numRepeat->setText(Language::translate("numReapeat"));

    ui->pushButton_infinityRepeat->setText(Language::translate("infinityRepeat"));
    ui->pushButton_limitRepeat->setText(Language::translate("limitRepeat"));
    ui->pushButton_pageRepeat->setText(Language::translate("pageRepeat"));

//    ui->pushButton_hidden->setText(Language::translate("hidden"));
//    ui->pushButton_nextpage->setText(Language::translate("nextPage"));
//    ui->pushButton_previouspage->setText(Language::translate("previousPage"));

//    if(player->isPlay()){
//        ui->pushButton_pausePlay->setText(Language::translate("pause"));
//        ui->pushButton_playStop->setText(Language::translate("stop"));
//    }
//    else if(player->isPause()){
//        ui->pushButton_pausePlay->setText(Language::translate("play"));
//        ui->pushButton_playStop->setText(Language::translate("stop"));
//    }
//    else if(player->isStop()){
//        ui->pushButton_pausePlay->setText(Language::translate("pause"));
//        ui->pushButton_playStop->setText(Language::translate("play"));
//    }


    ui->dockWidget_index_2->setWindowTitle(Language::translate("index"));
    ui->dockWidget_search_2->setWindowTitle(Language::translate("search"));
    ui->dock_bookmark_2->setWindowTitle(Language::translate("bookmark"));
    ui->dock_trans->setWindowTitle(Language::translate("translation"));


    ui->pushButton_export_search->setText(Language::translate("export"));
    ui->checkBox_langTrans_search->setText(Language::translate("translation"));
    ui->groupBox_search->setTitle(Language::translate("export"));

    ui->pushButton_export_book->setText(Language::translate("export"));
    ui->checkBox_langTrans_book->setText(Language::translate("translation"));



    ui->label_trans_lang->setText(Language::translate("language"));
    ui->label_trans_trans->setText(Language::translate("translator"));
    ui->label_trans_size->setText(Language::translate("size"));
    ui->button_trans_color->setText(Language::translate("color"));


    ui->btn_bookmark->setText(Language::translate("bookmark"));
    ui->btn_claps->setText(Language::translate("claps"));
    //ui->btn_exit->setText(Language::translate("exit"));
    ui->btn_help->setText(Language::translate("help"));
    ui->btn_index->setText(Language::translate("index"));
    ui->btn_search->setText(Language::translate("search"));
    ui->btn_setting->setText(Language::translate("setting"));
    ui->btn_translate->setText(Language::translate("translation"));
    ui->btn_exam->setText(Language::translate("exam"));
    ui->btn_fullscreen->setText(Language::translate("fullscreen"));


    ui->label_download->setText(Language::translate(Language::deTranslate(ui->label_download->text())));



    ui->dockWidget_exam->setWindowTitle(Language::translate("exam"));
    ui->groupBox_examType->setTitle(Language::translate("examType"));
    ui->radioButton_examAye->setText(Language::translate("aye"));
    ui->radioButton_examPage->setText(Language::translate("page"));
    ui->radioButton_examSound->setText(Language::translate("sound"));
    ui->radioButton_examText->setText(Language::translate("text"));

    ui->groupBox_examLimit->setTitle(Language::translate("filter"));
    ui->checkBox_examLimitAye->setText(Language::translate("aye"));
    ui->checkBox_examLimitPage->setText(Language::translate("page"));
    ui->checkBox_examLimitSore->setText(Language::translate("sore"));

    ui->label_examFromPage->setText(Language::translate("from"));
    ui->label_examToPage->setText(Language::translate("to"));

    ui->label_examFromAye->setText(Language::translate("from"));
    ui->label_examToAye->setText(Language::translate("to"));

    ui->label_qary_exam->setText(Language::translate("qary"));


}

void MainWindow::on_btn_fullscreen_triggered()
{
    activeFullScreenShortcut();
}


void MainWindow::on_pushButton_pageRepeat_clicked(bool checked)
{
    if(checked){
        repeatState=Global::PageRepeat;
        ui->pushButton_infinityRepeat->setChecked(false);
        ui->pushButton_limitRepeat->setChecked(false);
        ui->spinBox_numRpeatAye->setEnabled(false);
        ui->spinBox_numRpeatPage->setEnabled(true);

        numRepeat=ui->spinBox_numRpeatPage->value();
        counterRepeat=0;
    }
    else{
        repeatState=Global::NoRepeat;
        ui->spinBox_numRpeatPage->setEnabled(false);
    }
}

void MainWindow::on_pushButton_infinityRepeat_clicked(bool checked)
{
//    if(checked){
//        repeatState=Global::InfinityRepeat;
//        ui->pushButton_pageRepeat->setChecked(false);
//        ui->pushButton_limitRepeat->setChecked(false);
//        ui->spinBox_numRpeatAye->setEnabled(false);
//        ui->spinBox_numRpeatPage->setEnabled(false);
//    }
//    else{
//        repeatState=Global::NoRepeat;
//    }
}

void MainWindow::on_pushButton_limitRepeat_clicked(bool checked)
{
    if(checked){
        repeatState=Global::LimitRepeat;
        numRepeat=ui->spinBox_numRpeatAye->value();
        counterRepeat=0;
        ui->spinBox_numRpeatAye->setEnabled(true);
        ui->pushButton_pageRepeat->setChecked(false);
        ui->pushButton_infinityRepeat->setChecked(false);
        ui->spinBox_numRpeatPage->setEnabled(false);
    }
    else{
        repeatState=Global::NoRepeat;
        ui->spinBox_numRpeatAye->setEnabled(false);

    }
}

void MainWindow::on_spinBox_numRpeatAye_valueChanged(int arg1)
{
    numRepeat=arg1;
}

void MainWindow::on_toolButton_volum_up_clicked()
{
    player->setVolum(player->getVolum()+10);
}

void MainWindow::on_toolButton_volum_down_clicked()
{
    player->setVolum(player->getVolum()-10);
}

void MainWindow::on_toolButton_prePage_clicked()
{
    prePage();
}

void MainWindow::on_toolButton_nextPage_clicked()
{
    nextPage();
}

void MainWindow::on_toolButton_preSore_clicked()
{
    preSore();
}

void MainWindow::on_toolButton_nextSore_clicked()
{
    nextSore();
}

void MainWindow::on_toolButton_preAye_clicked()
{
    preAye();
}

void MainWindow::on_toolButton_nextAye_clicked()
{
    nextAye();
}

void MainWindow::on_toolButton_preJuz_clicked()
{
    preJuz();
}

void MainWindow::on_toolButton_nextJuz_clicked()
{
    nextJuz();
}

void MainWindow::on_toolButton_readDate_clicked()
{
    addRead->show();
}


void MainWindow::setExamCombos()
{
    ui->comboBox_examFromPage->clear();
    ui->comboBox_examToPage->clear();
    for(int i=0;i<pages.count();i++){
        ui->comboBox_examFromPage->addItem(QString::number(pages.at(i)->getNumPage1()),i);
        ui->comboBox_examFromPage->addItem(QString::number(pages.at(i)->getNumPage2()),i);

        ui->comboBox_examToPage->addItem(QString::number(pages.at(i)->getNumPage1()),i);
        ui->comboBox_examToPage->addItem(QString::number(pages.at(i)->getNumPage2()),i);
    }
    ui->comboBox_examQary->clear();
    for(int i=0;i<qary->count();i++)
        ui->comboBox_examQary->addItem(qary->at(i)->getName(),i);

    combosChanged=true;
    ui->comboBox_examSore->clear();
    for(int i=allQuran->keys().first();i<=allQuran->keys().last();i++)
        ui->comboBox_examSore->addItem(allQuran->value(i)->getName(),i);
    combosChanged=false;

    setExamAyeCombos();
}

void MainWindow::setExamAyeCombos()
{
    if(!combosChanged){
        QList<Aye*> list=allQuran->value(ui->comboBox_examSore->currentData().toInt())->getAye();

        ui->comboBox_examFromAye->clear();
        ui->comboBox_examToAye->clear();
        for(int i=0;i<list.count();i++){
            ui->comboBox_examFromAye->addItem(QString::number(list.at(i)->getVerse()),i);
            ui->comboBox_examToAye->addItem(QString::number(list.at(i)->getVerse()),i);
        }
    }

}

void MainWindow::setExamWidgets()
{
    ui->horizontalLayout_examPage->removeWidget(ui->label_examFromPage);
    ui->horizontalLayout_examPage->removeWidget(ui->comboBox_examFromPage);
    ui->horizontalLayout_examPage->removeWidget(ui->label_examToPage);
    ui->horizontalLayout_examPage->removeWidget(ui->comboBox_examToPage);

    ui->horizontalLayout_examAye->removeWidget(ui->label_examFromAye);
    ui->horizontalLayout_examAye->removeWidget(ui->comboBox_examFromAye);
    ui->horizontalLayout_examAye->removeWidget(ui->label_examToAye);
    ui->horizontalLayout_examAye->removeWidget(ui->comboBox_examToAye);

    ui->horizontalLayout_examQary->removeWidget(ui->label_qary_exam);
    ui->horizontalLayout_examQary->removeWidget(ui->comboBox_examQary);

    if(direction==Qt::LeftToRight){

        ui->horizontalLayout_examPage->addWidget(ui->label_examFromPage);
        ui->horizontalLayout_examPage->addWidget(ui->comboBox_examFromPage);
        ui->horizontalLayout_examPage->addWidget(ui->label_examToPage);
        ui->horizontalLayout_examPage->addWidget(ui->comboBox_examToPage);


        ui->horizontalLayout_examAye->addWidget(ui->label_examFromAye);
        ui->horizontalLayout_examAye->addWidget(ui->comboBox_examFromAye);
        ui->horizontalLayout_examAye->addWidget(ui->label_examToAye);
        ui->horizontalLayout_examAye->addWidget(ui->comboBox_examToAye);



        ui->horizontalLayout_examQary->addWidget(ui->comboBox_examQary);
        ui->horizontalLayout_examQary->addWidget(ui->label_qary_exam);



    }
    else if(direction==Qt::RightToLeft){
        ui->horizontalLayout_examPage->addWidget(ui->comboBox_examToPage);
        ui->horizontalLayout_examPage->addWidget(ui->label_examToPage);
        ui->horizontalLayout_examPage->addWidget(ui->comboBox_examFromPage);
        ui->horizontalLayout_examPage->addWidget(ui->label_examFromPage);



        ui->horizontalLayout_examAye->addWidget(ui->comboBox_examToAye);
        ui->horizontalLayout_examAye->addWidget(ui->label_examToAye);
        ui->horizontalLayout_examAye->addWidget(ui->comboBox_examFromAye);
        ui->horizontalLayout_examAye->addWidget(ui->label_examFromAye);


        ui->horizontalLayout_examQary->addWidget(ui->label_qary_exam);
        ui->horizontalLayout_examQary->addWidget(ui->comboBox_examQary);


    }

}

Aye* MainWindow::pageFilterExam(int from,int to)
{
    //from>=1  && to>=1


    QList<Aye*> list;
    Aye *a;

    if(from==to){
        if(from%2==0)
            list=pages.at((from+1)/2-1)->getAyePage2();
        else
            list=pages.at((from+1)/2-1)->getAyePage1();

        a=list.at(getExamIndexAye(list,-1,-1));
        if(ui->radioButton_examPage->isChecked()){

            return list.first();
        }

        return a;
    }
    else if(from<to){
        if(ui->radioButton_examPage->isChecked()){
            list=getFirstAyePages(from,to);
            a=list.at(getExamIndexAye(list,-1,-1));
            return a;
        }
        else{
//            int resRand=(rand()%(to-from+1));
//            resRand=resRand+from;
//            if(resRand%2==0)
//                list=pages.at((resRand+1)/2-1)->getAyePage2();
//            else
//                list=pages.at((resRand+1)/2-1)->getAyePage1();
//            a=list.at(getExamIndexAye(list,-1,-1));
//            if(ui->radioButton_examPage->isChecked())
//                return list.first();
            list=getAyePages(from,to);
            a=list.at(getExamIndexAye(list,-1,-1));
            return a;
        }

    }
}

QList<Aye *> MainWindow::getFirstAyePages(int from, int to)
{
    QList<Aye*> list;
    for(int i=from;i<=to;i++){
        if(i%2==0)
            list.append(pages.at(((i+1)/2)-1)->getAyePage2().first());
        else
            list.append(pages.at(((i+1)/2)-1)->getAyePage1().first());

    }

    return list;
}

QList<Aye *> MainWindow::getAyePages(int from, int to)
{
    QList<Aye*> list;
    for(int i=from;i<=to;i++){
        if(i%2==0)
            list.append(pages.at((i+1)/2)->getAyePage2());
        else
            list.append(pages.at((i+1)/2)->getAyePage1());

    }

    return list;
}

Aye *MainWindow::soreFilterExam()
{
    QList<Aye*> list=allQuran->value(ui->comboBox_examSore->currentData().toInt())->getAye();
    return list.at(getExamIndexAye(list,-1,-1));
}

 Aye * MainWindow::ayeFilterExam()
{
     int index=ui->comboBox_examSore->currentData().toInt();
     QList<Aye*> list=allQuran->value(index)->getAye();
     return list.at(getExamIndexAye(list,ui->comboBox_examFromAye->currentData().toInt(),ui->comboBox_examToAye->currentData().toInt()));
 }

 Aye *MainWindow::allFilterExam()
 {
     examAyeCount=0;
     if(ui->radioButton_examPage->isChecked()){

         return pageFilterExam(1,pages.count()*2);

     }
     else{
         for(int i=1;i<=allQuran->count();i++)
            examAyeCount+=allQuran->value(i)->getAye().count();


         int sore=rand()%114+1;
         QList<Aye*> list=allQuran->value(sore)->getAye();

         int index=rand()%list.count();
         if(ui->radioButton_examPage->isChecked())
             return pages.at((list.at(index)->getPage()+1)/2-1)->getAyesPage().first();
         return list.at(index);
     }


 }

int MainWindow::getExamIndexAye(QList<Aye *> list, int from, int to)
{

    //from>=0  && to>=0
    int index;
     if(from==-1 && to==-1){
         if(ui->radioButton_examPage->isChecked()){
             if(ui->checkBox_examLimitPage->isChecked())
                 examAyeCount=ui->comboBox_examToPage->currentText().toInt() - ui->comboBox_examFromPage->currentText().toInt()+1;
             else
                 examAyeCount=pages.count()*2;
             index=rand()%examAyeCount;
             return index;
         }
         else if(ui->checkBox_examLimitPage->isChecked()){
             int f=ui->comboBox_examFromPage->currentText().toInt();
             int t=ui->comboBox_examToPage->currentText().toInt();
             examAyeCount=0;
             for(int i=f;i<=t;i++){
                 if(i%2==0)
                     examAyeCount+=pages.at((i+1)/2)->getNumAyePage2();
                 else
                     examAyeCount+=pages.at((i+1)/2)->getNumAyePage1();
             }

             index=rand()%examAyeCount;
             return index;

         }

         else{
             examAyeCount=list.count();
             index=rand()%list.count();
             return index;

         }


    }

    examAyeCount=to-from+1;

    int r=rand();
    r=r%(to-from+1);
    index=(to)-r;
    return index;


}

Aye *MainWindow::checkLimitExam()
{
    Aye *examAye;



    if(ui->checkBox_examLimitPage->isChecked())
        examAye=pageFilterExam(ui->comboBox_examFromPage->currentText().toInt(),ui->comboBox_examToPage->currentText().toInt());

    else if(ui->checkBox_examLimitSore->isChecked())
        if(ui->checkBox_examLimitAye->isChecked())
            examAye=ayeFilterExam();
        else
            examAye=soreFilterExam();
    else if(!ui->checkBox_examLimitPage->isChecked() && !ui->checkBox_examLimitSore->isChecked())
        examAye=allFilterExam();



    return examAye;
}

void MainWindow::performExam()
{
    examAye=checkLimitExam();
    qDebug()<<"exam count "<<examAyeList->count();
    while(true){
        Global::ExamStatus res=checkExamAyeList();
        if(res==Global::ExamListFull){
            qDebug()<<"exam status Fullll";

            ui->label_examProperty->setText("");
            ui->label_examShow->setText(Language::translate("msg_resetExam"));//"برای شروع دوباره برروی Reset کلیک کنید.");


            break;
        }
        else if(res==Global::ExamAyeExist){
            examAye=checkLimitExam();
            //examAye=checkLimitExam();
        }
        else if(res==Global::ExamAyeNotExist){
            examAyeList->append(examAye);
            if(ui->radioButton_examText->isChecked()){

                //ui->label_examProperty->setText("سوره مبارکه "+examAye->getSoreName()+" , آیه "+QString::number(examAye->getVerse()));
                ui->label_examShow->setText("");

                ui->label_examProperty->setText(examAye->getText().mid(0,((examAye->getText().length()*15)/100))+"...");
            }
            else if(ui->radioButton_examSound->isChecked()){

                //ui->label_examProperty->setText("سوره مبارکه "+examAye->getSoreName()+" , آیه "+QString::number(examAye->getVerse()));
                ui->label_examProperty->setText("");
                ui->label_examShow->setText("");



            }

            else if(ui->radioButton_examAye->isChecked()){
                if(QString::compare(Language::getCurrentLanguage(),"english")==0){
                    ui->label_examProperty->setText(Language::translate("readExam")+Language::translate("aye")+" "+QString::number(examAye->getVerse())+
                                                   " , " +Language::translate("sore")+" "+allQuran->value(examAye->getChapter())->getEnName());
                }
                else{
                    ui->label_examProperty->setText(Language::translate("readExam")+Language::translate("aye")+" "+QString::number(examAye->getVerse())+
                                                   " , " +Language::translate("sore")+" "+examAye->getSoreName());
                }
                ui->label_examShow->setText("");
            }
            else if(ui->radioButton_examPage->isChecked()){

                ui->label_examProperty->setText(Language::translate("readPageExam")+QString::number(examAye->getPage()));
                ui->label_examShow->setText("");
            }

            break;
        }

    }


}

void MainWindow::playExam()
{
    playerState=Global::ExamPlay;
    player->stop();
    stopedSound();



    QString dir;
    if(QString::compare(qary->at(ui->comboBox_examQary->currentIndex())->getType(),"dir")==0)
        dir=qary->at(ui->comboBox_examQary->currentIndex())->getDir();
    else
        dir="Audio//"+qary->at(ui->comboBox_examQary->currentIndex())->getDir();


    soundPath=dir+"//"+examAye->getAudio();
    soundPath=soundPath+".mp3";
    player->setSoundPath(soundPath);

    if(QFile::exists(soundPath))
        player->play();
    else if(QString::compare(qary->at(ui->comboBox_examQary->currentIndex())->getType(),"dir")!=0){
        downloadSound();

        playerState=Global::NormalPlay;
        stopedSound();
    }
}

void MainWindow::checkExamDuration(int d)
{

    if(player->getDuration()>0 && d>(player->getDuration()*examDurationPercent/100) && !player->isFade()){
        player->fadeSound();

    }
}

Global::ExamStatus MainWindow::checkExamAyeList()
{
    if(examAyeList->count()>=examAyeCount)
        return Global::ExamListFull;

    else{
        if(searchExam())
            return Global::ExamAyeExist;
        return Global::ExamAyeNotExist;
    }


}

bool MainWindow::searchExam()
{
    if(!examAyeList->empty()){
        if(ui->radioButton_examPage->isChecked()){
            for(int i=0;i<examAyeList->count();i++){
                if(examAye->getPage()==examAyeList->at(i)->getPage())
                    return true;
            }
        }
        else{
            for(int i=0;i<examAyeList->count();i++){
                if(examAye->getChapter()==examAyeList->at(i)->getChapter() && examAye->getVerse()==examAyeList->at(i)->getVerse())
                    return true;
            }
        }

    }

    return false;
}

void MainWindow::resetExam()
{


    examAye=0;
    examAyeList=new QList<Aye*>();

    ui->label_examShow->setText("");
    ui->label_examProperty->setText("");
}





void MainWindow::on_spinBox_numRpeatPage_valueChanged(int arg1)
{
    numRepeat=arg1;
}



void MainWindow::on_comboBox_examSore_currentIndexChanged(int index)
{
    setExamAyeCombos();
}



void MainWindow::handleDoubleClickExamLabel()
{
    if(examAye!=NULL){
        setCurrentStates(examAye);
        doAllWork();
    }

}




void MainWindow::on_comboBox_examToAye_currentIndexChanged(int index)
{
    if(index<ui->comboBox_examFromAye->currentIndex())
        ui->comboBox_examToAye->setCurrentIndex(ui->comboBox_examFromAye->currentIndex());
}

void MainWindow::on_comboBox_examFromAye_currentIndexChanged(int index)
{
    if(index>ui->comboBox_examToAye->currentIndex())
        ui->comboBox_examToAye->setCurrentIndex(ui->comboBox_examFromAye->currentIndex());
}



void MainWindow::handleFadeEnd()
{
    stopedSound();
    player->stop();
    playerState=Global::NormalPlay;
}

void MainWindow::on_radioButton_examSound_toggled(bool checked)
{

    if(checked){
        ui->checkBox_examLimitSore->setEnabled(true);

        ui->toolButton_showAyeExam->setHidden(false);
        ui->toolButton_examPlay->setHidden(false);
        ui->toolButton_examStop->setHidden(false);
        ui->comboBox_examQary->setHidden(false);
        ui->label_qary_exam->setHidden(false);

        ui->label_examProperty->setHidden(true);

    }
    else{
        ui->toolButton_showAyeExam->setHidden(true);
        ui->toolButton_examPlay->setHidden(true);
        ui->toolButton_examStop->setHidden(true);
        ui->comboBox_examQary->setHidden(true);
        ui->label_qary_exam->setHidden(true);

        ui->label_examProperty->setHidden(false);
    }
}


void MainWindow::on_radioButton_examText_toggled(bool checked)
{
    if(checked){
        ui->checkBox_examLimitSore->setEnabled(true);

    }
}

void MainWindow::on_radioButton_examAye_toggled(bool checked)
{
    if(checked){
        resetExam();
        ui->checkBox_examLimitSore->setEnabled(true);

    }
}

void MainWindow::on_radioButton_examPage_toggled(bool checked)
{
    if(checked){
        resetExam();
        ui->checkBox_examLimitSore->setEnabled(false);
        ui->checkBox_examLimitAye->setEnabled(false);
        ui->comboBox_examFromAye->setEditable(false);
        ui->comboBox_examToAye->setEditable(false);

    }

}

void MainWindow::on_checkBox_examLimitPage_toggled(bool checked)
{
    if(checked){


        ui->label_examFromPage->setEnabled(true);
        ui->comboBox_examFromPage->setEnabled(true);
        ui->label_examToPage->setEnabled(true);
        ui->comboBox_examToPage->setEnabled(true);

        ui->checkBox_examLimitAye->setChecked(false);
        ui->checkBox_examLimitSore->setChecked(false);
        ui->comboBox_examSore->setEnabled(false);

        ui->checkBox_examLimitAye->setEnabled(false);
        ui->label_examFromAye->setEnabled(false);
        ui->comboBox_examFromAye->setEnabled(false);
        ui->label_examToAye->setEnabled(false);
        ui->comboBox_examToAye->setEnabled(false);

    }
    else{
        ui->label_examFromPage->setEnabled(false);
        ui->comboBox_examFromPage->setEnabled(false);
        ui->label_examToPage->setEnabled(false);
        ui->comboBox_examToPage->setEnabled(false);
    }
}

void MainWindow::on_checkBox_examLimitSore_toggled(bool checked)
{
    if(checked){


        ui->checkBox_examLimitSore->setChecked(true);
        ui->comboBox_examSore->setEnabled(true);

        ui->checkBox_examLimitAye->setEnabled(true);
        ui->label_examFromAye->setEnabled(false);
        ui->comboBox_examFromAye->setEnabled(false);
        ui->label_examToAye->setEnabled(false);
        ui->comboBox_examToAye->setEnabled(false);

        ui->label_examFromPage->setEnabled(false);
        ui->comboBox_examFromPage->setEnabled(false);
        ui->label_examToPage->setEnabled(false);
        ui->comboBox_examToPage->setEnabled(false);

        ui->checkBox_examLimitPage->setChecked(false);


    }
    else{
        ui->checkBox_examLimitSore->setChecked(false);
        ui->comboBox_examSore->setEnabled(false);
        ui->checkBox_examLimitAye->setEnabled(false);
        ui->label_examFromAye->setEnabled(false);
        ui->comboBox_examFromAye->setEnabled(false);
        ui->label_examToAye->setEnabled(false);
        ui->comboBox_examToAye->setEnabled(false);
    }
}

void MainWindow::on_checkBox_examLimitAye_toggled(bool checked)
{
    if(checked){

        ui->label_examFromAye->setEnabled(true);
        ui->comboBox_examFromAye->setEnabled(true);

        ui->label_examToAye->setEnabled(true);
        ui->comboBox_examToAye->setEnabled(true);
    }
    else{

        ui->label_examFromAye->setEnabled(false);
        ui->comboBox_examFromAye->setEnabled(false);
        ui->label_examToAye->setEnabled(false);
        ui->comboBox_examToAye->setEnabled(false);
    }
}



void MainWindow::on_toolButton_examReset_clicked()
{
    resetExam();
}

void MainWindow::on_toolButton_examStart_clicked()
{
    performExam();


}

void MainWindow::on_toolButton_examPlay_clicked()
{
    if(ui->radioButton_examSound->isChecked() && examAye!=NULL)
        playExam();
}

void MainWindow::on_toolButton_examStop_clicked()
{
    player->stop();
    playerState=Global::NormalPlay;
}







void MainWindow::on_toolButton_nozol_clicked()
{
    soreDialog->show();
}

void MainWindow::handleResetSetting()
{
//    ui->dockWidget_index_2->setFloating(false);
//    ui->dockWidget_search_2->setFloating(false);
//    this->tabifyDockWidget(ui->dockWidget_index_2,ui->dockWidget_search_2);

//    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dockWidget_exam);
//    ui->dockWidget_exam->close();

//    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dock_trans);
//    window->addDockWidget(Qt::BottomDockWidgetArea,ui->dock_bookmark_2);
//    ui->dock_bookmark_2->setFloating(false);
//    ui->dock_trans->setFloating(false);

//    window->tabifyDockWidget(ui->dock_trans,ui->dock_bookmark_2);

////    this->removeToolBar(ui->mainToolBar);
////    this->addToolBar(Qt::LeftToolBarArea,ui->mainToolBar);
////    ui->mainToolBar->show();

    this->restoreState(setting->loadSetting("defaultMainWindow").toByteArray());
    window->restoreState(setting->loadSetting("defaultCentralWindow").toByteArray());

    saveSetting();
}



void MainWindow::on_comboBox_examToPage_currentIndexChanged(int index)
{
    if(index<ui->comboBox_examFromPage->currentIndex())
        ui->comboBox_examToPage->setCurrentIndex(ui->comboBox_examFromPage->currentIndex());
}

void MainWindow::on_comboBox_examFromPage_currentIndexChanged(int index)
{
    if(index>ui->comboBox_examToPage->currentIndex())
        ui->comboBox_examToPage->setCurrentIndex(ui->comboBox_examFromPage->currentIndex());
}




void MainWindow::on_toolButton_showAyeExam_clicked()
{
    if(examAye!=NULL){
        setCurrentStates(examAye);
        doAllWork();
    }
}



void MainWindow::on_toolButton_preQary_clicked()
{
    preQary();
}

void MainWindow::on_toolButton_nextQary_clicked()
{
    nextQary();
}



void MainWindow::on_btn_help_triggered()
{
    helpDialog->show();
}

