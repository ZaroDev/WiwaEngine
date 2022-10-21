#pragma once

#include <string>

#include <Wiwa/Events/Event.h>

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
	virtual void OnEvent(Wiwa::Event& e){}

public:
	bool active = false;
protected:
	const char* name;
};