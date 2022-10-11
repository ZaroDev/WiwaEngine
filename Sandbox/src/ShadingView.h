#pragma once

struct ShadingView
{
	const char* name;
	bool active = false;

	ShadingView() = default;

	ShadingView(const char* Name, bool Active)
		: name(Name), active(Active) {}
};