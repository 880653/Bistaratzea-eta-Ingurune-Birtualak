#include "constants.h"

const float Constants::pi = 3.14159265359f;
const float Constants::distance_epsilon = 1e-05f;
const float Constants::angle_epsilon = 0.00872665f;  // 0.5 degrees

const float Constants::degree_to_rad = 0.0174533f; // 2.0 * 3.1415927 / 360.0
const float Constants::rad_to_degree = 57.2958f;   // 360 / ( 2.0 * 3.1415927 )

const int Constants::gl_texunits::texture = 0;
const int Constants::gl_texunits::bump = 1;
const int Constants::gl_texunits::specular = 2;
const int Constants::gl_texunits::projective = 3;
const int Constants::gl_texunits::shadow = 4;
const int Constants::gl_texunits::envmap = 4;
