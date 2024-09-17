#include "model.h"
//Конструктор создаёт корневой элемент, содержащий заголовки
model::model(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;
    rootItem = new TreeItem(rootData);
}
model::~model()
{
    delete rootItem;
}
//Для получения заголовка просто берутся данные из корневого элемента
QVariant model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}
//Обратный процесс
bool model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;
    const bool result = rootItem->setData(section, value);
    //При изменении заголовков об этом надо известить представление
    if (result)
        emit headerDataChanged(orientation, section, section);
    return result;
}
//Получение индекса - используется стандартный метод для его создания, по сути тут только проверки
QModelIndex model::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//Получение индекса родителя, опять же проверки и вызов стандартного метода
QModelIndex model::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->childCount(), 0, parentItem);
}
//Число строк = число детей у указанного родителя
int model::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
//Столбцов в данной моделе всегда один!
int model::columnCount(const QModelIndex &parent) const
{
     return 1;
}
//Добавление строк, по сути получение элемента и добавление ему детей
bool model::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;
    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,rows);
    endInsertRows();
    return success;
}
//Удаление аналогично
bool model::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;
    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}
//Получение данных
//Тут идёт проверка роли - если для отображения, то только первое поле
//Если UserRole - то все
QVariant model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::UserRole)
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        TreeItem *item = getItem(index);
        return item->data(0);
    }
    if(role==Qt::UserRole)
    {
        TreeItem *item = getItem(index);
        return QVariant(item->data());
    }
}
//А вот задание данных проще - тут сразу всё
bool model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;
    //Сначала только надо преобразовать в список
    QList<QVariant> itemData=value.toList();
    TreeItem *item = getItem(index);
    bool result = item->setData(itemData);
    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return result;
}
//Флаги в данном слечае для выделения и редактирования
Qt::ItemFlags model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//Возвращает элемент по индексу
//Спасибо указателям:)
TreeItem *model::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}
