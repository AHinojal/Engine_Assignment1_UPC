#pragma once
#include "Module.h"
#include "Mesh.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class ModuleModel : public Module
{
public:

	ModuleModel();
	~ModuleModel();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Load(const char* file_name);
	void LoadTextures(const aiScene* scene, aiMaterial** const mMaterials, unsigned int mNumMaterials);
	void LoadMeshes(aiMesh** const mMeshes, unsigned int mNumMeshes, unsigned int program);
	void Draw();

private:
	std::vector<Mesh> meshes;
	std::vector<unsigned int> textures;
};

