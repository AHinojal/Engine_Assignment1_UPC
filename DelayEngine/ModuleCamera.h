#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float4x4.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Methods
	/*void setPOV();
	void setAspectRadio();
	void setPlaneDistance();*/
	float4x4 getProjectionMatrix() { return projectionMatrix; };
	float4x4 getViewMatrix() { return viewMatrix; };
	void GoUpAndDown();


private:
	Frustum frustum;
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
};
