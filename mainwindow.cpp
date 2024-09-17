#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Создание и подключение модели
    QStringList headers = {"Иерархия"};
    mainModel = new model(headers);
    ui->treeView->setModel(mainModel);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_addButton_clicked()
{
        auto index= ui->treeView->selectionModel()->currentIndex();
        QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
        if(ui->nameLineEdit->text()!="" && ui->descrLineEdit->text()!=""/* && path!=""*/)
        {
            mainModel->insertRow(0,index);
            index=mainModel->index(0,0,index);
            Animal newItem;
            newItem.name=ui->nameLineEdit->text();
            newItem.descr=ui->descrLineEdit->text();
            newItem.path=path;
            path="";
            itemData={newItem.name,newItem.descr,newItem.path};
            mainModel->setData(index,itemData);
      }
    ui->treeView->clearSelection();
}
void MainWindow::on_deliteButton_clicked()
{
    if(ui->treeView->selectionModel()->selectedIndexes().count()>0)
    {
        auto index= ui->treeView->selectionModel()->currentIndex();
        mainModel->removeRow(index.row(),index.parent());
    }
    ui->treeView->clearSelection();
    ui->descrLabel->setText("");
    QPixmap myPhoto("../sad.jpg");
    ui->photoLabel->setPixmap(myPhoto);
    ui->nameLabel->setText("");
}
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    if(flag==0)
    {
        QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
        ui->nameLabel->setText(itemData[0].toString());
        ui->descrLabel->setText(itemData[1].toString());
        QPixmap myPhoto(itemData[2].toString());
        ui->photoLabel->setPixmap(myPhoto);
    }
    else
    {
        if(flag==1)
        {
            if(ui->treeView->selectionModel()->selectedIndexes().count()>0)
            {
                flag++;
                QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
                memoryIndex = index;
                //Данные заносятся в модель
                memoryData={itemData[0].toString(),itemData[1].toString(),itemData[2].toString()};
             }
        }
            else
            {
                flag=0;
                QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
                Animal newItem;
                newItem.name=itemData[0].toString();
                newItem.descr=itemData[1].toString();
                newItem.path=itemData[2].toString();
                itemData={newItem.name,newItem.descr,newItem.path};
                mainModel->setData(index,memoryData);
                mainModel->setData(memoryIndex,itemData);
            }
    }
}
void MainWindow::on_addPhotoButton_clicked()
{
    path = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл"),QDir::homePath(), QObject::tr("Все файлы (*.*)"));
}
void MainWindow::on_redButton_clicked()
{
    if(ui->treeView->selectionModel()->selectedIndexes().count()>0)
    {
        auto index= ui->treeView->selectionModel()->currentIndex();
        if(ui->nameLineEdit->text()!="" && ui->descrLineEdit->text()!="" && path!="")
        {
            QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
            Animal newItem;
            newItem.name=ui->nameLineEdit->text();
            newItem.descr=ui->descrLineEdit->text();
            newItem.path=path;
            path="";
            //Данные заносятся в модель
            itemData={newItem.name,newItem.descr,newItem.path};
            mainModel->setData(index,itemData);
                ui->nameLabel->setText(itemData[0].toString());
                ui->descrLabel->setText(itemData[1].toString());
                QPixmap myPhoto(itemData[2].toString());
                ui->photoLabel->setPixmap(myPhoto);
        }
    }
}
void MainWindow::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save INI", ".","INI files (*.ini)");
    QSettings settings(filename, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    TreeToIni(ui->treeView, settings,0);
}
void MainWindow::on_downloadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Открыть INI", ".","Файлы INI (*.ini)");
    QSettings settings(filename, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    mainModel->removeRows(0, mainModel->rowCount()); // Очистка существующих данных
    QStringList groupNames = settings.childGroups();
    QModelIndexList indexList;
    count=0;
        // Заполняем дерево
        for(int i=0; i<groupNames.count();i++)
        {
            // Создаем корневой элемент соответствующий группе
            auto index= ui->treeView->currentIndex();
            QList<QVariant> itemData=mainModel->data(index,Qt::UserRole).toList();
                Animal newItem;
                settings.beginGroup(groupNames[i]);
                if(settings.value("parent","").toString()!="")
                {
                    for(int j=0;j<indexList.count();j++)
                    {
                        TreeItem* a=mainModel->getItem(indexList[j]);
                        if(a->data()[0].toString()==settings.value("parent","").toString())
                        {
                            mainModel->insertRow(0,indexList[j]);
                            index=mainModel->index(0,0,indexList[j]);
                            newItem.descr=settings.value("descr","").toString();
                            newItem.name=settings.value("name","").toString();

                            newItem.path=settings.value("path","").toString();
                            itemData={newItem.name,newItem.descr,newItem.path};
                            mainModel->setData(index,itemData);
                            indexList.append(index);
                        }
                    }
                }
                else
                {
                mainModel->insertRow(0,index);
                index=mainModel->index(0,0,index);
                newItem.descr=settings.value("descr","").toString();
                newItem.name=settings.value("name","").toString();

                newItem.path=settings.value("path","").toString();
                itemData={newItem.name,newItem.descr,newItem.path};
                mainModel->setData(index,itemData);
                indexList.append(index);
                }
                settings.endGroup();
        }
}
void MainWindow::on_pushButton_clicked()
{
    flag++;
}
void MainWindow::TreeToIni(QTreeView* treeView, QSettings& settings,int lev=0, const QModelIndex& parentIndex)
{
    for (int row = 0; row < treeView->model()->rowCount(parentIndex); ++row) {
           QModelIndex index = treeView->model()->index(row, 0, parentIndex);
           QList<QVariant> itemText = mainModel->data(index,Qt::UserRole).toList();
           QList<QVariant> parentItemText = mainModel->parent(index).data(Qt::UserRole).toList();
           QString cou=QString::number(count);
           settings.beginGroup("Item_"+cou);
           if(parentItemText.count()!=0)
           {
               QString str=parentItemText[0].toString();
               settings.setValue(QString("name/") , itemText[0].toString());
               settings.setValue(QString("descr/") , itemText[1].toString());
               settings.setValue(QString("path/") , itemText[2].toString());
               settings.setValue(QString("parent/"), str);
           }
           else
           {
               settings.setValue(QString("name/") , itemText[0].toString());
               settings.setValue(QString("descr/") , itemText[1].toString());
               settings.setValue(QString("path/") , itemText[2].toString());
               settings.setValue(QString("parent/"), "");
           }
           lev++;count++;
           settings.endGroup();
           if (treeView->model()->hasChildren(index)) {
               TreeToIni(treeView, settings,lev, index);
           }
       }
}




