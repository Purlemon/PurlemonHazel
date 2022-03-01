#include<PurlemonHazel.h>

#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

#define PH PurlemonHazel

class ExampleLayer : public PurlemonHazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), camera_(-1.6f, 1.6f, -0.9f, 0.9f), camera_pos_(0.0f), camera_rotation_(0.0f), square_pos_(0.0f)
	{

		// ----------------------------
		// OpenGL Render
		// ----------------------------

		// 三角形
		vertex_array_.reset(PH::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		PH::BufferLayout layout = {
			{PH::ShaderDataType::Float3, "a_Position"},
			{PH::ShaderDataType::Float4, "a_Color"},
		};
		std::shared_ptr<PH::VertexBuffer>vertex_buffer_;
		vertex_buffer_.reset(PH::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertex_buffer_->SetLayout(layout);
		vertex_array_->AddVertexBuffer(vertex_buffer_);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<PH::IndexBuffer>index_buffer_;
		index_buffer_.reset(PH::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		vertex_array_->SetIndexBuffer(index_buffer_);

		std::string vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ProjectionView;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ProjectionView * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		shader_.reset(PH::Shader::Create(vertex_src, fragment_src));

		// 正方形
		square_va_.reset(PH::VertexArray::Create());

		float square_vertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<PH::VertexBuffer>square_vb;
		square_vb.reset(PH::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		square_vb->SetLayout({
			{ PH::ShaderDataType::Float3, "a_Position" }
			});
		square_va_->AddVertexBuffer(square_vb);

		unsigned int square_indices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<PH::IndexBuffer>square_ib;
		square_ib.reset(PH::IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(unsigned int)));
		square_va_->SetIndexBuffer(square_ib);

		std::string blue_vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ProjectionView;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ProjectionView * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string blue_fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = u_Color;
			}
		)";
		color_shader_.reset(PH::Shader::Create(blue_vertex_src, blue_fragment_src));
	}

	void OnUpdate(PH::Timestep ts) override
	{
		// Camera
		if (PH::Input::IsKeyPressed(PH_KEY_LEFT))
			camera_pos_.x -= camera_move_speed_ * ts;
		else if (PH::Input::IsKeyPressed(PH_KEY_RIGHT))
			camera_pos_.x += camera_move_speed_ * ts;

		if (PH::Input::IsKeyPressed(PH_KEY_UP))
			camera_pos_.y += camera_move_speed_ * ts;
		else if (PH::Input::IsKeyPressed(PH_KEY_DOWN))
			camera_pos_.y -= camera_move_speed_ * ts;
		
		if (PH::Input::IsKeyPressed(PH_KEY_A))
			camera_rotation_ += camera_rota_speed_ * ts;
		else if (PH::Input::IsKeyPressed(PH_KEY_D))
			camera_rotation_ -= camera_rota_speed_ * ts;

		// Square
		if (PH::Input::IsKeyPressed(PH_KEY_J))
			square_pos_.x -= square_move_speed_ * ts;
		else if (PH::Input::IsKeyPressed(PH_KEY_L))
			square_pos_.x += square_move_speed_ * ts;

		if (PH::Input::IsKeyPressed(PH_KEY_I))
			square_pos_.y += square_move_speed_ * ts;
		else if (PH::Input::IsKeyPressed(PH_KEY_K))
			square_pos_.y -= square_move_speed_ * ts;

		PurlemonHazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		PurlemonHazel::RenderCommand::Clear();

		camera_.SetPosition(camera_pos_);
		camera_.SetRotation(camera_rotation_);

		PurlemonHazel::Renderer::BeginScene(camera_);
		{
			glm::vec4 blue = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
			glm::vec4 red = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			
			std::dynamic_pointer_cast<PH::OpenGLShader>(color_shader_)->Bind();
			std::dynamic_pointer_cast<PH::OpenGLShader>(color_shader_)->UploadUniforFloat4("u_Color", square_color_);

			for (int y = 0; y < 20; ++y) {
				for (int x = 0; x < 20; ++x) {
					glm::vec3 pos(x * 0.18f, y * 0.18f, 0.0f);
					glm::mat4 transform = glm::translate(glm::translate(glm::mat4(1.0f),square_pos_), pos) * scale;
					PurlemonHazel::Renderer::Submit(color_shader_, square_va_, transform);
				}
			}

			PurlemonHazel::Renderer::Submit(shader_, vertex_array_);
		}
		PurlemonHazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(square_color_));
		ImGui::End();
	}

	void OnEvent(PurlemonHazel::Event& event) override
	{

	}

private:
	// Render 
	std::shared_ptr<PurlemonHazel::Shader>shader_;
	std::shared_ptr<PurlemonHazel::VertexArray>vertex_array_;

	std::shared_ptr<PurlemonHazel::Shader>color_shader_;
	std::shared_ptr<PurlemonHazel::VertexArray>square_va_;

	// Camera
	PurlemonHazel::OrthographicCamera camera_;
	float camera_rotation_;
	glm::vec3 camera_pos_;

	float camera_rota_speed_ = 60.0f;
	float camera_move_speed_ = 5.0f;

	// Square
	glm::vec3 square_pos_;
	float square_move_speed_ = 10.0f;
	glm::vec4 square_color_ = { 0.2f, 0.3f, 0.8f, 1.0f };
};

class Sandbox:public PurlemonHazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

// 实现入口方法
PurlemonHazel::Application* PurlemonHazel::CreateApplication()
{
	return new Sandbox();
}