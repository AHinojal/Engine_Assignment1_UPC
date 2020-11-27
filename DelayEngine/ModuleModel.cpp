#include "Globals.h"
#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModuleModel::ModuleModel()
{
}

ModuleModel::~ModuleModel()
{
}

bool ModuleModel::Init()
{
    return true;
}

update_status ModuleModel::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleModel::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleModel::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleModel::CleanUp()
{
    return true;
}

void ModuleModel::Load(const char* file_name)
{
    const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);

    App->program->CreateProgram();

    if (scene)
    {
        LoadTextures(scene, scene->mMaterials, scene->mNumMaterials);
        LoadMeshes(scene->mMeshes, scene->mNumMeshes, App->program->GetProgram());
    }
    else
    {
       LOG("Error loading %s: %s", file_name, aiGetErrorString());
    }
}

void ModuleModel::LoadTextures(const aiScene* scene, aiMaterial** const mMaterials, unsigned int mNumMaterials)
{
    aiString file;
    textures.reserve(scene->mNumMaterials);
    for (unsigned i = 0; i < scene->mNumMaterials; ++i)
    {
        if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
        {
            unsigned isLoadImage = App->texture->LoadTexture(file.data);
            if (isLoadImage) {
                LOG("Texture loaded.");
                textures.push_back(isLoadImage);
            }
            else {
                LOG("Texture not found.");
            }
            
        }
    }
}

void ModuleModel::LoadMeshes(aiMesh** const mMeshes, unsigned int mNumMeshes, unsigned int program)
{
    meshes.reserve(mNumMeshes);

    for (unsigned int i = 0; i < mNumMeshes; i++) {
        meshes.push_back(Mesh(program));
        meshes[i].LoadVBO(mMeshes[i]);
        meshes[i].LoadEBO(mMeshes[i]);
        meshes[i].CreateVAO();
    }
}

void ModuleModel::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(textures);
    }
}
