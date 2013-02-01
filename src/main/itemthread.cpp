#include "itemthread.h"

#include "filetreewidgetitem.h"

ItemThread::ItemThread(const QString &aFileName, FileTreeWidgetItem *aItem, QObject *parent) :
    QThread(parent)
{
    mFileName=aFileName;
    mItem=aItem;
    mOk=false;

    connect(this, SIGNAL(finished()), this, SLOT(threadFinished()));
}

void ItemThread::run()
{

}

void ItemThread::threadFinished()
{
    mItem->threadFinished();
}

bool ItemThread::isOk()
{
    return mOk;
}
