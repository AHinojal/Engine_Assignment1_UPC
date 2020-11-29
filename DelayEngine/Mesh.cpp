#include "Mesh.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include <assimp/mesh.h>
#include <GL/glew.h>

Mesh::Mesh()
{
	
}

Mesh::Mesh(unsigned programId)
{
	program = programId;
}

Mesh::~Mesh()
{
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * nVertexs));
	glBindVertexArray(0); // Dont forget it!
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	nVertexs = mesh->mNumVertices;
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;

	nFaces = mesh->mNumFaces;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, index_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	nIndexes = mesh->mNumFaces * 3;
}

void Mesh::Draw(const std::vector<unsigned>& model_textures)
{
	unsigned program = App->program->GetProgram();
	const float4x4& view = App->camera->getViewMatrix();
	const float4x4& proj = App->camera->getProjectionMatrix();
	float4x4 model = App->camera->getModelMatrix();
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[textureIndex]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, nIndexes, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::CleanUp()
{

}

void Mesh::DrawLenna() {
	// TODO: retrieve model view and projection -> Get it from the camera
	glUseProgram(App->program->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "model"), 1, GL_TRUE, &App->camera->getModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "view"), 1, GL_TRUE, &App->camera->getViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->GetProgram(), "proj"), 1, GL_TRUE, &App->camera->getProjectionMatrix()[0][0]);
	// TODO: bind buffer and vertex attributes
	// WORKING WITH VBO
	float vtx_data[] = {
		// first triangle
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		1.0f, -1.0f, 0.0f, // ← v1 pos
		-1.0f, 1.0f, 0.0f, // ← v2 pos
		1.0f, 1.0f, 0.0f, // ← v3 pos

		0.0f, 0.0f, // ← v0 texcoord
		1.0f, .0f, // ← v1 texcoord
		0.0f, 1.0f, // ← v2 texcoord
		1.0f, 1.0f, // ← v2 texcoord


		/*Testing: Triangle
		-1.0f, -1.0f, 0.0f, // ← v0 pos
		1.0f, -1.0f, 0.0f, // ← v1 pos
		0.0f, 1.0f, 0.0f, // ← v2 pos
		0.0f, 0.0f, // ← v0 texcoord
		1.0f, 0.0f, // ← v1 texcoord
		0.5f, -1.0f, // ← v2 texcoord*/
	};

	// CREATE VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// CREATE VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	unsigned int indexes[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		2, 1, 3 // second triangle
	};

	// CREATE EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 4 * 3)); // last parameter buffer offset
	glEnableVertexAttribArray(1);

	// Enable texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->LoadTexture("..\\Game\\test_lenna.png"));
	glUniform1i(glGetUniformLocation(App->program->GetProgram(), "mytexture"), 0);

	// 1 triangle to draw = 3 vertices
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//// 2 triangle to draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}