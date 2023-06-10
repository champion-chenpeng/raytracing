# Ray Tracing

This project demonstrates ray tracing using the C++ programming language. The code implements a basic ray tracing algorithm to render a scene with various materials and lighting.

## Prerequisites

To run this project, you need to have the following installed on your system:

- C++ compiler
- STB Image library (stb_image.h)
- STB Image Write library (stb_image_write.h)

## Getting Started

1. Clone the project repository to your local machine.
2. Ensure that the `basic.h`, `beam.cpp`, `camera.cpp`, and `scene.cpp` files are in the same directory as the `raytracing.cpp` file.
3. Compile the project using the C++ compiler.

## Usage

The `raytracing.cpp` file contains the main function and the scene setup. You can modify the scene parameters and camera settings to create different ray-traced images.

### Scene Setup

The scene setup includes defining materials, lights, camera parameters, and the scene objects (spheres and boards). You can customize these settings in the `main` function of `raytracing.cpp`:

- Materials: Define different materials using the `Material` struct. Each material has properties such as refractive index, albedo, diffuse color, and specular exponent.
- Lights: Define the position and intensity of lights in the scene using the `Light` struct. Add lights to the `scene.lights` vector.
- Camera: Specify the camera position, direction, and the grid window dimensions using the `Camera` class. Modify the `cor_origin`, `neg_z`, and `default_window` variables to adjust the camera settings.
- Scene Objects: Create spheres and boards in the scene by adding instances of the `Sphere` and `Board` classes to the `spheres` vector. Assign the desired material and position for each object.

### Rendering and Saving

The ray tracing process and saving the rendered image are handled in the `main` function:

1. The `camera.render(scene)` function generates the ray-traced image by casting rays from the camera's viewpoint and tracing their interactions with the scene objects.
2. The rendered image is saved to a file using the `camera.save(outfile, framebuffer)` function. Provide the output file path as the `outfile` parameter.

## Examples

Here are a few examples of how you can modify the scene setup to create different images:

- Change the materials of the objects to create different reflective, refractive, or diffuse surfaces.
- Adjust the camera position, direction, and field of view to change the perspective and framing of the scene.
- Add or remove lights to alter the lighting conditions in the scene.
- Modify the properties of the scene objects, such as their positions and sizes, to change the arrangement and composition of the scene.

Feel free to experiment with the code and explore the possibilities of ray tracing.

## Acknowledgements

This project utilizes the STB Image and STB Image Write libraries for image processing. These libraries provide a simple and lightweight solution for reading and writing image files in various formats.

For more information on STB Image and STB Image Write, please refer to the following repositories:

- STB Image: [https://github.com/nothings/stb/blob/master/stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
- STB Image Write: [https://github.com/nothings/stb/blob/master/stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)
