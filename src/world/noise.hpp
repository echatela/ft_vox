
# include <cmath>
# include "world/chunk_manager.hpp"

float	smoothstep(float value);
float	lerp(float start, float end, float weight);
float	gradientScalar(int xCorner, int yCorner, float x, float y);

constexpr auto kNoiseResolution = kLoadRange * 16;  /*336;*/

namespace noise {

	float	fractalBrownMotion(float x, float y, int octaves = 4);
	float	perlin(float x, float y);

}