#include "itemthread.h"

#include <QFile>

#include "filetreewidgetitem.h"

ItemThread::ItemThread(const QString &aFileName, FileTreeWidgetItem *aItem, QObject *parent) :
    QThread(parent)
{
    mFileName=aFileName;
    mItem=aItem;
    mOk=true;

    connect(this, SIGNAL(finished()), this, SLOT(threadFinished()));
}

void ItemThread::run()
{
    QFile aFile(mFileName);

    if (aFile.exists())
    {
        aFile.open(QIODevice::ReadOnly);

        while (!aFile.atEnd())
        {
            QString aLine=QString::fromUtf8(aFile.readLine());

            if (!aLine.startsWith("+"))
            {
                mOk=false;
                break;
            }
        }

        aFile.close();
    }
}

void ItemThread::threadFinished()
{
    mItem->threadFinished();
}

bool ItemThread::isOk()
{
    return mOk;
}
