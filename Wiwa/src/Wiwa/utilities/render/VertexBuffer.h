#pragma once

#include <Wiwa/utilities/math/Math.h>

namespace Wiwa {
	struct Vertex {
		Vector3f position;
	};

	struct TextureClip {
		Vector2f bottomRight;
		Vector2f topRight;
		Vector2f topLeft;
		Vector2f bottomLeft;
	};

	struct VertexTexture {
		Vector3f position;
		Vector2f texCoord;
	};

	struct VertexInstance {
		Vector2f position;
		Vector2f scale;
		Color4f color;
	};

	struct VertexInstanceTexture {
		Vector2f position;
		Vector2f scale;
		Color4f color;

		// Texture data
		float textureId;
		TextureClip textureClip;
	};

	struct Quad {
		Vertex vertices[4];
	};
}