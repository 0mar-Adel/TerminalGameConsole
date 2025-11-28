#ifndef SH_VECTORS
#define SH_VECTORS
namespace SH {
struct vec2 {
  float x;
  float y;
  vec2(float x, float y) {
    this->x = x;
    this->y = y;
  }
  vec2() {
    this->x = 0.0;
    this->y = 0.0;
  }
};
struct vec3 {
  float x;
  float y;
  float z;
  vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  vec3() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
  }
};
float dot(vec2, vec2);
float dot(vec3, vec3);
vec3 cross(vec2, vec2);
vec3 cross(vec3, vec3);
vec2 add(vec2, vec2);
vec3 add(vec3, vec3);
// subtract arg1 - arg2
vec2 subtract(vec2, vec2);
// subtract arg1 - arg2
vec3 subtract(vec3, vec3);
float distance(vec2, vec2);
float distance(vec3, vec3);

} // namespace SH
#endif
