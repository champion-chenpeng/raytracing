#include "basic.h"

int main(int argc, char ** args){
//basic
    Material      ivory(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),   50.);
    Material      glass(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.);
    Material red_rubber(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1),   10.);
    Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

//camera
    Vec3f cor_origin(0,0,0), neg_z(0,0,-1);
    Grid_Window default_window(M_PI/3.,1024,768);
    Camera camera(cor_origin,neg_z,default_window);
//scene
    Scene scene;
    scene.lights.push_back(Light(Vec3f(-20, 20,  20), 1.5));
    scene.lights.push_back(Light(Vec3f( 30, 50, -25), 1.8));
    scene.lights.push_back(Light(Vec3f( 30, 20,  30), 1.7));

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2,      glass));
    spheres.push_back(Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f( 7,    5,   -18), 4,     mirror));
    for(size_t i=0;i<spheres.size();i++){
        scene.p_mediums.push_back(&spheres[i]);//不能直接p_mediums.push_back(&Sphere), 因为Sphere是临时产生的，没有分配空间；如果先用spheres存储就可以保存指针
    }

    // std::vector<Board> boards;
    // boards.push_back(Board(Vec3f( 0,    -4,   -20), Vec3f(atof(args[2]),atof(args[3]),1).normalize(), 10,   mirror));
    // for(size_t i=0;i<boards.size();i++){
    //     scene.p_mediums.push_back(&boards[i]);//不能直接p_mediums.push_back(&Sphere), 因为Sphere是临时产生的，没有分配空间；如果先用spheres存储就可以保存指针
    // }
//background
    char infile[100],outfile[100];
    char * bkg_pic=args[1];
    strcpy(infile,bkg_pic);
    strcat(infile,".jpg");

    strcpy(outfile,bkg_pic);
    strcat(outfile,"_out.jpg");

    scene.background = Background(infile,M_PI/3.);
//render and save
    std::vector<Vec3f> framebuffer = camera.render(scene);
    camera.save(outfile, framebuffer);
    return 0;
}