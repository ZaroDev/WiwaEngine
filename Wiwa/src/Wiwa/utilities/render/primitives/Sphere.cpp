#include <wipch.h>

#include "Sphere.h"

namespace Wiwa {
	Sphere::Sphere() : Mesh(NULL)
	{
		int stack_count = 18;
		int sector_count = 36;
        int radius = 1;

		float x, y, z, xy;
		float sectorStep = 2 * PI_F / sector_count;
		float stackStep = PI_F / stack_count;
		float sectorAngle, stackAngle;

        // Generate vertices
        for (int i = 0; i <= stack_count; ++i)
        {
            stackAngle = (float)PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sector_count; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                vbo_data.push_back(x);
                vbo_data.push_back(y);
                vbo_data.push_back(z);
            }
        }

        // Generate indices
        int k1, k2;
        for (int i = 0; i < stack_count; ++i)
        {
            k1 = i * (sector_count + 1);     // beginning of current stack
            k2 = k1 + sector_count + 1;      // beginning of next stack

            for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    ebo_data.push_back(k1);
                    ebo_data.push_back(k2);
                    ebo_data.push_back(k1 + 1);
                }

                if (i != (stack_count - 1))
                {
                    ebo_data.push_back(k1 + 1);
                    ebo_data.push_back(k2);
                    ebo_data.push_back(k2 + 1);
                }
            }
        }

		generateBuffers();
	}

	Sphere::~Sphere()
	{
	}
}