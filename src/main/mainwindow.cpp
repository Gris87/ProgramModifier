#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

#include "treedirectorydialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    mDividerSplitter = new QSplitter(Qt::Horizontal, this);
    ui->mainLayout->removeWidget(ui->filesTreeWidget);
    ui->mainLayout->removeWidget(ui->codeTextEdit);

    mDividerSplitter->addWidget(ui->filesTreeWidget);
    mDividerSplitter->addWidget(ui->codeTextEdit);

    ui->mainLayout->addWidget(mDividerSplitter);

    mDividerSplitter->setSizes(QList<int>() << 200 << 800);



    mIconProvider=new QFileIconProvider();


    QTreeWidgetItem *aItem=new QTreeWidgetItem(QStringList() << "/");
    aItem->setIcon(0, mIconProvider->icon(QFileIconProvider::Folder));
    ui->filesTreeWidget->addTopLevelItem(aItem);

    loadState();

    updateProjectFolder();
}

MainWindow::~MainWindow()
{
    saveState();

    delete ui;
}

void MainWindow::insertTreeNodes(QTreeWidgetItem *aParentItem, QString aFolder)
{
    while (aParentItem->childCount()>0)
    {
        delete aParentItem->takeChild(0);
    }

    if (!aFolder.endsWith("/"))
    {
        aFolder.append("/");
    }

    QFileInfoList aFiles=QDir(aFolder).entryInfoList(QDir::NoFilter, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);

    for (int i=0; i<aFiles.length(); i++)
    {
        QString aFileName=aFiles.at(i).fileName();

        if (aFiles.at(i).isDir())
        {
            if (aFileName=="." || aFileName=="..")
            {
                continue;
            }

            QTreeWidgetItem *aItem=new QTreeWidgetItem(QStringList() << aFileName);
            aItem->setIcon(0, mIconProvider->icon(QFileIconProvider::Folder));
            aParentItem->addChild(aItem);

            insertTreeNodes(aItem, aFolder+aFileName);
        }
        else
        {
            QTreeWidgetItem *aItem=new QTreeWidgetItem(QStringList() << aFileName);
            aItem->setIcon(0, mIconProvider->icon(QFileIconProvider::File));
            aParentItem->addChild(aItem);
        }
    }
}

void MainWindow::updateProjectFolder()
{
    insertTreeNodes(ui->filesTreeWidget->topLevelItem(0), QDir::fromNativeSeparators(mProjectFolder));

    setWindowTitle("ProgramModifier ["+mProjectFolder+"]");
}

void MainWindow::openFile()
{

}

void MainWindow::on_actionOpen_folder_triggered()
{
    TreeDirectoryDialog dialog(mProjectFolder, this);

    if (dialog.exec())
    {
        mProjectFolder=dialog.path();
        updateProjectFolder();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::on_filesTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem * /*previous*/)
{
    QString aFileName=QDir::fromNativeSeparators(current->text(0));

    while (current->parent())
    {
        current=current->parent();
        aFileName.insert(0, "/");
        aFileName.insert(0, current->text(0));
    }

    mCurrentFile=QDir::toNativeSeparators(mProjectFolder+aFileName);

    while (mCurrentFile.contains("//"))
    {
        mCurrentFile.replace("//", "/");
    }

    openFile();
}

void MainWindow::saveState()
{
    QString dir=QApplication::applicationDirPath()+"/";

    QDir(dir).mkpath(dir+"data");
    QFile::remove(dir+"data/mainwindow_ui.ini");
    QSettings aSettings(dir+"data/mainwindow_ui.ini",QSettings::IniFormat);



    aSettings.beginGroup("States");

    aSettings.setValue("ProjectFolder", mProjectFolder);
    aSettings.setValue("Geometry", saveGeometry());
    aSettings.setValue("DividerState", mDividerSplitter->saveState());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QString dir=QApplication::applicationDirPath()+"/";

    QSettings aSettings(dir+"data/mainwindow_ui.ini",QSettings::IniFormat);

    aSettings.beginGroup("States");

    mProjectFolder=aSettings.value("ProjectFolder", "C:\\").toString();
    restoreGeometry(aSettings.value("Geometry").toByteArray());
    mDividerSplitter->restoreState(aSettings.value("DividerState").toByteArray());

    aSettings.endGroup();
}
