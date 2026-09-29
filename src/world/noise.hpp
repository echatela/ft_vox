
# include <cmath>

float	smoothstep(float value);
float	lerp(float start, float end, float weight);
float	gradientScalar(int xCorner, int yCorner, float x, float y);

constexpr auto kNoiseResolution = 1024;

namespace noise {

	float	fractalBrownMotion(float x, float y, int octaves = 8);
	float	perlin(float x, float y);

}