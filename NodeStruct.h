#ifndef NODESTRUCT_H
#define NODESTRUCT_H
#include <QList>
#include <QVariant>
//Структуры, которые описывают данные
//животные
struct Animal
{
    QString name;
    QString descr;
    QString path;
};
//Класс, описывающий элемент иерархии. Достаточно универсальный
class TreeItem
{
public:
    //Конструктор и деструктор
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();
    //Добавление и удаление дочерних элементов (сразу пачкой)
    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);
    //Данные методы понадобятся в первую очередь для сериализации и десериализации
    //Получение доступа к дочернему элементу по его номеру
    TreeItem *child(int row);
    //Число детей
    int childCount() const;
    //Номер элемента среди детей родителя
    int row() const;
    //Получение родителя
    TreeItem *parent();
    //Получение и заполнение данных. Два варианта - конкретной части данных и всех сразу
    QVariant data(int index) const;
    QList<QVariant> data() const;
    bool setData(int index, const QVariant &value);
    bool setData(const QList<QVariant> &value);
private:
    //Список детей
    QList<TreeItem *> m_childItems;
    //Данные в виде списка полей
    QList<QVariant> m_itemData;
    //Родитель
    TreeItem *m_parentItem;
};
#endif // NODESTRUCT_H
