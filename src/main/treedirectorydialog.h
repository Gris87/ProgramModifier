#ifndef TREEDIRECTORYDIALOG_H
#define TREEDIRECTORYDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QFileInfoList>
#include <QDir>
#include <QFileIconProvider>

#include "ui_treedirectorydialog.h"

namespace Ui {
    class TreeDirectoryDialog;
}

class TreeDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::TreeDirectoryDialog *ui;
    QFileIconProvider *iconProvider;

    explicit TreeDirectoryDialog(QString aPath="", QWidget *parent = 0);
    ~TreeDirectoryDialog();

    QString path();

private slots:
    void on_dirTreeWidget_itemExpanded(QTreeWidgetItem* item);
    void on_dirTreeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void on_cancelButton_clicked();
    void on_okButton_clicked();
};

#endif // TREEDIRECTORYDIALOG_H
