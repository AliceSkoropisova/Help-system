#ifndef MODEL_H
#define MODEL_H
#include "NodeStruct.h"
#include <QAbstractItemModel>
class model : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit model(const QStringList &headers, QObject *parent = nullptr);
    ~model();
    // Получение и установка заголовков столбцов
    //В данной модели заголовки хранятся в корневом элементе
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    //Получение индекса по номеру столбца и строки. В данной модели будет только 1 столбец!
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    //Получения родителя по индексу элемента
    QModelIndex parent(const QModelIndex &index) const override;
    //Получение числа строк и столбцов. Строки в данном случае - это число дочерних элементов
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    //Добавление и удаление строк
    //Аналогичные методы, добавляющие или удаляющие по одной строке уже определены в базовом классе
    bool insertRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,const QModelIndex &parent = QModelIndex()) override;
    //Получение данных элемента
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Редактирование элемента
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Служебная функция, указывающая что можно делать с элементами модели
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    //Данный метод служебный и не входит в открытый API, позволяет обратиться к элементу по индексу
    TreeItem *getItem(const QModelIndex &index) const;
private:
    TreeItem *rootItem;
};
#endif // MODEL_H
