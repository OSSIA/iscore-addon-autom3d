#include "AutomImage.hpp"
#include <Autom3D/Autom3DModel.hpp>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkSplineWidget2.h>
#include <vtkCubeAxesActor.h>
#include <vtkSplineRepresentation.h>
#include <vtkParametricSpline.h>
#include <vtkWindowToImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKWidget.h>
#include <vtkCamera.h>

namespace Autom3D
{

AutomImage::AutomImage(const ProcessModel &proc):
    m_proc{proc}
{
    m_widget = new QVTKWidget;
    m_renwin = vtkRenderWindow::New();
    m_renwin->SetOffScreenRendering( 1 );
    m_widget->SetRenderWindow(m_renwin);



    QVTKInteractor *iren = m_widget->GetInteractor();

    // add a renderer
    m_renderer = vtkRenderer::New();
    m_renwin->AddRenderer(m_renderer);
    m_renderer->GetActiveCamera()->Zoom(0.5);
    m_renderer->GetActiveCamera()->SetPosition(0, 0, 4);

    // put cone in one window
    m_spline = vtkSplineWidget2::New();
    m_spline->SetInteractor(iren);
    m_spline->On();

    auto cubeAxesActor = vtkCubeAxesActor::New();

    cubeAxesActor->SetCamera(m_renderer->GetActiveCamera());
    m_renderer->AddActor(cubeAxesActor);

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


    con(m_proc, &ProcessModel::handlesChanged,
        this, &AutomImage::on_handlesChanged);
}

AutomImage::~AutomImage()
{
    delete m_widget;
}

QImage AutomImage::getImage() const
{
    //m_widget->saveImageToCache();
    auto  m_img = vtkWindowToImageFilter::New();
    m_img->SetInput(m_renwin);
    m_img->Update();

    auto img = vtkImageDataToQImage(m_img->GetOutput());
    m_img->Delete();
    return img;
}

void AutomImage::on_handlesChanged()
{
    auto rep = vtkSplineRepresentation::SafeDownCast(m_spline->GetRepresentation());
    auto spl = rep->GetParametricSpline();

    spl->SetNumberOfPoints(0);

    auto newPoints = vtkPoints::New();
    for(const Point& pt : m_proc.handles())
    {
        newPoints->InsertNextPoint(pt.x(), pt.y(), pt.z());
    }
    spl->SetNumberOfPoints(newPoints->GetNumberOfPoints());
    rep->InitializeHandles(newPoints);

    emit changed();
}

QImage AutomImage::vtkImageDataToQImage(vtkImageData *p_input) const
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

}
