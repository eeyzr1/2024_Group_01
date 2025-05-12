/**
 * @file ModelPart.h
 * @brief Declaration of the ModelPart class.
 * @details Template class representing hierarchical model parts to be added as tree view items and rendered with VTK.
 *          Each part supports visibility, color, and optional filters such as clipping and shrinking.
 * @version 1.0.0
 * @author Woojin, Zhixing, Zhiyuan/Paul
 * @date 2025-05-12/2022
 */
  
#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

/* VTK headers - will be needed when VTK used in next worksheet,
 * commented out for now
 *
 * Note that there are a few function definitions and variables
 * commented out below - this is because you haven't yet installed
 * the VTK library which is needed.
 */
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>
/**
 * @class ModelPart
 * @brief Represents a single part in a hierarchical model tree and links it to a VTK-rendered entity.
 */
class ModelPart {
public:
    /** Constructor
     * @brief Constructor for a model part.
     * @param data is a List (array) of strings for each property of this item (part name and visiblity in our case
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor
     *  @brief Destructor that deletes all children.
      * Needs to free array of child items
      */
    ~ModelPart();

    /** Add a child to this item.
     *  @brief Adds a child item to this part.
      * @param item Pointer to child object (must already be allocated using new)
      */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item
     *  @brief Returns a child at the given row.
      * @param row is the row number (below this item)
      * @return pointer to the item requested.
      */
    ModelPart* child(int row);

    /** Return number of children to this item
     *  @brief Returns the number of child parts.
      * @return number of children
      */
    int childCount() const;         /* Note on the 'const' keyword - it means that this function is valid for
                                     * constant instances of this class. If a class is declared 'const' then it
                                     * cannot be modifed, this means that 'set' type functions are usually not
                                     * valid, but 'get' type functions are.
                                     */

    /** Get number of data items (2 - part name and visibility string) in this case.
      * @brief Returns the number of data columns this part holds.
      * @return number of visible data columns
      */
    int columnCount() const;

    /** Return the data item at a particular column for this item.
      * i.e. either part name of visibility
      * @brief Gets data at a specified column.
      * used by Qt when displaying tree
      * @param column is column index
      * @return the QVariant (represents string)
      */
    QVariant data(int column) const;


    /** Default function required by Qt to allow setting of part
      * properties within treeview.
      * @brief Sets the value at a specific column.
      * @param column is the index of the property to set
      * @param value is the value to apply
      */
    void set( int column, const QVariant& value );

    /** Get pointer to parent item
      * @brief Returns this part's parent item.
      * @return pointer to parent item
      */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item
      * @brief Returns this part's index in the parent's child list.
      * @return row index
      */
    int row() const;


    /**
     * @brief Sets the part's color using RGB values.
     * @param R Red (0–255)
     * @param G Green (0–255)
     * @param B Blue (0–255)
     */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    /**
     * @brief Gets the red component of the color.
     */
    unsigned char getColourR();

    /**
     * @brief Gets the green component of the color.
     */
    unsigned char getColourG();

    /**
     * @brief Gets the blue component of the color.
     */
    unsigned char getColourB();

    /** Set visible flag
     *  @brief Sets the visibility of the part.
      * @param isVisible sets visible/non-visible
      */
    void setVisible(bool isVisible);

    /** Get visible flag
     *  @brief Checks whether the part is visible.
      * @return visible flag as boolean 
      */
    bool visible();
	
	/** Load STL file
     *  @brief Loads an STL file into this part.
      * @param fileName
      */
    void loadSTL(QString fileName);

    /** Return actor
     *  @brief Gets the VTK actor for GUI rendering.
      * @return pointer to default actor for GUI rendering
      */
    vtkSmartPointer<vtkActor> getActor();

    /** Return new actor for use in VR
     *  @brief Creates and returns a new VTK actor for VR rendering.
      * @return pointer to new actor
      */
    vtkActor* getNewActor();

    /**
     * @brief Applies selected VTK filters (clip/shrink) to the part.
     */
    void setFilter();
    /**
     * @brief Checks whether clip filter is enabled.
     */
    bool clip();
    /**
     * @brief Checks whether shrink filter is enabled.
     */
    bool shrink();
    /**
     * @brief Enables or disables the clip filter.
     * @param clip True to enable.
     */
    void setClip(bool clip);
    /**
     * @brief Enables or disables the shrink filter.
     * @param shrink True to enable.
     */
    void setShrink(bool shrink);
    /**
     * @brief Removes and returns the child at the specified row.
     * @param row Index of the child.
     * @return Pointer to the removed child.
     */
    ModelPart* takeChild(int row);

private:
    QList<ModelPart*>                           m_childItems;       /**< List (array) of child items */
    QList<QVariant>                             m_itemData;         /**< List (array of column data for item */
    ModelPart*                                  m_parentItem;       /**< Pointer to parent */

    /* These are some typical properties that I think the part will need, you might
     * want to add you own.
     */
    bool                                        isVisible;          /**< True/false to indicate if should be visible in model rendering */
    bool                                        clipFilter;
    bool                                        shrinkFilter;

	/* These are vtk properties that will be used to load/render a model of this part,
	 * commented out for now but will be used later
	 */
    vtkSmartPointer<vtkSTLReader>               file;               /**< Datafile from which part loaded */
    vtkSmartPointer<vtkMapper>                  mapper;             /**< Mapper for rendering */
    vtkSmartPointer<vtkActor>                   actor;              /**< Actor for rendering */
    vtkColor3<unsigned char>                    colour;             /**< User defineable colour */
};  


#endif

