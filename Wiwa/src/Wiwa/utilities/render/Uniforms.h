#pragma once
#include <cstdint>

struct DefaultUnlitUniforms
{
	uint32_t Projection;
	uint32_t View;
	uint32_t Model;
};
struct DirLightUniforms
{
	uint32_t Direction;
	uint32_t Ambient;
	uint32_t Diffuse;
	uint32_t Specular;
};
struct PointLightUniforms
{
	uint32_t LightsNum;
	uint32_t Position;
	uint32_t Ambient;
	uint32_t Diffuse;
	uint32_t Specular;
	uint32_t Constant;
	uint32_t Linear;
	uint32_t Quadratic;
};
struct DefaultLitUniforms
{
	uint32_t Diffuse;
	uint32_t Specular;

	uint32_t Shininess;

	uint32_t ViewPos;

	uint32_t Projection;
	uint32_t View;
	uint32_t Model;
};