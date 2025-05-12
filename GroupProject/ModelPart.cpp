/**
 * @file ModelPart.cpp
 * @brief Implementation of the ModelPart class for representing tree view elements and VTK-rendered parts.
 * @details This file defines the functionality of model parts including hierarchy, rendering, filtering, and color control.
 * @version 1.0.0
 * @author Woojin, Zhixing, Zhiyuan/Paul
 * @date 2025-05-12/2022
 */

#include "ModelPart.h"


/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>


/**
 * @brief Constructor for ModelPart.
 * @param data List of property values.
 * @param parent Pointer to the parent ModelPart in the tree.
 */
ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent), isVisible(true),
    clipFilter(false), shrinkFilter(false),
    actor(nullptr), mapper(nullptr)  {
    colour.Set(100,100,100);
    /* You probably want to give the item a default colour */
}

/**
 * @brief Destructor. Cleans up all child items.
 */
ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

/**
 * @brief Adds a child ModelPart to this part.
 * @param item Pointer to the new child item.
 */
void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}

/**
 * @brief Retrieves the child item at the given row.
 * @param row Index of the child to retrieve.
 * @return Pointer to the child ModelPart, or nullptr if out of range.
 */
ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}
/**
 * @brief Returns the number of child items.
 */
int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}

/**
 * @brief Returns the number of columns this part holds.
 */
int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}
/**
 * @brief Retrieves the data for a specific column.
 * @param column Column index.
 * @return The data value.
 */
QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

/**
 * @brief Sets the data for a specific column.
 * @param column Column index.
 * @param value New data value.
 */
void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

/**
 * @brief Returns the parent item in the tree hierarchy.
 */
ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

/**
 * @brief Returns the index of this item in its parent's child list.
 */
int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}
/**
 * @brief Sets the display color of this part.
 * @param R Red component [0–255].
 * @param G Green component [0–255].
 * @param B Blue component [0–255].
 */
void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    colour.Set(R, G, B);
    /* As the name suggests ... */
}
/**
 * @brief Returns the red component of the current color.
 */
unsigned char ModelPart::getColourR() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetRed();
}

/**
 * @brief Returns the green component of the current color.
 */
unsigned char ModelPart::getColourG() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetGreen();
}

/**
 * @brief Returns the blue component of the current color.
 */
unsigned char ModelPart::getColourB() {
   /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return colour.GetBlue();
}


/**
 * @brief Sets the visibility of this model part.
 * @param visible Boolean value indicating visibility.
 */
void ModelPart::setVisible(bool visible) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    isVisible=visible;
    /* As the name suggests ... */
}
/**
 * @brief Returns the visibility state.
 */
bool ModelPart::visible() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return isVisible;
}

/**
 * @brief Loads an STL file and creates associated VTK mapper and actor.
 * @param fileName Path to the STL file.
 */
void ModelPart::loadSTL( QString fileName ) {
    /* This is a placeholder function that you will need to modify if you want to use it */

    /* 1. Use the vtkSTLReader class to load the STL file
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    /* 2. Initialise the part's vtkMapper */
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());


    /* 3. Initialise the part's vtkActor and link to the mapper */

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(
        colour.GetRed()   / 255.0,
        colour.GetGreen() / 255.0,
        colour.GetBlue()  / 255.0
        );
    actor->SetVisibility(isVisible);
}
/**
 * @brief Returns the current VTK actor for GUI rendering.
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    return actor;
    /* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
}
/**
 * @brief Creates a new VTK actor for VR rendering, using a fresh mapper.
 * @return Pointer to the new VTK actor.
 */
vtkActor* ModelPart::getNewActor() {
    /* This is a placeholder function that you will need to modify if you want to use it
     * 
     * The default mapper/actor combination can only be used to render the part in 
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */
     
     
     /* 1. Create new mapper */
    auto newMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    newMapper->SetInputConnection(file->GetOutputPort());
     
     /* 2. Create new actor and link to mapper */
    auto newActor = vtkSmartPointer<vtkActor>::New();
    newActor->SetMapper(newMapper);
     
     /* 3. Link the vtkProperties of the original actor to the new actor. This means 
      *    if you change properties of the original part (colour, position, etc), the
      *    changes will be reflected in the GUI AND VR rendering.
      *    
      *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
      *    functions.
      */
    newActor->SetProperty(actor->GetProperty());
    newActor->SetVisibility(isVisible);
    

    /* The new vtkActor pointer must be returned here */
    return newActor;
    
}
/**
 * @brief Applies VTK filters (clip, shrink) based on current filter flags.
 */
void ModelPart::setFilter()
{
    if (!file) {
        return;
    }

    if (clipFilter && shrinkFilter) {

        vtkSmartPointer<vtkPlane> planeLeft =
            vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(0, 1, 0.0);

        vtkSmartPointer<vtkClipDataSet> clip = vtkSmartPointer<vtkClipDataSet>::New();
        clip->SetInputConnection(file->GetOutputPort());
        clip->SetClipFunction(planeLeft.Get());

        vtkSmartPointer<vtkShrinkFilter> shrink = vtkSmartPointer<vtkShrinkFilter>::New();
        shrink->SetInputConnection(clip->GetOutputPort());
        shrink->SetShrinkFactor(0.8);
        shrink->Update();

        mapper->SetInputConnection(shrink->GetOutputPort());
    }

    else if (clipFilter) {
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(0.0, 1, 0.0);

        vtkSmartPointer<vtkClipDataSet> clip = vtkSmartPointer<vtkClipDataSet>::New();
        clip->SetInputConnection(file->GetOutputPort());
        clip->SetClipFunction(planeLeft.Get());

        mapper->SetInputConnection(clip->GetOutputPort());
    }

    else if (shrinkFilter) {
        vtkSmartPointer<vtkShrinkFilter> shrink = vtkSmartPointer<vtkShrinkFilter>::New();
        shrink->SetInputConnection(file->GetOutputPort());
        shrink->SetShrinkFactor(0.8);
        shrink->Update();

        mapper->SetInputConnection(shrink->GetOutputPort());
    }

    else {
        mapper->SetInputConnection(file->GetOutputPort());
    }

    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(
        colour.GetRed()   / 255.0,
        colour.GetGreen() / 255.0,
        colour.GetBlue()  / 255.0
        );
    actor->SetVisibility(isVisible);
}
/**
 * @brief Returns whether clipping is enabled.
 */
bool ModelPart::clip() {
    return clipFilter;
}
/**
 * @brief Returns whether shrinking is enabled.
 */
bool ModelPart::shrink() {
    return shrinkFilter;
}

/**
 * @brief Enables or disables clipping filter.
 * @param clip Boolean toggle.
 */
void ModelPart::setClip(bool clip) {
    clipFilter = clip;
}

/**
 * @brief Enables or disables shrink filter.
 * @param shrink Boolean toggle.
 */
void ModelPart::setShrink(bool shrink) {
    shrinkFilter = shrink;
}
/**
 * @brief Removes and returns a child at the specified row.
 * @param row Row index of the child.
 * @return Pointer to the removed child, or nullptr if invalid.
 */
ModelPart* ModelPart::takeChild(int row) {
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.takeAt(row);
}
