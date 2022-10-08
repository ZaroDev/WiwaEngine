#pragma once

#include <vector>

#include <Wiwa/utilities/math/Math.h>

class Mesh
{
private:
	

	std::vector<float> vbo_data;
	std::vector<int> ebo_data;

	unsigned int vao, vbo, ebo;

	void getMeshFromFile(const char* file);
public:
	Mesh();
	~Mesh();

	void Init(const char* file);
	void Render();
};