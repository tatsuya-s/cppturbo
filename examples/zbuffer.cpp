#include <iostream>
#include <limits>
#include <vector>
#include <array>
#include <vtkSmartPointer.h>
#include <vtkPLYReader.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageShiftScale.h>
#include <vtkPNGWriter.h>
#include "turbo_colormap.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " input(.ply) output(.png)" << std::endl;
        return EXIT_FAILURE;
    }

    vtkNew<vtkPLYReader> poly_reader;
    poly_reader->SetFileName(argv[1]);

    vtkNew<vtkPolyDataMapper> poly_mapper;
    poly_mapper->SetInputConnection(poly_reader->GetOutputPort());

    vtkNew<vtkActor> poly_actor;
    poly_actor->SetMapper(poly_mapper);

    vtkNew<vtkRenderer> poly_renderer;
    poly_renderer->AddActor(poly_actor);

    vtkNew<vtkRenderWindow> render_window;
    render_window->SetSize(512, 512); 
    render_window->AddRenderer(poly_renderer);
    render_window->Render();

    vtkNew<vtkWindowToImageFilter> filter;
    filter->SetInput(render_window);
    filter->SetScale(1);
    filter->SetInputBufferTypeToZBuffer();

    vtkNew<vtkImageShiftScale> scale;
    scale->SetOutputScalarTypeToDouble();
    scale->SetInputConnection(filter->GetOutputPort());
    scale->SetShift(0);
    scale->Update();

    vtkSmartPointer<vtkImageData> zbuffer = scale->GetOutput();
    int* dims = zbuffer->GetDimensions();
    vtkNew<vtkImageData> image;
    image->SetDimensions(dims[0], dims[1], 1);
    image->AllocateScalars(VTK_UNSIGNED_CHAR, 4);

    double* arr = reinterpret_cast<double*>(zbuffer->GetScalarPointer());
    std::vector<double> vec(arr, arr + dims[0] * dims[1]);
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    for (const double z : vec) {
        if (z >= 1.0) {
            continue;
        }
        min = min > z ? z : min;
        max = max < z ? z : max;
    }

    for (int y = 0; y < dims[1]; y++) {
        for (int x = 0; x < dims[0]; x++) {
            unsigned char* pixel = reinterpret_cast<unsigned char*>(image->GetScalarPointer(x, y, 0));
            turbo::Color3b uchars;
            double alpha = 255;

            if (y < 30) {
                int index  = static_cast<int>(x + 16) / 32 * 16;
                turbo::GetColor(index, uchars);
            }
            else if (y < 60) {
                turbo::GetColor(x / 2, uchars);
            }
            else {
                double z = vec[(y - 30) * dims[1] + x];
                double z_new = z < 1.0 ? 1 - ((z - min) / (max - min)) : 0;
                turbo::GetColor(z_new, uchars);
                alpha = z < 1.0 ? 255 : 0;
            }

            pixel[0] = uchars.r;
            pixel[1] = uchars.g;
            pixel[2] = uchars.b;
            pixel[3] = alpha;
        }
    }

    vtkNew<vtkPNGWriter> writer;
    writer->SetFileName(argv[2]);
    writer->SetInputData(image);
    writer->Write();

    vtkNew<vtkDataSetMapper> image_mapper;
    image_mapper->SetInputData(image);

    vtkNew<vtkActor> image_actor;
    image_actor->SetMapper(image_mapper);

    vtkNew<vtkRenderer> image_renderer;
    image_renderer->AddActor(image_actor);

    render_window->SetSize(1024, 512);
    render_window->AddRenderer(image_renderer);

    std::array<double, 4> left_viewport = {0.0, 0.0, 0.5, 1.0};
    std::array<double, 4> right_viewport = {0.5, 0.0, 1.0, 1.0};
    poly_renderer->SetViewport(left_viewport.data());
    image_renderer->SetViewport(right_viewport.data());

    vtkNew<vtkRenderWindowInteractor> interactor;
    vtkNew<vtkInteractorStyleImage> style;
    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(render_window);

    poly_renderer->ResetCamera();
    image_renderer->ResetCamera();
    render_window->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}
