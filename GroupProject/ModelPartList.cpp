/**
 * @file ModelPartList.cpp
 * @brief Implementation of the ModelPartList class used to manage the model tree structure.
 * @details This class implements a custom tree model using Qt's QAbstractItemModel,
 *          with support for inserting, removing, and retrieving model parts for use in a tree view.
 * @version 1.0.0
 * @author Woojin, Zhixing, Zhiyuan/Paul
 * @date 2025-05-12/2022
 */

#include "ModelPartList.h"
#include "ModelPart.h"
/**
 * @brief Constructs the model with an optional name and parent object.
 * @param data Root node label.
 * @param parent Optional QObject parent.
 */
ModelPartList::ModelPartList( const QString& data, QObject* parent ) : QAbstractItemModel(parent) {
    /* Have option to specify number of visible properties for each item in tree - the root item
     * acts as the column headers
     */
    rootItem = new ModelPart( { tr("Part"), tr("Visible?") } );
}


/**
 * @brief Destructor, deletes root item and its children.
 */
ModelPartList::~ModelPartList() {
    delete rootItem;
}

/**
 * @brief Returns the number of columns from the root item.
 */
int ModelPartList::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);

    return rootItem->columnCount();
}

/**
 * @brief Retrieves data from a specific index, only when display role is requested.
 */
QVariant ModelPartList::data( const QModelIndex& index, int role ) const {
    /* If the item index isnt valid, return a new, empty QVariant (QVariant is generic datatype
     * that could be any valid QT class) */
    if( !index.isValid() )
        return QVariant();

    /* Role represents what this data will be used for, we only need deal with the case
     * when QT is asking for data to create and display the treeview. Return a new,
     * empty QVariant if any other request comes through. */
    if (role != Qt::DisplayRole)
        return QVariant();

    /* Get a a pointer to the item referred to by the QModelIndex */
    ModelPart* item = static_cast<ModelPart*>( index.internalPointer() );

    /* Each item in the tree has a number of columns ("Part" and "Visible" in this 
     * initial example) return the column requested by the QModelIndex */
    return item->data( index.column() );
}

/**
 * @brief Returns item flags for the given index.
 */
Qt::ItemFlags ModelPartList::flags( const QModelIndex& index ) const {
    if( !index.isValid() )
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags( index );
}

/**
 * @brief Returns the header label at a given section and orientation.
 */
QVariant ModelPartList::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

/**
 * @brief Returns a model index for the given row and column under the specified parent.
 */
QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const {
    ModelPart* parentItem;
    
    if( !parent.isValid() || !hasIndex(row, column, parent) )
        parentItem = rootItem;              // default to selecting root 
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart* childItem = parentItem->child(row);
    if( childItem )
        return createIndex(row, column, childItem);
    
    
    return QModelIndex();
}


/**
 * @brief Returns the parent model index for a given child index.
 */
QModelIndex ModelPartList::parent( const QModelIndex& index ) const {
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

/**
 * @brief Returns the number of child rows under a parent index.
 */
int ModelPartList::rowCount( const QModelIndex& parent ) const {
    ModelPart* parentItem;
    if( parent.column() > 0 )
        return 0;

    if( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem->childCount();
}


/**
 * @brief Returns a pointer to the root ModelPart item.
 */
ModelPart* ModelPartList::getRootItem() {
    return rootItem; 
}


/**
 * @brief Appends a new child to the model under a given parent and returns its index.
 * @param parent Index of the parent item.
 * @param data List of data values for the new part.
 * @return Index of the newly created child item.
 */
QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data) {      
    ModelPart* parentPart;

    if (parent.isValid())
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem );
    }

    beginInsertRows( parent, rowCount(parent), rowCount(parent) ); 

    ModelPart* childPart = new ModelPart( data, parentPart );

    parentPart->appendChild(childPart);

    QModelIndex child = createIndex(0, 0, childPart);

    endInsertRows();

    emit layoutChanged();

    return child;
}
/**
 * @brief Removes one or more child items from a given parent.
 * @param row Starting row of items to remove.
 * @param count Number of rows to remove.
 * @param parent Parent model index.
 * @return True if rows were removed successfully.
 */
bool ModelPartList::removeRows(int row, int count, const QModelIndex &parent) {
    if (!hasIndex(row, 0, parent))
        return false;

    beginRemoveRows(parent, row, row + count - 1);

    ModelPart* parentItem = parent.isValid()
                                ? static_cast<ModelPart*>(parent.internalPointer())
                                : rootItem;
    for (int i = 0; i < count; ++i) {
        ModelPart* child = parentItem->takeChild(row);
        delete child;
    }

    endRemoveRows();
    return true;
}
