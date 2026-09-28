
# include <cmath>

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
	if (value >= 1.0)  return (0.0f);

	return (value * value * 3.0 - 2.0 * value);
}

/*
*	weight has to be in-between [0, 1]
*/
float	lerp(float start, float end, float weight)
{
	return (start + (end - start) * smoothstep(weight));
}

constexpr auto kNoiseResolution = 512;

// constexpr auto kGradientVectors = {{}}

typedef float t_gradientsGrid[kNoiseResolution][kNoiseResolution][2];

/*
*	This is where we specify each values from the grid, which we'll then interpolate
*	to compute or coord noise value.
*	Therefore, modifying the distribution allows to modify the final result.
*/
// void generateGradients(t_gradientsGrid &gradients)
// {
// 	for (unsigned int y = 0; y < kNoiseResolution; y++)
// 	{
// 		for (unsigned int x = 0; x < kNoiseResolution; x++)
// 		{

// 			for (unsigned int i = 0; i < 2; i++)
// 			gradients[y][x][i] = 
// 		}
// 	}
// }

/*	
*	Scalar product of two vectors :
*	1) one of the case's corner gradient vectors
*	2) the distance of the coord from the corner
*/
float gradientScalar(int xCorner, int yCorner, float x, float y)
{
	// There will probably be the need to % the xInt/yInt to ensure it's :
	// 1 - under kNoiseResolution
	// 2 - positive ?

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

	return (xCeiled * gradients[yCorner][xCorner][0] + yCeiled * gradients[yCorner][xCorner][1]);

}

namespace noise {

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

		// ...and use it to interpolate the values from the edges
		float corner1, corner2, value1, value2;

		corner1 = gradientScalar(x0, y0, x, y); // upper - left
		corner2 = gradientScalar(x1, y0, x, y); // upper - right
		value1 = lerp(corner1, corner2, rx);

		corner1 = gradientScalar(x0, y1, x, y);	// lower - left
		corner2 = gradientScalar(x1, y1, x, y);	// lower - right
		value2 = lerp(corner1, corner2, rx);

		return (lerp(value1, value2, ry));
	}

}