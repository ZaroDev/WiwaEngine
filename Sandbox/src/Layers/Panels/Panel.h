#pragma once

#include <string>
#include <Wiwa/Events/Event.h>
#include "../../Events/PanelEvents.h"

class EditorLayer;

class Panel
{
public:
	Panel(const char* name, EditorLayer* instance);
	virtual ~Panel();

	void SetActive(bool enabled);
	void SwitchActive();
	bool IsActive() const;
	virtual void Draw() = 0;
	virtual void Update(){}
	const char* GetName() const { return name; }
	virtual void OnEvent(Wiwa::Event&e){}
public:
	bool active = false;
protected:
	const char* name;
	EditorLayer* instance;
};