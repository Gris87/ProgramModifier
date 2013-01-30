#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSplitter>
#include <QFileIconProvider>
#include <QTreeWidgetItem>

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
    QFileIconProvider *mIconProvider;
    QString            mProjectFolder;
    QString            mCurrentFile;

    void insertTreeNodes(QTreeWidgetItem *aParentItem, QString aFolder);
    void updateProjectFolder();
    void openFile();

    inline void saveState();
    inline void loadState();

private slots:
    void on_actionOpen_folder_triggered();
    void on_actionExit_triggered();
    void on_filesTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
};

#endif // MAINWINDOW_H
