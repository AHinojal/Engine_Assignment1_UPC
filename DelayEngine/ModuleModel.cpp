#include "Globals.h"
#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "Mesh.h"
#include <string>
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
    /*for (Mesh m : meshes) {
        // Delete VBO, VAO & EBO
        glDeleteBuffers(1, &m.getVBO());
        glDeleteBuffers(1, &m.getVAO());
        glDeleteBuffers(1, &m.getEBO());
    }*/
    return true;
}

void ModuleModel::Load(const char* file_name)
{
    const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
    sizeMeshes = scene->mNumMeshes;

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
            // I think that when you loaded the fisrt time, also search in the previous folder
            // Only access here if the texture is in a next folder
            LOG("Looking the texture on the path described in the FBX...: %s", file.data)
            unsigned isLoadImage = App->texture->LoadTexture(file.data);
            if (isLoadImage) {
                LOG("Texture loaded.");
                textures.push_back(isLoadImage);
            }
            else {
                std::string sourcefileData = "..\\Game\\assets\\";
                sourcefileData.append(file.data);
                LOG("Texture not found. Trying in the same folder than the FBX...: %s", sourcefileData.c_str());
                isLoadImage = App->texture->LoadTexture(sourcefileData.c_str());
                if (isLoadImage) {
                    LOG("Texture loaded.");
                    textures.push_back(isLoadImage);
                }
                else {
                    // Only access here if the texture is in a next folder
                    std::string textureFileData = "..\\Game\\assets\\textures\\";
                    textureFileData.append(file.data);
                    LOG("Texture not found. Trying in own Texture/ folder...: %s", textureFileData.c_str());
                    isLoadImage = App->texture->LoadTexture(textureFileData.c_str());
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

int ModuleModel::getVertexMeshes()
{
    int acuVertex = 0;
    for (Mesh m : meshes) {
        acuVertex += m.getVertexs();
    }
    return acuVertex;
}

int ModuleModel::getIndexesMeshes()
{
    int acuIndexes = 0;
    for (Mesh m : meshes) {
        acuIndexes += m.getIndexs();
    }
    return acuIndexes;
}

int ModuleModel::getFacesMeshes()
{
    int acuFaces = 0;
    for (Mesh m : meshes) {
        acuFaces += m.getFaces();
    }
    return acuFaces;
}
