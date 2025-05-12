/**
 * @file ModelPartList.h
 * @brief Declaration of the ModelPartList class used to model tree structures in Qt.
 * @details Provides a custom QAbstractItemModel implementation for managing a hierarchy of ModelPart items,
 *          to be displayed in a QTreeView. Each node supports data display and interaction via Qt's model/view system.
 * @version 1.0.0
 * @date 2025-05-12/2022
 * @author Woojin, Zhixing, Zhiyuan/Paul
 */
  
#ifndef VIEWER_MODELPARTLIST_H
#define VIEWER_MODELPARTLIST_H


#include "ModelPart.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QList>

class ModelPart;
/**
 * @class ModelPartList
 * @brief Qt model class representing a tree of model parts.
 * @details Extends QAbstractItemModel to allow dynamic interaction with parts data in a hierarchical tree.
 */
class ModelPartList : public QAbstractItemModel {
    Q_OBJECT        /**< A special Qt tag used to indicate that this is a special Qt class that might require preprocessing before compiling. */
public:
    /** Constructor
      *  Arguments are standard arguments for this type of class but are not used in this example.
      * @brief Constructs the model with a label and optional parent.
      * @param data is not used
      * @param parent is used by the parent class constructor
      */
    ModelPartList( const QString& data, QObject* parent = NULL );

    /**
     * @brief Destructor. Cleans up root item.
     */
    ~ModelPartList();

    /**
     * @brief Returns the number of columns in the tree view.
     * @param parent Not used.
     * @return Number of columns (usually 2: "Part", "Visible").
     */
    int columnCount( const QModelIndex& parent ) const;

    /** This returns the value of a particular row (i.e. the item index) and 
      *  columns (i.e. either the "Part" or "Visible" property).
      *  It is used by QT internally - this is how Qt retrieves the text to display in the TreeView
      *  @brief Provides data for the view at a given index and role.
      * @param index in a stucture Qt uses to specify the row and column it wants data for
      * @param role is how Qt specifies what it wants to do with the data
      * @return a QVariant which is a generic variable used to represent any Qt class type, in this case the QVariant will be a string
      */
    QVariant data( const QModelIndex& index, int role ) const;

    /** Standard function used by Qt internally.
     *   @brief Returns flags for the item at a given index.
      * @param index in a stucture Qt uses to specify the row and column it wants data for
      * @return a Qt item flags
      */
    Qt::ItemFlags flags( const QModelIndex& index ) const;


    /**
     * @brief Returns the header data for a given section and orientation.
     * @param section The header index.
     * @param orientation Horizontal or vertical.
     * @param role The purpose of header data access.
     * @return The QVariant holding the header label.
     */
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;


    /** Get a valid QModelIndex for a location in the tree (row is the row in the tree under "parent"
      * or under the root of the tree if parent isnt specified. Column is either 0 = "Part" or 1 = "Visible" 
      * in this example 
      * @brief Returns the model index for a child at a given row and column.
      * @param row is the item index
      * @param column is 0 or 1 - part name or visible stringstream
      * @param parent where the row is referenced from, usually the tree root
      * @return the QModelIndex structure
     */
    QModelIndex index( int row, int column, const QModelIndex& parent ) const;


    /** Take a QModelIndex for an item, get a QModel Index for its parent
     *  @brief Returns the parent model index of a given child.
      * @param index of item
      * @return index of parent
      */
    QModelIndex parent( const QModelIndex& index ) const;

    /** Get number of rows (items) under an item in tree
     *  @brief Returns the number of child rows under a parent.
      *  @param is the parent, all items under this will be counted
      *  @return number of children
      */
    int rowCount( const QModelIndex& parent ) const;

    /** Get a pointer to the root item of the tree
     *  @brief Returns a pointer to the root item.
      * @return the root item pointer
      */
    ModelPart* getRootItem();

    /**
     * @brief Appends a new child ModelPart to the tree under a given parent.
     * @param parent Index of the parent item.
     * @param data List of QVariant values for the child.
     * @return Index of the new child.
     */
    QModelIndex appendChild( QModelIndex& parent, const QList<QVariant>& data );

    /**
     * @brief Removes one or more child rows from a parent.
     * @param row The starting row.
     * @param count Number of rows to remove.
     * @param parent The parent index.
     * @return True if the rows were removed.
     */
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    ModelPart *rootItem;    /**< This is a pointer to the item at the base of the tree */
};
#endif

