#pragma once

#include "engine/core/scene/render_node.h"
#include "engine/core/render/base/mesh/mesh.h"
#include "engine/core/render/base/material.h"
#include "engine/core/render/base/proxy/render_proxy.h"

namespace Echo
{
	class Trail : public Render
	{
		ECHO_CLASS(Trail, Render)

			struct VertexFormat
		{
			Vector3		m_position;
			Vector2		m_uv;

			VertexFormat(const Vector3& pos, const Vector2& uv)
				: m_position(pos), m_uv(uv)
			{}
		};
		typedef vector<VertexFormat>::type	VertexArray;
		typedef vector<Word>::type	IndiceArray;

	public:
		Trail();
		virtual ~Trail();

		// width
		i32 getWidth() const { return m_width; }
		void setWidth(i32 width);

		// width
		i32 getHeight() const { return m_height; }
		void setHeight(i32 height);

		// material
		Material* getMaterial() const { return m_material; }
		void setMaterial(Object* material);

	protected:
		// build drawable
		void buildRenderable();

		// update
		virtual void update_self() override;

		// update vertex buffer
		void updateMeshBuffer();

	private:
		bool                    m_isRenderableDirty = true;
		i32						m_width = 64;
		i32						m_height = 64;
		MeshPtr				    m_mesh;
		MaterialPtr				m_material;
		RenderProxy*			m_renderable = nullptr;
	};
}
