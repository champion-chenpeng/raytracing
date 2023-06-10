#include "basic.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

std::vector<Vec3f> Camera::render(const Scene & scene){
    std::vector<Vec3f> framebuffer(window.width*window.height);
    #pragma omp parallel for
    for (size_t j = 0; j<window.height; j++) { // actual rendering loop
        for (size_t i = 0; i<window.width; i++) {
            float dir_x =  (i + 0.5) -  window.width/2.;
            float dir_y = -(j + 0.5) + window.height/2.;    // this flips the image at the same time
            float dir_z = -window.height/(2.*tan(window.fov/2.));
            Beam beam(Vec3f(0,0,0), Vec3f(dir_x, dir_y, dir_z).normalize(),0);
            framebuffer[i+j*window.width] = beam.raytracing(scene);
        }
    }
    return framebuffer;
}
void Camera::save(char * outfile, std::vector<Vec3f> framebuffer){
    std::vector<unsigned char> pixmap(window.width*window.height*3);
    for (size_t i = 0; i < window.height*window.width; ++i) {
        Vec3f &c = framebuffer[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max>1) c = c*(1./max);
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    stbi_write_jpg(outfile, window.width, window.height, 3, pixmap.data(), 100);
}