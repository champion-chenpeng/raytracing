#include "geometry.h"

//-----------------basic----------------------
struct Material {//Scene
    Material(const float r, const Vec4f &a, const Vec3f &color, const float spec) : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1,0,0,0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
};

class Intersect{//Beam
public:
    Vec3f point, N;
    Material material;
};

class Grid_Window{//Background and Camera
public:
    float fov;
    int width, height;
    Grid_Window(){}
    Grid_Window(float f, int w, int h):fov(f),width(w),height(h){}
};

//-----------------Scene----------------------
struct Light {
    Light(const Vec3f &p, const float i) : position(p), intensity(i) {}
    Vec3f position;
    float intensity;
};

class Background{
public:
    Grid_Window window;
    std::vector<Vec3f> envmap;

    Background(){}
    Background(Grid_Window g, std::vector<Vec3f> e): window(g),envmap(e){}
    Background(char * infile, float fov=M_PI/3.);
    Vec3f get_envpix(const Vec3f & dir) const;
};
class Medium;
class Scene{
public:
    std::vector<Light> lights;
    std::vector<Medium *> p_mediums;
    Background background;
    Scene(){};
};

//-----------------Medium----------------------
class Beam;
class Medium{
public:
    Material material;
    
    Medium(Material m):material(m){}
    virtual ~Medium(){}
    virtual bool intersect(Beam & beam,float & dist) const = 0;
    virtual Vec3f get_N(const Intersect & intersect) const =0;
};
class Sphere: public Medium {
private://geometry
    Vec3f center;
    float radius;
public:
    Sphere(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), Medium(m){
    }
    ~Sphere(){}
    bool intersect(Beam & beam,float & dist) const;
    Vec3f get_N(const Intersect & intersect) const;
};
// class Mesh: public Medium{

// };
class Board: public Medium{
private://geometry
    Vec3f center, Normal;
    float radius;
public:
    Board(const Vec3f &c, const Vec3f & N,const float r, const Material &m) : center(c), Normal(N), radius(r), Medium(m){
    }
    ~Board(){}
    bool intersect(Beam & beam,float & dist) const;
    Vec3f get_N(const Intersect & intersect) const;
};

//-----------------Beam----------------------
class Beam{
public:
    Vec3f origin,direction;
    int depth;
    Beam(Vec3f o, Vec3f dir, int d):origin(o),direction(dir),depth(d) {}
//method
    Vec3f reflect(const Intersect & intersect);
    Vec3f refract(const Intersect & intersect);
    // transmit

    bool scene_intersect(const Scene & scene, Intersect & intersect);
    Vec3f raytracing(const Scene & scene);
    
    Vec3f get_medium_color(Intersect & intersect,const Scene & scene, std::string optics_type);
    std::vector<float> get_light_intensity(Intersect & intersect, const Scene & scene);
};

//-----------------Camera----------------------
class Camera{
    Vec3f position,direction;//it is not enought to decide a camera, rotation around the central direction is needed;
    Grid_Window window;
public:
//get member reference(to use or modify)
    Vec3f & get_pos(){
        return position;
    }
    Vec3f & get_dir(){
        return direction;
    }
    Grid_Window & get_window(){
        return window;
    }
//construct function
    Camera(){};
    Camera(Vec3f p, Vec3f d, Grid_Window gw):position(p),direction(d),window(gw){}
//other method
    std::vector<Vec3f> render(const Scene & scene);
    void save(char * outfile, std::vector<Vec3f> framebuffer);
};