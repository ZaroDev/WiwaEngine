#include <wipch.h>

#include "SpriteRenderer.h"
#include <Wiwa/utilities/Log.h>

#include <Wiwa/ecs/EntityManager.h>

//namespace Wiwa {
//	SpriteRenderer::SpriteRenderer()
//	{
//	}
//
//	SpriteRenderer::~SpriteRenderer()
//	{
//
//	}
//
//	void SpriteRenderer::OnUpdate()
//	{
//		/*std::vector<Transform2D*>* t2dv = GetComponents<Transform2D>();
//		std::vector<Sprite*>* sprv = GetComponents<Sprite>();
//
//		size_t size = t2dv->size();
//		Transform2D** t2d = t2dv->data();
//		Sprite** spr = sprv->data();
//
//		Renderer2D& r2d = Application::Get().GetRenderer2D();
//		Size2i& res = Application::Get().GetTargetResolution();
//
//		for (size_t i = 0; i < size; i++) {
//			r2d.UpdateInstancedQuadTex(i, t2d[i]->position, spr[i]->pivot);
//		}*/
//	}
//
//	void SpriteRenderer::OnEntityAdded(EntityId entityID)
//	{
//		Application& App = Application::Get();
//		Transform2D* temp2D = App.GetEntityManager().GetComponent<Transform2D>(entityID);
//		Sprite* tempSpriteScreen = App.GetEntityManager().GetComponent<Sprite>(entityID);
//
//		Color4f color = { 0.10588235294117647058823529411765f, 0.77647058823529411764705882352941f, 0.87843137254901960784313725490196f, 1.0f };
//
//		App.GetRenderer2D().CreateInstancedQuadTex(tempSpriteScreen->spriteResource, temp2D->position, tempSpriteScreen->sizeScreen, color, tempSpriteScreen->rect, tempSpriteScreen->pivot);
//	}
//}