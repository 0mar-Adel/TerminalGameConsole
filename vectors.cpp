#include "vectors.hpp"
#include <math.h>
namespace SH {
float dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 cross(vec2 a, vec2 b) {
  vec3 c;
  c.z = a.x * b.y - a.y * b.x;
  return c;
}
vec3 cross(vec3 a, vec3 b) {
  vec3 c;
  c.x = a.y * b.z - a.z * b.y;
  c.y = -1 * (a.x * b.z - a.x * b.x);
  c.z = a.x * b.y - a.y * b.x;
  return c;
}
vec2 add(vec2 a, vec2 b) {
  vec2 c;
  c.x = a.x + b.x;
  c.y = a.y + b.y;
  return c;
}
vec3 add(vec3 a, vec3 b) {
  vec3 c;
  c.x = a.x + b.x;
  c.y = a.y + b.y;
  c.z = a.z + b.z;
  return c;
}
vec2 subtract(vec2 a, vec2 b) {
  vec2 c;
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  return c;
}
vec3 subtract(vec3 a, vec3 b) {
  vec3 c;
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
  return c;
}
float distance(vec2 a, vec2 b) {
  return std::sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}
float distance(vec3 a, vec3 b) {
  return std::sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) +
                   pow((a.z - b.z), 2));
}
} // namespace SH
