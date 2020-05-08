#include <vtkActor.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkColorTransferFunction.h>
#include "cppturbo.hpp"

void getTurboLUT(vtkSmartPointer<vtkColorTransferFunction> lut, const double min_value = 0.0, const double max_value = 1.0)
{
    lut->RemoveAllPoints();
    lut->SetColorSpaceToHSV();

    const int num_colors = 256;
    for (int i = 0; i < num_colors; ++i) 
    {
        turbo::Color3d color;
        turbo::GetColor(i, color);

        const double x = min_value + ((max_value - min_value) / (num_colors - 1)) * i;
        lut->AddRGBPoint(x, color.r, color.g, color.b);
    }
}

int main()
{
    vtkNew<vtkSphereSource> sphere;
    sphere->SetCenter(0, 0, 0);
    sphere->SetRadius(1);
    sphere->SetPhiResolution(30);
    sphere->SetThetaResolution(60);
    sphere->Update();

    // Create scalar data to associate with the vertices of the sphere
    int num_pts = sphere->GetOutput()->GetPoints()->GetNumberOfPoints();
    vtkNew<vtkFloatArray> scalars;
    scalars->SetNumberOfValues(num_pts);
    for (int i = 0; i < num_pts; ++i)
    {
        scalars->SetValue(i, static_cast<double>(i) / (num_pts - 1));
    }

    vtkNew<vtkPolyData> poly;
    poly->DeepCopy(sphere->GetOutput());
    poly->GetPointData()->SetScalars(scalars);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(poly);
    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUsePointData();
    mapper->SetColorModeToMapScalars();

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    vtkNew<vtkScalarBarActor> scalar_bar;
    scalar_bar->SetLookupTable(mapper->GetLookupTable());
    scalar_bar->SetTitle("Turbo");
    scalar_bar->SetNumberOfLabels(5);

    // Create a lookup table to share between the mapper and the scalarbar
    vtkNew<vtkColorTransferFunction> lut;
    getTurboLUT(lut);

    mapper->SetLookupTable(lut);
    scalar_bar->SetLookupTable(lut);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->AddActor2D(scalar_bar);

    vtkNew<vtkRenderWindow> render_window;
    render_window->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(render_window);

    render_window->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}
