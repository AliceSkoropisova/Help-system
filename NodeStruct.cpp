#include "NodeStruct.h"
TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{}
TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}
//При добавлении ребёнка проверяется индекс и добавляется указанное число новых детей
//При этом для них не заданы значения!
bool TreeItem::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size())
        return false;
    for (int row = 0; row < count; ++row) {
        QList<QVariant> data;
        TreeItem *item = new TreeItem(data, this);
        m_childItems.insert(position, item);
    }
    return true;
}
//Удаление схожим образом
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;
    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);
    return true;
}
//Следующие методы очевидны
TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}
int TreeItem::childCount() const
{
    return m_childItems.count();
}
int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//Если вызвано с параметром - возвращается только указанный элемент из данных
QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}
//Если не задан - копия данных
QList<QVariant> TreeItem::data() const
{
    QList<QVariant> res = m_itemData;
    return res;
}
bool TreeItem::setData(int index, const QVariant &value)
{
    if(m_itemData.count()<=index)
        return false;
    else
        m_itemData[index]=value;
    return true;
}
bool TreeItem::setData(const QList<QVariant> &value)
{
    m_itemData=value;
}
TreeItem *TreeItem::parent()
{
    return m_parentItem;
}
