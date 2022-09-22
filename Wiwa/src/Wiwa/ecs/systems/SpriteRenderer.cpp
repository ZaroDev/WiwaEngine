#include "../../../headers/ecs/systems/SpriteRenderer.h"
#include "../../../headers/Debug.h"
#include "../../../headers/Renderer2D.h"
#include "../../../headers/Video.h"

EM::SpriteRenderer::SpriteRenderer()
{
}

EM::SpriteRenderer::~SpriteRenderer()
{
	
}

void EM::SpriteRenderer::OnUpdate()
{
	std::vector<Transform2D*>* t2dv = GetComponents<Transform2D>();
	std::vector<Sprite*>* sprv = GetComponents<Sprite>();

	size_t size = t2dv->size();
	Transform2D** t2d = t2dv->data();
	Sprite** spr = sprv->data();

	Renderer2D* r2d = Application::GetInstance()->renderer2D;
	Video* v = Application::GetInstance()->video;

	for (size_t i = 0; i < size; i++) {
		t2d[i]->position.x = (t2d[i]->position.x + 1) % v->getScreenWidth();
		t2d[i]->position.y = (t2d[i]->position.y + 1) % v->getScreenHeight();

		r2d->UpdateInstancedQuadTex(i, t2d[i]->position, spr[i]->pivot);
	}
}

void EM::SpriteRenderer::OnEntityAdded(EntityId entityID)
{
	Application* App =  Application::GetInstance();
	Transform2D* temp2D = App->entityManager->GetComponent<Transform2D>(entityID);
	Sprite* tempSpriteScreen = App->entityManager->GetComponent<Sprite>(entityID);

	Color4f color = { 0.10588235294117647058823529411765f, 0.77647058823529411764705882352941f, 0.87843137254901960784313725490196f, 1.0f };

	App->renderer2D->CreateInstancedQuadTex(tempSpriteScreen->spriteResource, temp2D->position, tempSpriteScreen->sizeScreen, color, tempSpriteScreen->rect, tempSpriteScreen->pivot);
}