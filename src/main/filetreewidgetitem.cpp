#include "filetreewidgetitem.h"

#include <QDir>

#define THREADS_COUNT 4
QList<ItemThread *> threads;

FileTreeWidgetItem::FileTreeWidgetItem(int type) :
    QTreeWidgetItem(type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(const QStringList &strings, int type) :
    QTreeWidgetItem(strings, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(QTreeWidget *view, int type) :
    QTreeWidgetItem(view, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(QTreeWidget *view, const QStringList &strings, int type) :
    QTreeWidgetItem(view, strings, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(QTreeWidget *view, FileTreeWidgetItem *after, int type) :
    QTreeWidgetItem(view, after, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(FileTreeWidgetItem *parent, int type) :
    QTreeWidgetItem(parent, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(FileTreeWidgetItem *parent, const QStringList &strings, int type) :
    QTreeWidgetItem(parent, strings, type)
{
    init();
}

FileTreeWidgetItem::FileTreeWidgetItem(FileTreeWidgetItem *parent, FileTreeWidgetItem *after, int type) :
    QTreeWidgetItem(parent, after, type)
{
    init();
}

void FileTreeWidgetItem::init()
{
    mState=NORMAL;
    mProjectFolder="";
    mMyThread=0;
}

FileTreeWidgetItem::~FileTreeWidgetItem()
{
    if (mMyThread)
    {
        int index=threads.indexOf(mMyThread);

        if (index>=0 && index<THREADS_COUNT)
        {
            mMyThread->wait();
        }

        delete mMyThread;
    }
}

void FileTreeWidgetItem::createThread()
{
    mMyThread=new ItemThread(filePath(), this);
    threads.append(mMyThread);

    if (threads.length()<THREADS_COUNT)
    {
        mMyThread->start(QThread::IdlePriority);
    }
}

void FileTreeWidgetItem::threadFinished()
{
    threads.removeOne(mMyThread);

    if (threads.length()>THREADS_COUNT)
    {
        threads.at(THREADS_COUNT)->start(QThread::IdlePriority);
    }

    if (mMyThread->isOk())
    {
        setState(VERIFIED_GOOD);
    }
    else
    {
        setState(VERIFIED_BAD);
    }

    mMyThread->deleteLater();
    mMyThread=0;
}

QString FileTreeWidgetItem::filePath()
{
    FileTreeWidgetItem *aItem=this;

    QString aFileName=QDir::fromNativeSeparators(aItem->text(0));

    while (aItem->parent())
    {
        aItem=(FileTreeWidgetItem *)aItem->parent();
        aFileName.insert(0, "/");
        aFileName.insert(0, aItem->text(0));
    }

    aFileName=QDir::fromNativeSeparators(mProjectFolder)+aFileName;

    while (aFileName.contains("//"))
    {
        aFileName.replace("//", "/");
    }

    aFileName=QDir::toNativeSeparators(aFileName);

    return aFileName;
}

void FileTreeWidgetItem::setState(const State &aState)
{
    mState=aState;
}

void FileTreeWidgetItem::setProjectFolder(const QString &aProjectFolder)
{
    mProjectFolder=aProjectFolder;
}
