#include "itemthread.h"

ItemThread::ItemThread(const QString &aFileName, QObject *parent) :
    QThread(parent)
{
    mFileName=aFileName;
    mOk=false;

    connect(this, SIGNAL(finished()), this, SLOT(threadFinished()));
}

void ItemThread::run()
{

}

void ItemThread::threadFinished()
{

}

bool ItemThread::isOk()
{
    return mOk;
}
