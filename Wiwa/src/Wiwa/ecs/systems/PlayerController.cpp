#include "..\..\..\headers\ecs\systems\PlayerController.h"
#include "../../../headers/Input.h"
#include "../../../headers/Video.h"

EM::PlayerController::PlayerController()
{
}

EM::PlayerController::~PlayerController()
{
}

void EM::PlayerController::OnUpdate()
{
	std::vector<Transform2D*>* t2dv = GetComponents<Transform2D>();
	std::vector<Player*>* pv = GetComponents<Player>();

	size_t size = t2dv->size();

	Transform2D** t2d = t2dv->data();
	Player** p = pv->data();

	Input* inp = Application::GetInstance()->input;
	Video* video = Application::GetInstance()->video;

	for (size_t i = 0; i < size; i++) {
		if (inp->getKey(SDL_SCANCODE_W)) {
			t2d[i]->position.y -= p[i]->moveSpeed;
		}

		if (inp->getKey(SDL_SCANCODE_S)) {
			t2d[i]->position.y += p[i]->moveSpeed;
		}

		if (inp->getKey(SDL_SCANCODE_A)) {
			t2d[i]->position.x -= p[i]->moveSpeed;
		}

		if (inp->getKey(SDL_SCANCODE_D)) {
			t2d[i]->position.x += p[i]->moveSpeed;
		}

		if (t2d[i]->position.x < 0) {
			t2d[i]->position.x = 0;
		}
		else if (t2d[i]->position.x > video->getScreenWidth()) {
			t2d[i]->position.x = video->getScreenWidth();
		}
	}
}

void EM::PlayerController::OnEntityAdded(EntityId entityID)
{
}
