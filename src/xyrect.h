#ifndef RECTH
#define RECTH

#include "hitable.h"

class xy_rect : public hitable {
public:
  xy_rect() {}
  xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) :
    x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng);
  virtual bool bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(vec3(x0,y0,k-0.0001), vec3(x1,y1,k+0.0001));
    return(true);
  }
  virtual float pdf_value(const vec3& o, const vec3& v, random_gen& rng) {
    hit_record rec;
    if(this->hit(ray(o,v), 0.001, FLT_MAX, rec, rng)) {
      float area = (x1-x0)*(y1-y0);
      float distance_squared = rec.t * rec.t * v.squared_length();
      float cosine = fabs(dot(v,rec.normal)/v.length());
      return(distance_squared / (cosine * area));
    } else {
      return(0);
    }
  }
  virtual vec3 random(const vec3& o, random_gen& rng) {
    vec3 random_point = vec3(x0 + rng.unif_rand() * (x1 - x0), y0 + rng.unif_rand() * (y1-y0),k);
    return(random_point - o);
  }
  float x0, x1, y0, y1, k;
  material *mp;
};

class xz_rect : public hitable {
public:
  xz_rect() {}
  xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) :
  x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng);
  virtual bool bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(vec3(x0,k-0.0001,z0), vec3(x1,k+0.0001,z1));
    return(true);
  }
  virtual float pdf_value(const vec3& o, const vec3& v, random_gen& rng) {
    hit_record rec;
    if(this->hit(ray(o,v), 0.001, FLT_MAX, rec, rng)) {
      float area = (x1-x0)*(z1-z0);
      float distance_squared = rec.t * rec.t * v.squared_length();
      float cosine = fabs(dot(v, rec.normal)/v.length());
      return(distance_squared / (cosine * area));
    } else {
      return(0);
    }
  }
  virtual vec3 random(const vec3& o, random_gen& rng) {
    vec3 random_point = vec3(x0 + rng.unif_rand() * (x1 - x0), k, z0 + rng.unif_rand() * (z1-z0));
    return(random_point - o);
  }
  float x0, x1, z0, z1, k;
  material *mp;
};

class yz_rect : public hitable {
public:
  yz_rect() {}
  yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) :
  y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng);
  virtual bool bounding_box(float t0, float t1, aabb& box) const {
    box = aabb(vec3(k-0.0001,y0,z0), vec3(k+0.0001,y1,z1));
    return(true);
  }
  virtual float pdf_value(const vec3& o, const vec3& v, random_gen& rng) {
    hit_record rec;
    if(this->hit(ray(o,v), 0.001, FLT_MAX, rec, rng)) {
      float area = (y1-y0)*(z1-z0);
      float distance_squared = rec.t * rec.t * v.squared_length();
      float cosine = fabs(dot(v,rec.normal)/v.length());
      return(distance_squared / (cosine * area));
    } else {
      return(0);
    }
  }
  virtual vec3 random(const vec3& o, random_gen& rng) {
    vec3 random_point = vec3(k, y0 + rng.unif_rand() * (y1 - y0), z0 + rng.unif_rand() * (z1-z0));
    return(random_point-o);
  }
  float y0, y1, z0, z1, k;
  material *mp;
};

bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng) {
  float t = (k-r.origin().z()) / r.direction().z();
  if(t < t_min || t > t_max) {
    return(false);
  }
  float x = r.origin().x() + t*r.direction().x();
  float y = r.origin().y() + t*r.direction().y();
  if(x < x0 || x > x1 || y < y0 || y > y1) {
    return(false);
  }
  rec.u = (x-x0)/(x1-x0);
  rec.v = (y-y0)/(y1-y0);
  rec.t = t;
  rec.mat_ptr = mp;
  rec.p = r.point_at_parameter(t);
  rec.normal = vec3(0,0,1);
  return(true);
}

bool xz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng) {
  float t = (k-r.origin().y()) / r.direction().y();
  if(t < t_min || t > t_max) {
    return(false);
  }
  float x = r.origin().x() + t*r.direction().x();
  float z = r.origin().z() + t*r.direction().z();
  if(x < x0 || x > x1 || z < z0 || z > z1) {
    return(false);
  }
  rec.u = (x-x0)/(x1-x0);
  rec.v = (z-z0)/(z1-z0);
  rec.t = t;
  rec.mat_ptr = mp;
  rec.p = r.point_at_parameter(t);
  rec.normal = vec3(0,1,0);
  return(true);
}

bool yz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec, random_gen& rng) {
  float t = (k-r.origin().x()) / r.direction().x();
  if(t < t_min || t > t_max) {
    return(false);
  }
  float z = r.origin().z() + t*r.direction().z();
  float y = r.origin().y() + t*r.direction().y();
  if(z < z0 || z > z1 || y < y0 || y > y1) {
    return(false);
  }
  rec.u = (y-y0)/(y1-y0);
  rec.v = (z-z0)/(z1-z0);
  rec.t = t;
  rec.mat_ptr = mp;
  rec.p = r.point_at_parameter(t);
  rec.normal = vec3(1,0,0);
  return(true);
}

#endif
