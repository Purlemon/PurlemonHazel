#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace PH {

	struct Renderer2DStorage
	{
		Ref<VertexArray> quad_vertex_array;
		Ref<Shader> flat_color_shader;
<<<<<<< HEAD
=======
		Ref<Shader> texture_shader;
>>>>>>> parent of 81a3470 (将两个shader合并)
	};

	static Renderer2DStorage* sData;

	void Renderer2D::Init()
	{
		sData = new Renderer2DStorage();
		sData->quad_vertex_array= VertexArray::Create();

		float square_vertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		Ref<PH::VertexBuffer>square_vb;
		square_vb.reset(VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		square_vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			});
		sData->quad_vertex_array->AddVertexBuffer(square_vb);

		unsigned int square_indices[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer>square_ib;
		square_ib.reset(IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(unsigned int)));
		sData->quad_vertex_array->SetIndexBuffer(square_ib);

		sData->flat_color_shader = Shader::Create("flat_color", "assets/shaders/vertex/flat_color.vert", "assets/shaders/fragment/flat_color.frag");
<<<<<<< HEAD
=======
	
		sData->texture_shader = Shader::Create("texture", "assets/shaders/vertex/texture.vert", "assets/shaders/fragment/texture.frag");
		sData->texture_shader->Bind();
		sData->texture_shader->SetInt("u_Texture", 0);
>>>>>>> parent of 81a3470 (将两个shader合并)
	}

	void Renderer2D::Shutdown()
	{
		delete sData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
<<<<<<< HEAD
		std::dynamic_pointer_cast<OpenGLShader>(sData->flat_color_shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(sData->flat_color_shader)->UploadUniformMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
=======
		sData->flat_color_shader->Bind();
		sData->flat_color_shader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
		
		sData->texture_shader->Bind();
		sData->texture_shader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
>>>>>>> parent of 81a3470 (将两个shader合并)
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
<<<<<<< HEAD
		std::dynamic_pointer_cast<OpenGLShader>(sData->flat_color_shader)->Bind();
		glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f)), position);
		std::dynamic_pointer_cast<OpenGLShader>(sData->flat_color_shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(sData->flat_color_shader)->UploadUniformFloat4("u_Color", color);
=======
		sData->flat_color_shader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		sData->flat_color_shader->SetMat4("u_Transform", transform);
		sData->flat_color_shader->SetFloat4("u_Color", color);
>>>>>>> parent of 81a3470 (将两个shader合并)

		sData->quad_vertex_array->Bind();
		RenderCommand::DrawIndexed(sData->quad_vertex_array);
	}

<<<<<<< HEAD
=======
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const const Ref<Texture2D>& texture)
	{
		sData->texture_shader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		sData->texture_shader->SetMat4("u_Transform", transform);

		texture->Bind(0);

		sData->quad_vertex_array->Bind();
		RenderCommand::DrawIndexed(sData->quad_vertex_array);
	}
>>>>>>> parent of 81a3470 (将两个shader合并)
}