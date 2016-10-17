#include "AutomWidget.hpp"


#include <QMenu>

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkConeSource.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkTDxInteractorStyleCamera.h"
#include "vtkTDxInteractorStyleSettings.h"
#include "QVTKInteractor.h"
#include <QVBoxLayout>
#include <QVTKWidget.h>
#include <QVTKWidget2.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>
#include <vtkSplineWidget.h>
#include <vtkSplineWidget2.h>
#include <vtkCubeAxesActor.h>
#include <vtkSplineRepresentation.h>
#include <vtkParametricSpline.h>
#include <vtkWindowToImageFilter.h>
#include <vtkSmartPointer.h>

#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <Autom3D/Commands/ChangeAddress.hpp>
#include <Autom3D/Autom3DModel.hpp>
#include <iscore/document/DocumentContext.hpp>
namespace Autom3D
{

AutomWidget::AutomWidget(
        const ProcessModel& proc,
        const iscore::DocumentContext& doc,
        QWidget* parent):
    QWidget{parent},
    m_disp{doc.commandStack},
    m_proc{proc}
{
    m_widget = new QVTKWidget;
    /*m_widget->setAutomaticImageCacheEnabled(true);
    m_widget->setMaxRenderRateForImageCache(10);
    connect(m_widget, &QVTKWidget2::cachedImageClean,
            this, &AutomWidget::on_cachedImageClean);
            */
    auto lay = new QVBoxLayout;
    this->setLayout(lay);
    lay->addWidget(m_widget);

    // create a window to make it stereo capable and give it to QVTKWidget
    auto renwin = vtkRenderWindow::New();
    //renwin->SetOffScreenRendering( 1 );

    m_widget->SetRenderWindow(renwin);

    QVTKInteractor *iren = m_widget->GetInteractor();

    // add a renderer
    m_renderer = vtkRenderer::New();
    m_widget->GetRenderWindow()->AddRenderer(m_renderer);

    // put cone in one window
    m_spline = vtkSplineWidget2::New();
    m_spline->SetInteractor(iren);
    m_spline->On();

    auto cubeAxesActor = vtkCubeAxesActor::New();

    cubeAxesActor->SetCamera(m_renderer->GetActiveCamera());
    m_renderer->AddActor(cubeAxesActor);
    m_connections = vtkEventQtSlotConnect::New();

    cubeAxesActor->XAxisLabelVisibilityOff();
    cubeAxesActor->YAxisLabelVisibilityOff();
    cubeAxesActor->ZAxisLabelVisibilityOff();

    cubeAxesActor->XAxisTickVisibilityOff();
    cubeAxesActor->YAxisTickVisibilityOff();
    cubeAxesActor->ZAxisTickVisibilityOff();

    cubeAxesActor->XAxisMinorTickVisibilityOff();
    cubeAxesActor->YAxisMinorTickVisibilityOff();
    cubeAxesActor->ZAxisMinorTickVisibilityOff();
    // update coords as we move through the window

    m_img = vtkWindowToImageFilter::New();
    m_img->SetInput(renwin);


    m_connections->Connect(m_spline,
                           vtkCommand::StartInteractionEvent,
                           this,
                           SLOT(press(vtkObject*)));

    m_connections->Connect(m_spline,
                           vtkCommand::EndInteractionEvent,
                           this,
                           SLOT(release(vtkObject*)));


    //renwin->Delete();

    con(m_proc, &ProcessModel::handlesChanged,
        this, &AutomWidget::on_handlesChanged);
    on_handlesChanged();
}

AutomWidget::~AutomWidget()
{
    m_renderer->Delete();
    m_connections->Delete();
}

QImage AutomWidget::getImage() const
{
    //m_widget->saveImageToCache();
    m_img->Update();
    return vtkImageDataToQImage(m_img->GetOutput());
}


void AutomWidget::press(vtkObject* obj)
{
}

void AutomWidget::release(vtkObject* obj)
{
    auto spl = vtkSplineRepresentation::SafeDownCast(m_spline->GetRepresentation())->GetParametricSpline();
    auto& pts = *spl->GetPoints();

    std::vector<State::vec3f> handles;
    int n = pts.GetNumberOfPoints();
    handles.reserve(n);
    for(int i = 0; i < n; i++)
    {
        double pt[3];
        pts.GetPoint(i, pt);
        handles.push_back({pt[0], pt[1], pt[2]});
    }

    auto cmd = new UpdateSpline{m_proc, std::move(handles)};
    m_disp.submitCommand(cmd);
}

void AutomWidget::on_cachedImageClean()
{
    //m_cache = vtkImageDataToQImage(m_widget->cachedImage());
}

QImage AutomWidget::vtkImageDataToQImage(vtkImageData *p_input) const
{
    if (!p_input)
        return QImage();

    const int width = p_input->GetDimensions()[0];
    const int height = p_input->GetDimensions()[1];

    QImage image(width, height, QImage::Format_RGB32);
    QRgb* rgbPtr = reinterpret_cast<QRgb*>(image.bits()) +width * (height-1);
    unsigned char* colorsPtr = reinterpret_cast<unsigned char*>(p_input->GetScalarPointer());

    // mirror vertically
    for(int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            // Swap rgb
            *(rgbPtr++) = QColor(colorsPtr[0], colorsPtr[1], colorsPtr[2]).rgb();
            colorsPtr +=  3;
        }
        rgbPtr -= width * 2;
    }
    return image;
}

void AutomWidget::on_handlesChanged()
{
    auto rep = vtkSplineRepresentation::SafeDownCast(m_spline->GetRepresentation());
    auto spl = rep->GetParametricSpline();

    spl->SetNumberOfPoints(0);

    auto newPoints = vtkPoints::New();
    for(const State::vec3f& pt : m_proc.handles())
    {
        newPoints->InsertNextPoint(pt[0], pt[1], pt[2]);
    }
    spl->SetNumberOfPoints(newPoints->GetNumberOfPoints());
    rep->InitializeHandles(newPoints);
    m_widget->update();

}
}
