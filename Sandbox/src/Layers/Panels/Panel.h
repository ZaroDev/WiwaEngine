#pragma once

#include <string>

class Panel
{
public:
	Panel(const char* name);
	virtual ~Panel();

	void SetActive(bool enabled);
	void SwitchActive();
	bool IsActive() const;
	virtual void Draw() = 0;
	virtual void Update(){}
	const char* GetName() const { return name; }

public:
	bool active = false;
protected:
	const char* name;
};