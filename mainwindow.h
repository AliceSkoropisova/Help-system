#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "model.h"
#include <QSettings>
#include <QTreeView>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_addButton_clicked();
    void on_deliteButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_addPhotoButton_clicked();
    void on_redButton_clicked();
    void on_saveButton_clicked();
    void on_downloadButton_clicked();
    void on_pushButton_clicked();
    void TreeToIni(QTreeView* treeView, QSettings& settings,int lev, const QModelIndex& parentIndex = QModelIndex() );
private:
    Ui::MainWindow *ui;
    model *mainModel;
    QString path="";
    int flag = 0;
    QModelIndex memoryIndex;
    QList<QVariant> memoryData;
    int count=0;
};
#endif // MAINWINDOW_H
