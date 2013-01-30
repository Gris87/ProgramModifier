#include "treedirectorydialog.h"

TreeDirectoryDialog::TreeDirectoryDialog(QString aPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeDirectoryDialog)
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::TreeDirectoryDialog()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    ui->setupUi(this);

    iconProvider=new QFileIconProvider();

    QFileInfoList aDrives=QDir::drives();

    for (int i=0; i<aDrives.length(); i++)
    {
        QStringList values;

        QString aDisk=aDrives.at(i).absoluteFilePath();

        if (aDisk!="/")
        {
            aDisk.remove(aDisk.length()-1, 1);
        }

        values.append(aDisk);

        QTreeWidgetItem *aItem=new QTreeWidgetItem(values);
        aItem->setIcon(0, iconProvider->icon(QFileIconProvider::Drive));
        ui->dirTreeWidget->addTopLevelItem(aItem);

        values.clear();

        QTreeWidgetItem *aItem2=new QTreeWidgetItem(values);
        aItem->addChild(aItem2);
    }

    if (aPath!="")
    {
        aPath=QDir::fromNativeSeparators(aPath);

        QString aPart=aPath.left(aPath.indexOf("/"));
        QTreeWidgetItem *aItem=0;

        for (int i=0; i<ui->dirTreeWidget->topLevelItemCount(); i++)
        {
            if (ui->dirTreeWidget->topLevelItem(i)->text(0)==aPart)
            {
                aItem=ui->dirTreeWidget->topLevelItem(i);
                break;
            }
        }

        if (aItem)
        {
            do
            {
                aPath.remove(0, aPart.length()+1);
                ui->dirTreeWidget->expandItem(aItem);

                if (aPath=="")
                {
                    break;
                }

                aPart=aPath.left(aPath.indexOf("/"));

                bool good=false;

                for (int i=0; i<aItem->childCount(); i++)
                {
                    if (aItem->child(i)->text(0)==aPart)
                    {
                        aItem=aItem->child(i);
                        good=true;
                        break;
                    }
                }

                if (!good)
                {
                    break;
                }
            }
            while(true);

            ui->dirTreeWidget->setCurrentItem(aItem);
            ui->dirTreeWidget->scrollToItem(aItem);
        }

        ui->dirTreeWidget->expandItem(0);
    }
}

TreeDirectoryDialog::~TreeDirectoryDialog()
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::~TreeDirectoryDialog()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    delete ui;
}

QString TreeDirectoryDialog::path()
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::path()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    return ui->pathLineEdit->text();
}

void TreeDirectoryDialog::on_okButton_clicked()
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::on_okButton_clicked()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    accept();
}

void TreeDirectoryDialog::on_cancelButton_clicked()
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::on_cancelButton_clicked()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    reject();
}

void TreeDirectoryDialog::on_dirTreeWidget_itemExpanded(QTreeWidgetItem* item)
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::on_dirTreeWidget_itemExpanded()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    QTreeWidgetItem* aOrigItem=item;

    QString aFolder=QDir::fromNativeSeparators(item->text(0));

    while (item->parent())
    {
        item=item->parent();
        aFolder.insert(0, "/");
        aFolder.insert(0, item->text(0));
    }

    if (!aFolder.endsWith("/"))
    {
        aFolder.append("/");
    }

    while (aOrigItem->childCount()>0)
    {
        delete aOrigItem->takeChild(0);
    }

    QDir aDir(aFolder);

    QFileInfoList aDirectories=aDir.entryInfoList();

    for (int i=0; i<aDirectories.length(); i++)
    {
        if (!aDirectories.at(i).isDir())
        {
            continue;
        }

        QStringList values;

        QString aDirectory=aDirectories.at(i).fileName();

        if (aDirectory=="." || aDirectory=="..")
        {
            continue;
        }

        values.append(aDirectory);

        QTreeWidgetItem *aItem=new QTreeWidgetItem(values);
        aItem->setIcon(0, iconProvider->icon(QFileIconProvider::Folder));
        aOrigItem->addChild(aItem);

        values.clear();

        QTreeWidgetItem *aItem2=new QTreeWidgetItem(values);
        aItem->addChild(aItem2);
    }
}

void TreeDirectoryDialog::on_dirTreeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
#ifdef MY_DEBUG
    qDebug()<<"--TreeDirectoryDialog::on_dirTreeWidget_currentItemChanged()"<<"| time ="<<QDateTime::currentMSecsSinceEpoch()-aActionStart;
#endif

    QString aFolder=QDir::fromNativeSeparators(current->text(0));

    while (current->parent())
    {
        current=current->parent();
        aFolder.insert(0, "/");
        aFolder.insert(0, current->text(0));
    }

    if (!aFolder.endsWith("/"))
    {
        aFolder.append("/");
    }

    ui->pathLineEdit->setText(QDir::toNativeSeparators(aFolder));
}
