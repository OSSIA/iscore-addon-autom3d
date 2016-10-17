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
#include <vtkOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKWidget.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkGraphicsFactory.h>
#include <vtkPolyDataMapper.h>
namespace Autom3D
{

AutomImage::AutomImage(const ProcessModel &proc):
    m_proc{proc}
{
    m_widget = new QVTKWidget;
    m_renwin = vtkRenderWindow::New();
    m_renwin->SetOffScreenRendering( 1 );
    m_renwin->SetSize(1024, 768);
    m_widget->SetRenderWindow(m_renwin);



    QVTKInteractor *iren = m_widget->GetInteractor();

    // add a renderer
    m_renderer = vtkRenderer::New();
    m_renwin->AddRenderer(m_renderer);
    m_renwin->SetAlphaBitPlanes(1);
    m_renderer->GetActiveCamera()->Zoom(0.75);
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
    on_handlesChanged();
}

AutomImage::~AutomImage()
{
}

QImage AutomImage::getImage(int w, int h) const
{
    m_renwin->Render();

    auto  m_img = vtkWindowToImageFilter::New();
    m_img->SetInput(m_renwin);
    m_img->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    //m_img->ReadFrontBufferOff(); // read from the back buffer
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
    for(const State::vec3f& pt : m_proc.handles())
    {
        newPoints->InsertNextPoint(pt[0], pt[1], pt[2]);
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

    QImage image(width, height, QImage::Format_ARGB32);
    QRgb* rgbPtr = reinterpret_cast<QRgb*>(image.bits()) +width * (height-1);
    unsigned char* colorsPtr = reinterpret_cast<unsigned char*>(p_input->GetScalarPointer());

    // mirror vertically
    for(int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            // Swap rgb
            *(rgbPtr++) = qRgba(colorsPtr[0], colorsPtr[1], colorsPtr[2], colorsPtr[3]);
            colorsPtr +=  4;
        }
        rgbPtr -= width * 2;
    }
    return image;
}

}
