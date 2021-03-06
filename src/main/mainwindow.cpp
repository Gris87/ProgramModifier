#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSettings>
#include <QTextFrame>
#include <QKeyEvent>

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



    mGoodFormat.setBackground(QColor(220, 220, 255));
    mBadFormat.setBackground( QColor(255, 220, 220));



    ui->filesTreeWidget->installEventFilter(this);
    ui->codeTextEdit->installEventFilter(this);



    loadState();



    ui->filesTreeWidget->blockSignals(true);

    FileTreeWidgetItem *aItem=new FileTreeWidgetItem(QStringList() << "/");
    aItem->setProjectFolder(mProjectFolder);
    aItem->setIcon(0, mIconProvider.icon(QFileIconProvider::Folder));
    ui->filesTreeWidget->addTopLevelItem(aItem);

    updateProjectFolder();

    ui->filesTreeWidget->blockSignals(false);

    openFile();
}

MainWindow::~MainWindow()
{
    saveState();

    delete ui;
}

bool MainWindow::eventFilter(QObject *aObject, QEvent *aEvent)
{
    if (aEvent->type()==QEvent::KeyPress && aObject==ui->filesTreeWidget && ((QKeyEvent *)aEvent)->key()==Qt::Key_Delete)
    {
        deleteFile();
        return true;
    }
    else
    if (aEvent->type()==QEvent::KeyPress && aObject==ui->codeTextEdit && ((QKeyEvent *)aEvent)->key()==Qt::Key_Space)
    {
        toggleRows();
        return true;
    }

    return false;
}

void MainWindow::deleteFile()
{
    FileTreeWidgetItem *aItem=(FileTreeWidgetItem *)ui->filesTreeWidget->currentItem();

    if (aItem)
    {
        QString aFileName=aItem->filePath();

        if (QMessageBox::question(this, tr("Delete file"), tr("Do you want to delete file:\n%1").arg(aFileName), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape)==QMessageBox::Yes)
        {
            deleteFile(QDir::fromNativeSeparators(aFileName));

            if (aItem!=ui->filesTreeWidget->topLevelItem(0))
            {
                delete aItem;
            }
        }
    }
}

void MainWindow::deleteFile(QString aFileName)
{
    QFileInfo aFileInfo(aFileName);

    if (aFileInfo.exists())
    {
        if (aFileInfo.isDir())
        {
            QDir aDir(aFileName);
            QFileInfoList aFiles=aDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

            for (int i=0; i<aFiles.length(); ++i)
            {
                deleteFile(aFiles.at(i).absoluteFilePath());
            }

            aDir.cdUp();
            aDir.rmdir(aFileName.mid(aFileName.lastIndexOf("/")+1));
        }
        else
        {
            QFile::remove(aFileName);
        }
    }
}

void MainWindow::toggleRows()
{
    QTextCursor aCursor(ui->codeTextEdit->textCursor());

    int aStart=aCursor.selectionStart();
    int aStop=aCursor.selectionEnd();

    aCursor.setPosition(aStart);
    aCursor.movePosition(QTextCursor::StartOfBlock);
    aStart=aCursor.selectionStart();

    bool aGoodText=true;

    while (aCursor.position()<=aStop)
    {
        if (aCursor.block().blockFormat().background().color()==mBadFormat.background().color())
        {
            aGoodText=false;
            break;
        }

        if (!aCursor.movePosition(QTextCursor::NextBlock))
        {
            break;
        }
    }

    aGoodText=!aGoodText;

    aCursor.setPosition(aStart);
    aCursor.movePosition(QTextCursor::StartOfBlock);

    while (aCursor.position()<=aStop)
    {
        if (aGoodText)
        {
            aCursor.deleteChar();
            aCursor.insertText("+");
            aCursor.setBlockFormat(mGoodFormat);
        }
        else
        {
            aCursor.deleteChar();
            aCursor.insertText(" ");
            aCursor.setBlockFormat(mBadFormat);
        }

        if (!aCursor.movePosition(QTextCursor::NextBlock))
        {
            break;
        }
    }

    QFile aFile(mCurrentFile);
    aFile.open(QIODevice::WriteOnly);

    QStringList aLines=ui->codeTextEdit->toPlainText().split("\n");

    for (int i=0; i<aLines.length(); ++i)
    {
        if (i>0)
        {
#ifdef Q_OS_WIN
            aFile.write(QString("\r\n").toUtf8());
#else
            aFile.write(QString("\n").toUtf8());
#endif
        }

        QString aLine=aLines.at(i);

        if (aLine.startsWith(" "))
        {
            aLine.remove(0, 1);
        }

        aFile.write(aLine.toUtf8());
    }

    aFile.close();
}

void MainWindow::insertTreeNodes(FileTreeWidgetItem *aParentItem, QString aFolder)
{
    while (aParentItem->childCount()>0)
    {
        delete aParentItem->takeChild(0);
    }

    if (!aFolder.endsWith("/"))
    {
        aFolder.append("/");
    }

    QFileInfoList aFiles=QDir(aFolder).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);

    for (int i=0; i<aFiles.length(); i++)
    {
        QString aFileName=aFiles.at(i).fileName();

        if (aFiles.at(i).isDir())
        {
            FileTreeWidgetItem *aItem=new FileTreeWidgetItem(QStringList() << aFileName);
            aItem->setProjectFolder(mProjectFolder);
            aItem->setIcon(0, mIconProvider.icon(QFileIconProvider::Folder));
            aParentItem->addChild(aItem);

            insertTreeNodes(aItem, aFolder+aFileName);

            if (aItem->childCount()==0)
            {
                aItem->setState(FileTreeWidgetItem::VERIFIED_GOOD);
            }
        }
        else
        {
            FileTreeWidgetItem *aItem=new FileTreeWidgetItem(QStringList() << aFileName);
            aItem->setProjectFolder(mProjectFolder);
            aItem->setIcon(0, mIconProvider.icon(QFileIconProvider::File));
            aParentItem->addChild(aItem);

            aItem->createThread();
        }
    }
}

void MainWindow::updateProjectFolder()
{
    if (mProjectFolder!="")
    {
        setWindowTitle("ProgramModifier ["+mProjectFolder+"]");

        ui->filesTreeWidget->setUpdatesEnabled(false);

        FileTreeWidgetItem *aItem=(FileTreeWidgetItem *)ui->filesTreeWidget->topLevelItem(0);
        insertTreeNodes(aItem, QDir::fromNativeSeparators(mProjectFolder));
        aItem->setExpanded(true);

        ui->filesTreeWidget->setUpdatesEnabled(true);
    }
}

void MainWindow::openFile()
{
    ui->codeTextEdit->setUpdatesEnabled(false);
    ui->codeTextEdit->clear();

    QFile aFile(mCurrentFile);

    if (aFile.exists())
    {
        aFile.open(QIODevice::ReadOnly);

        QTextCursor aCursor(ui->codeTextEdit->textCursor());
        aCursor.movePosition(QTextCursor::Start);

        while (!aFile.atEnd())
        {
            QString aLine=QString::fromUtf8(aFile.readLine());

            while (aLine.length()>0 && aLine.at(aLine.length()-1).isSpace())
            {
                aLine.remove(aLine.length()-1, 1);
            }

            if (aLine.startsWith("+"))
            {
                aCursor.setBlockFormat(mGoodFormat);
            }
            else
            {
                aCursor.setBlockFormat(mBadFormat);
                aCursor.insertText(" ");
            }

            aCursor.insertText(aLine);
            aCursor.insertBlock();
        }

        aFile.close();
    }

    ui->codeTextEdit->setUpdatesEnabled(true);
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

bool firstRun=true;

void MainWindow::on_filesTreeWidget_currentItemChanged(QTreeWidgetItem *aCurrent, QTreeWidgetItem * /*aPrevious*/)
{
    if (firstRun)
    {
        firstRun=false;
        return;
    }

    if (aCurrent==0)
    {
        return;
    }

    QString aFileName=((FileTreeWidgetItem *)aCurrent)->filePath();

    if (QFileInfo(aFileName).isDir())
    {
        return;
    }

    mCurrentFile=aFileName;
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
    aSettings.setValue("CurrentFile", mCurrentFile);
    aSettings.setValue("Geometry", saveGeometry());
    aSettings.setValue("DividerState", mDividerSplitter->saveState());

    aSettings.endGroup();
}

void MainWindow::loadState()
{
    QString dir=QApplication::applicationDirPath()+"/";

    QSettings aSettings(dir+"data/mainwindow_ui.ini",QSettings::IniFormat);

    aSettings.beginGroup("States");

    mProjectFolder=aSettings.value("ProjectFolder", "").toString();
    mCurrentFile=aSettings.value("CurrentFile", "").toString();
    restoreGeometry(aSettings.value("Geometry").toByteArray());
    mDividerSplitter->restoreState(aSettings.value("DividerState").toByteArray());

    aSettings.endGroup();
}
