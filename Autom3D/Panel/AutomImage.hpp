#pragma once
#include <QObject>
class QVTKWidget;
class vtkRenderer;
class vtkSplineWidget2;
class vtkWindowToImageFilter;
class vtkImageData;
class vtkRenderWindow;
class vtkCubeAxesActor;
namespace Autom3D
{

    class ProcessModel;
class AutomImage : public QObject
{
    const ProcessModel& m_proc;

    QVTKWidget* m_widget{};
    vtkRenderer* m_renderer{};
    vtkSplineWidget2* m_spline{};
    vtkRenderWindow* m_renwin{};
    vtkCubeAxesActor* m_axes{};

    Q_OBJECT
public:
    AutomImage(const ProcessModel& proc);
    ~AutomImage();


    QImage getImage(int w, int h) const;
    void on_handlesChanged();
    QImage vtkImageDataToQImage(vtkImageData *p_input) const;

signals:
    void changed();
};
}
