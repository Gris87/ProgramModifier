#ifndef ITEMTHREAD_H
#define ITEMTHREAD_H

#include <QThread>

class ItemThread : public QThread
{
    Q_OBJECT
public:
    explicit ItemThread(const QString &aFileName, QObject *parent = 0);

    void run();

    bool isOk();

private:
    bool    mOk;
    QString mFileName;

private slots:
    void threadFinished();
};

#endif // ITEMTHREAD_H
