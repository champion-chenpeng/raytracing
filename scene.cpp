#include "basic.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif
Background::Background(char * infile, float fov){
    window.fov = fov;
    int n = -1;
    unsigned char *pixmap = stbi_load(infile, &window.width, &window.height, &n, 0);
    if (!pixmap || 3!=n) {
        std::cerr << "Error: can not load the environment map" << std::endl;
    }
    envmap = std::vector<Vec3f>(window.width*window.height);
    for (int j = window.height-1; j>=0 ; j--) {
        for (int i = 0; i<window.width; i++) {
            envmap[i+j*window.width] = Vec3f(pixmap[(i+j*window.width)*3+0], pixmap[(i+j*window.width)*3+1], pixmap[(i+j*window.width)*3+2])*(1/255.);
        }
    }
    stbi_image_free(pixmap);
}

float normalize(const Vec3f & dir,const char & component, float fov){//->[0,1], center 1/2
    float dir_z = dir.z, dir_ = dir.y;
    if(component=='x'){
        dir_ = -dir.x;
        dir_z = -dir.z;
    }//swap z and -x
    if(fabs(dir_)+fabs(dir_z) < 1e-50) return 1/2;
    int sgn = int(dir_>0) - int(dir_<0);
    float angle = atan(dir_z/dir_) + sgn * M_PI/2;//[-pi,pi]
    float fov_normal = tan(angle/M_PI*fov/2)/tan(fov/2);
    if(component=='y') fov_normal = -fov_normal;
    return 1/2.*(1 + fov_normal);
}
Vec3f Background::get_envpix(const Vec3f & dir) const{//若仅将背景定义在-z方向前2PI立体角内，不考虑相机-z方向窗格背后的窗格，以下定义会导致背后的背景为正面M_PI/2的反演世界。
    float fov_x = 2*atan(window.width/window.height*tan(window.fov/2));
    int i = normalize(dir,'x',fov_x) * window.width;
    int j = normalize(dir,'y',window.fov) * window.height;
    if(i>=0 && i < window.width && j>=0 && j < window.height) return envmap[i + j * window.width];
    return Vec3f(0.2, 0.7, 0.8); // outrange background color
}
bool Sphere::intersect(Beam & beam,float & dist) const {
    Vec3f orig = beam.origin, dir = beam.direction;
    float & t0 = dist;

    Vec3f L = center - orig;
    float tca = L*dir;
    float d2 = L*L - tca*tca;
    if (d2 > radius*radius) return false;
    float thc = sqrtf(radius*radius - d2);
    t0       = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    return true;
}
Vec3f Sphere::get_N(const Intersect & intersect) const{
    return (intersect.point - center).normalize();
}

bool Board::intersect(Beam & beam,float & dist) const {
    Vec3f center_ = center - beam.origin;//direct to center

    float cos_beam = Normal*beam.direction;
    float h = center_*Normal;

    if(fabs(cos_beam) < 1e-5) return false;//avoid /0, or do not view the parallel board
    dist = h/cos_beam;
    if(dist < 0) return false; //beam is not in the direction to the center(or board)
    Vec3f pt = beam.origin + beam.direction * dist;
    if((pt-center).norm() > radius) return false;
    return true;
}

Vec3f Board::get_N(const Intersect & intersect) const{
    return Normal;
}