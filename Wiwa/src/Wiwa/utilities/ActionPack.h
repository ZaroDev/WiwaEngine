#pragma once
#include "Action.h"

template<class... _params>
class ActionPack {
private:
	std::vector<Action<_params...>> actions;
public:
	ActionPack() {

	}

	void addAction(Action<_params...> action) {
		actions.emplace_back(action);
	}

	void removeObject(void* obj) {
		size_t size = actions.size();

		for (size_t i = 0; i < size; i++) {
			if (actions[i].getObj() == obj) {
				actions.erase(actions.begin() + i);
				break;
			}
		}
	}

	void execute(_params... params) {
		size_t size = actions.size();

		for (size_t i = 0; i < size; i++) {
			actions[i](params...);
		}
	}

	void operator+=(Action<_params...> action) {
		addAction(action);
	}

	void operator-=(void* obj) {
		removeObject(obj);
	}

	void operator()(_params... params) {
		execute(params...);
	}
};