#include "basic.h"
Vec3f Beam::reflect(const Intersect & intersect) {
    const Vec3f & I = direction;
    const Vec3f & N = intersect.N;
    return I - N*2.f*(I*N);
}

Vec3f Beam::refract(const Intersect & intersect) { // Snell's law
    const Vec3f & I = direction;
    Vec3f N = intersect.N;
    const float eta_t = intersect.material.refractive_index;
    const float eta_i = 1.f;
    float cosi = - std::max(-1.f, std::min(1.f, I*N));
    float eta = eta_i / eta_t;

    if (cosi<0) {
        eta = 1./eta; // if the ray comes from the inside the object, swap the air and the media
        N = -N;
        cosi = -cosi;
    }
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k<0 ? Vec3f(1,0,0) : I*eta + N*(eta*cosi - sqrtf(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

bool Beam::scene_intersect(const Scene & scene, Intersect & intersect){//find minimum contact medium
    float min_dist = std::numeric_limits<float>::max();
    for(size_t i=0; i < scene.p_mediums.size(); i++){
        float dist_i;
        if(scene.p_mediums[i]->intersect(*this,dist_i) && dist_i < min_dist){
            min_dist = dist_i;
            intersect.point = origin + direction*dist_i;
            intersect.material = scene.p_mediums[i]->material;
            intersect.N = scene.p_mediums[i]->get_N(intersect);
        }
    }
    return min_dist < 1000;
}


std::vector<float> Beam::get_light_intensity(Intersect & intersect, const Scene & scene)
{
    std::vector<Light> lights = scene.lights;

    Vec3f N=intersect.N, point = intersect.point;
    Material material = intersect.material;

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i=0; i<lights.size(); i++) {
        Vec3f light_dir      = (lights[i].position - point).normalize();
        float light_distance = (lights[i].position - point).norm();

        Vec3f shadow_orig = light_dir*N < 0 ? point - N*1e-3 : point + N*1e-3; // checking if the point lies in the shadow of the lights[i]
        Intersect shadow_intersect;
        if (Beam(shadow_orig,light_dir,depth+1).scene_intersect(scene,shadow_intersect) && (shadow_intersect.point-shadow_orig).norm() < light_distance)
            continue;

        diffuse_light_intensity  += lights[i].intensity * std::max(0.f, light_dir*N);
        specular_light_intensity += powf(std::max(0.f, Beam(lights[i].position,light_dir,0).reflect(intersect)*direction), material.specular_exponent)*lights[i].intensity;
    }
    std::vector<float> light_intensity;
    light_intensity.push_back(diffuse_light_intensity);
    light_intensity.push_back(specular_light_intensity);
    return light_intensity;
}


Vec3f Beam::get_medium_color(Intersect & intersect,const Scene & scene, std::string optics_type){
    Vec3f N=intersect.N, point = intersect.point;
    Material material = intersect.material;

    Vec3f dir;
    if(optics_type=="refract"){
        dir = refract(intersect).normalize();
        }
    else if(optics_type=="reflect") {
        dir = reflect(intersect).normalize();
        }

    Vec3f orig = dir*N < 0 ? point - N*1e-3 : point + N*1e-3;
    Vec3f color = Beam(orig,dir,depth+1).raytracing(scene);

    return color;
}

Vec3f Beam::raytracing(const Scene & scene){
//get intersect point
    Intersect intersect;
    if (depth>4 || !scene_intersect(scene, intersect)) {
        return scene.background.get_envpix(direction);
    }

//recurse by indirect optics
    Vec3f reflect_color = get_medium_color(intersect,scene,"reflect");
    Vec3f refract_color = get_medium_color(intersect,scene,"refract");
    std::vector<float> light_intensity= get_light_intensity(intersect, scene);
    float diffuse_light_intensity = light_intensity[0];
    float specular_light_intensity = light_intensity[1];

    Material material = intersect.material;

    return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + Vec3f(1., 1., 1.)*specular_light_intensity * material.albedo[1] + reflect_color*material.albedo[2] + refract_color*material.albedo[3];
}





