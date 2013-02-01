#ifndef ITEMTHREAD_H
#define ITEMTHREAD_H

#include <QThread>

class FileTreeWidgetItem;

class ItemThread : public QThread
{
    Q_OBJECT
public:
    explicit ItemThread(const QString &aFileName, FileTreeWidgetItem *aItem, QObject *parent = 0);

    void run();

    bool isOk();

private:
    QString             mFileName;
    FileTreeWidgetItem *mItem;
    bool                mOk;

private slots:
    void threadFinished();
};

#endif // ITEMTHREAD_H
