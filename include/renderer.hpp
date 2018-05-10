#pragma once
#include <glad.h>
#include <window.hpp>
#include <string>
#include <vector>
#include <stdio.h>

typedef std::vector<Texture> Textures;

class OpenglMesh {
	private:
		unsigned int vbo = 0;
		unsigned int vboPositions = 0;
		unsigned int vboNormals = 0;
		unsigned int vboTexcoords = 0;
		unsigned int vao = 0;
		unsigned int ebo = 0;
		unsigned int vertices = 0;
		unsigned int indices = 0;

	public:
		OpenglMesh(const Mesh& mesh) {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);
//			glBindBuffer(GL_ARRAY_BUFFER, vbo);
//			glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &vboPositions);
			glGenBuffers(1, &vboNormals);
			glGenBuffers(1, &vboTexcoords);

			glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
			glBufferData(GL_ARRAY_BUFFER, mesh.positions.size() * sizeof(Vector3), &mesh.positions[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*) 0);


			glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
			glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(Vector3), &mesh.normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*) 0);


			glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, mesh.texcoords.size() * sizeof(Vector2), &mesh.texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*) 0);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

//			glEnableVertexAttribArray(0);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
//
//			glEnableVertexAttribArray(1);
//			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normals));
//
//			glEnableVertexAttribArray(2);
//			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tex));




			glBindVertexArray(0);

			indices = mesh.indices.size();

			printf("opengl: mesh loaded\n");
			printf("opengl: vbo:%d\n", vbo);
			printf("opengl: vao:%d\n", vao);
			printf("opengl: ebo:%d\n", ebo);
//			printf("opengl: vertices:%d\n", vertices);
			printf("opengl: indices:%d\n", indices);
		}

		unsigned int getVBO() const {
			return vbo;
		}

		unsigned int getVAO() const {
			return vao;
		}

		unsigned int getEBO() const {
			return ebo;
		}

		unsigned int getVertices() const {
			return vertices;
		}

		unsigned int getIndices() const {
			return indices;
		}

};

class OpenglModel {
	private:
		std::vector<OpenglMesh> meshes;
		public:
		OpenglModel(const Model& m) {
			for (Mesh mesh : m.meshes) {
				meshes.push_back(OpenglMesh(mesh));
			}
		}

		const std::vector<OpenglMesh>& getMeshes() const {
			return meshes;
		}
};

class Renderer {
	private:
		Window* window = nullptr;
		bool msaa = true;
		bool ambient = true;
		bool specular = true;
		bool diffuse = true;
		std::unique_ptr<Shader> defaultShader = std::unique_ptr<Shader>();
		std::unique_ptr<Shader> lineShader = std::unique_ptr<Shader>();

	public:
		Renderer(Window* window, const char* vertexPath, const char* fragmentPath) {
			this->window = window;
			lineShader = Shader::loadFromFile("shaders/vline.glsl", "shaders/fline.glsl");
			defaultShader = Shader::loadFromFile(vertexPath, fragmentPath);

			glEnable (GL_DEPTH_TEST);
			glEnable (GL_MULTISAMPLE);
//			glLineWidth(3);

			unsigned int vao = 0;
			glGenVertexArrays(1, &vao);
		}
		;

		void clearColor(float x, float y, float z) {
			glClearColor(x, y, z, 1.0f);
		}

		void clearColorBuffer() {
			glClear (GL_COLOR_BUFFER_BIT);
		}

		void clearDepthBuffer() {
			glClear (GL_DEPTH_BUFFER_BIT);
		}

		bool isMsaa() const {
			return msaa;
		}

		bool isAmbientLight() const {
			return ambient;
		}

		bool isSpecularLight() const {
			return specular;
		}

		bool isDiffuseLight() const {
			return diffuse;
		}

		void toggleAmbientLight() {
			ambient = !ambient;
			defaultShader->set("ambientLight", ambient);
		}

		void toggleSpecularLight() {
			specular = !specular;
			defaultShader->set("specularLight", specular);
		}

		void toggleDiffuseLight() {
			diffuse = !diffuse;
			defaultShader->set("diffuseLight", diffuse);
		}

		void toggleMsaa() {
			msaa = !msaa;
			if (msaa) {
				glEnable (GL_MULTISAMPLE);
			} else {
				glDisable (GL_MULTISAMPLE);
			}
		}

		void drawLine(Vector3 v1, Vector3 v2, const Camera& camera, Vector4 color) {
			lineShader->use();
			lineShader->set("color", color);
			lineShader->set("view", camera.view());
			lineShader->set("projection", camera.perspective(window->aspectRatio()));

			GLfloat buffer[] = { v1.x, v1.y, v1.z, v2.x, v2.y, v2.z };

			unsigned int vao = 0;
			unsigned int vbo = 0;

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_LINES, 0, 2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
		}

		void draw(OpenglMesh m, const Camera& camera, const Matrix4x4& model, Textures textures) {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			defaultShader->use();
			defaultShader->set("ambientColor", Vector3 { 1.0f, 1.0f, 1.0f });
			defaultShader->set("lightPosition", Vector3 { 0.0f, 0.0f, 0.0f });
			defaultShader->set("cameraPosition", Vector3 { 0.0f, 0.0f, 0.0f });

			defaultShader->set("colorOverride", Vector4 { 1.0f, 1.0f, 1.0f, 0.0f });
			defaultShader->set("ambientLight", ambient);
			defaultShader->set("specularLight", specular);
			defaultShader->set("diffuseLight", diffuse);

			if (textures.empty()) {
				defaultShader->set("textures", false);
			} else {
				defaultShader->set("textures", true);
				for (unsigned int i = 0; i < textures.size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);
					string number;
					string name = textures[i].type;
					if (name == "texture_diffuse") {
						number = std::to_string(diffuseNr++);
					} else if (name == "texture_specular") {
						number = std::to_string(specularNr++);
					} else if (name == "texture_normal") {
						number = std::to_string(normalNr++);
					} else if (name == "texture_height") {
						number = std::to_string(heightNr++);
					}

					glUniform1i(glGetUniformLocation(defaultShader->getId(), (name + number).c_str()), i);
					glBindTexture(GL_TEXTURE_2D, textures[i].id);
				}
			}

			defaultShader->set("projection", camera.perspective(window->aspectRatio()));
			defaultShader->set("view", camera.view());
			defaultShader->set("model", model);

			glBindVertexArray(m.getVAO());
			glDrawElements(GL_TRIANGLES, m.getIndices(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture (GL_TEXTURE0);
		}

		void draw(const OpenglMesh& m, const Camera& camera, const Matrix4x4& model) {
			defaultShader->use();
			defaultShader->set("ambientColor", Vector3 { 1.0f, 1.0f, 1.0f });
			defaultShader->set("lightPosition", Vector3 { 0.0f, 0.0f, 0.0f });
			defaultShader->set("cameraPosition", Vector3 { 0.0f, 0.0f, 0.0f });

			defaultShader->set("colorOverride", Vector4 { 1.0f, 1.0f, 1.0f, 0.0f });
			defaultShader->set("ambientLight", ambient);
			defaultShader->set("specularLight", specular);
			defaultShader->set("diffuseLight", diffuse);

			defaultShader->set("textures", false);

			defaultShader->set("projection", camera.perspective(window->aspectRatio()));
			defaultShader->set("view", camera.view());
			defaultShader->set("model", model);
			glBindVertexArray(m.getVAO());
			glDrawElements(GL_TRIANGLES, m.getIndices(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		void draw(const OpenglModel& m, const Camera& camera, const Matrix4x4& model) {
			for (unsigned int i = 0; i < m.getMeshes().size(); i++) {
				draw(m.getMeshes()[i], camera, model);
			}
		}

};
