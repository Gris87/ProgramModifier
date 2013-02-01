#ifndef FILETREEWIDGETITEM_H
#define FILETREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "itemthread.h"

class FileTreeWidgetItem : public QTreeWidgetItem
{
public:
    enum State
    {
        NORMAL,
        VERIFIED_GOOD,
        VERIFIED_BAD
    };

    explicit FileTreeWidgetItem(int type = Type);
    FileTreeWidgetItem(const QStringList &strings, int type = Type);
    explicit FileTreeWidgetItem(QTreeWidget *view, int type = Type);
    FileTreeWidgetItem(QTreeWidget *view, const QStringList &strings, int type = Type);
    FileTreeWidgetItem(QTreeWidget *view, FileTreeWidgetItem *after, int type = Type);
    explicit FileTreeWidgetItem(FileTreeWidgetItem *parent, int type = Type);
    FileTreeWidgetItem(FileTreeWidgetItem *parent, const QStringList &strings, int type = Type);
    FileTreeWidgetItem(FileTreeWidgetItem *parent, FileTreeWidgetItem *after, int type = Type);
    ~FileTreeWidgetItem();

    void createThread();
    void threadFinished();

    QString filePath();



    void setState(const State &aState);

    void setProjectFolder(const QString &aProjectFolder);

private:
    State       mState;
    QString     mProjectFolder;
    ItemThread *mMyThread;

    void init();
};

#endif // FILETREEWIDGETITEM_H
