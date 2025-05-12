/**
 * @file VRRenderThread.h
 * @brief Declaration of the VRRenderThread class for multithreaded OpenVR rendering.
 * @details This class handles VR rendering in a separate thread using VTK's OpenVR module and Qt's QThread.
 *          It enables asynchronous control and animation of 3D actors in a virtual scene.
 * @version 1.0.0
 * @date 2025-05-12/2022
 * @author Woojin, Zhixing, Zhiyuan/Paul
 */
#ifndef VR_RENDER_THREAD_H
#define VR_RENDER_THREAD_H

/* Project headers */

/* Qt headers */
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/* Vtk headers */
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>				
#include <vtkOpenVRRenderWindowInteractor.h>	
#include <vtkOpenVRRenderer.h>					
#include <vtkOpenVRCamera.h>	
#include <vtkActorCollection.h>
#include <vtkCommand.h>



/* Note that this class inherits from the Qt class QThread which allows it to be a parallel thread
 * to the main() thread, and also from vtkCommand which allows it to act as a "callback" for the 
 * vtkRenderWindowInteractor. This callback functionallity means that once the renderWindowInteractor
 * takes control of this thread to enable VR, it can callback to a function in the class to check to see
 * if the user has requested any changes
 */
/**
 * @class VRRenderThread
 * @brief Thread class for managing a VTK OpenVR render loop separate from the GUI.
 * @details Inherits from QThread to support background processing, and manages VTK VR actors and interactions.
 */
class VRRenderThread : public QThread {
    Q_OBJECT

public:
    /** List of command names */
    /**
     * @brief Enumeration of command types to issue to the rendering thread.
     */
    enum {
        END_RENDER,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z
    } Command;


    /**
     * @brief Constructs the VRRenderThread instance.
     * @param parent Optional parent QObject.
     */
    VRRenderThread(QObject* parent = nullptr);

    /**
     * @brief Destructor for cleaning up resources.
     */
    ~VRRenderThread();


    /**
     * @brief Adds a VTK actor to be rendered by the thread before VR is started.
     * @param actor Pointer to the VTK actor.
     */
    void addActorOffline(vtkActor* actor);


    /**
     * @brief Issues a rendering command to the thread safely.
     * @param cmd Command type.
     * @param value Associated value (e.g., rotation angle).
     */
    void issueCommand( int cmd, double value );


protected:
    /**
     * @brief Re-implemented thread run method that initializes and executes the VR render loop.
     */
    void run() override;

private:
    vtkSmartPointer<vtkOpenVRRenderWindow> window;              /**< VR render window */
    vtkSmartPointer<vtkOpenVRRenderWindowInteractor> interactor;/**< VR input interactor */
    vtkSmartPointer<vtkOpenVRRenderer> renderer;                /**< Scene renderer */
    vtkSmartPointer<vtkOpenVRCamera> camera;                    /**< Active VR camera */

    QMutex mutex;                                               /**< Synchronization mutex */
    QWaitCondition condition;                                   /**< Synchronization wait condition */

    vtkSmartPointer<vtkActorCollection> actors;                 /**< Collection of actors to render */

    std::chrono::time_point<std::chrono::steady_clock> t_last; /**< Timestamp for animation timing */

    bool endRender;                                             /**< Flag to signal render loop termination */

    double rotateX; /**< Rotation amount around X axis (degrees) */
    double rotateY; /**< Rotation amount around Y axis (degrees) */
    double rotateZ; /**< Rotation amount around Z axis (degrees) */
};
};



#endif
