#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSplitter>
#include <QFileIconProvider>
#include <QTreeWidgetItem>
#include <QTextBlockFormat>

#include "filetreewidgetitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow    *ui;
    QSplitter         *mDividerSplitter;
    QFileIconProvider  mIconProvider;
    QTextBlockFormat   mGoodFormat;
    QTextBlockFormat   mBadFormat;
    QString            mProjectFolder;
    QString            mCurrentFile;

    void deleteFile();
    void deleteFile(QString aFileName);
    void toggleRows();
    void insertTreeNodes(FileTreeWidgetItem *aParentItem, QString aFolder);
    void updateProjectFolder();
    QString itemPath(FileTreeWidgetItem *aItem);
    void openFile();

    inline void saveState();
    inline void loadState();

protected:
    bool eventFilter(QObject *aObject, QEvent *aEvent);

private slots:
    void on_actionOpen_folder_triggered();
    void on_actionExit_triggered();
    void on_filesTreeWidget_currentItemChanged(QTreeWidgetItem *aCurrent, QTreeWidgetItem *aPrevious);
};

#endif // MAINWINDOW_H
