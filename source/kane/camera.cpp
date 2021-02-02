#include <kane/camera.h>

glm::vec2 kane::camera::location { 0, 0 };
glm::vec2 kane::camera::view_min { 0, 0 };
glm::vec2 kane::camera::view_max { 0, 0 };

float kane::camera::scale = 1.f;