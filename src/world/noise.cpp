
# include <cmath>
# include "noise.hpp"

/*
*	The smoothstep function is a classic in videogames, often used in animation.
* 	It is used to both clamp (keep a value in between two thresholds)
*	and to lerp values (interpolate, create a smoother curve of distribution)
*	based on a ratio (hence the step).
*
*	After we selected a point in the grid, we use it to choose which percentage of each value
*	we apply.
*
*	Here I took the version from wikipedia, but we could change the lerp if we want to change
*	the value distribution.
*/
float	smoothstep(float value)
{
	if (value <= 0.0) return (0.0f);
	if (value >= 1.0)  return (1.0f);

	return (value * value * (3.0 - 2.0 * value));
}

/*
*	weight has to be in-between [0, 1]
*/
float	lerp(float start, float end, float weight)
{
	return (start + (end - start) * smoothstep(weight));
}

/*
*	Replacement of the lerp function for improved perlin
*/
float	fade(float value)
{
	return ((6*value - 15)*value + 10)*value*value*value;
}

#include "glm/vec2.hpp"

constexpr auto kGradientVectorsEntries = 3;
constexpr int kGradientVectors[kGradientVectorsEntries] = {-1, 0, 1};

typedef float t_gradientsGrid[kNoiseResolution][kNoiseResolution][2];

/*
*	This is where we specify each values from the grid, which we'll then interpolate
*	to compute or coord noise value.
*	Therefore, modifying the distribution allows to modify the final result.
*/
void generateGradients(t_gradientsGrid &gradients)
{
	for (unsigned int y = 0; y < kNoiseResolution; y++)
	{
		for (unsigned int x = 0; x < kNoiseResolution; x++)
		{

			for (unsigned int i = 0; i < 2; i++)
			{
				int r = rand() % kGradientVectorsEntries;
				gradients[y][x][i] = kGradientVectors[r];
			}
		}
	}
}

/*	
*	Scalar product of two vectors :
*	1) one of the case's corner gradient vectors
*	2) the distance of the coord from the corner
*/
float gradientScalar(int xCorner, int yCorner, float x, float y)
{
	// Currently I % the xCorner and the yCorner but we might need to find another solution
	// for contiguous noises

	static t_gradientsGrid gradients;
	static bool	 generated = false;

	if (!generated)
	{
		generateGradients(gradients);
		generated = true;
	}

	// This is the distance vector
	float xCeiled = x - xCorner;
	float yCeiled = y - yCorner;

	return (xCeiled * gradients[yCorner % kNoiseResolution][xCorner % kNoiseResolution][0] + 
		yCeiled * gradients[yCorner % kNoiseResolution][xCorner % kNoiseResolution][1]);

}

namespace noise {

	float	fractalBrownMotion(float x, float y, int octaves)
	{
		float result = 0.0;
		float amplitude = 1.0;
		float frequency = 0.005;

		// result = perlin(x, y);

		for (int i = 0; i < octaves; i++)
		{
			result += amplitude * perlin(x * frequency, y * frequency);
			
			amplitude *= 0.5;
			frequency *= 2.0;
		}

		return (result);
	}

	float	perlin(float x, float y)
	{
		// We compute the coordinates from the grid square in which we are
		int x0 = std::floor(x);
		int x1 = x0 + 1;
		int y0 = std::floor(y);
		int y1 = y0 + 1;

		// We compute the ratio representing the distance from our point to the edges...
		// (We could use other curves to modify the result)
		float rx = x - x0;
		float ry = y - y0;

		rx = fade(rx);
		ry = fade(ry);

		// ...and use it to interpolate the values from the edges
		float corner1, corner2, value1, value2;

		corner1 = gradientScalar(x0, y0, x, y); // upper - left
		corner2 = gradientScalar(x1, y0, x, y); // upper - right
		value1 = lerp(corner1, corner2, rx);

		corner1 = gradientScalar(x0, y1, x, y);	// lower - left
		corner2 = gradientScalar(x1, y1, x, y);	// lower - right
		value2 = lerp(corner1, corner2, rx);

		float result = lerp(value1, value2, ry);

		return (result);
	}

}