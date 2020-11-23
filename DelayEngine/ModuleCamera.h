#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3.h"

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
	float4x4 getProjectionMatrix() { return projectionMatrix; };
	float4x4 getViewMatrix() { return viewMatrix; };
	float getFOV() { return horizontalFOV; };
	float getAspectRadio() { return aspectRadio; };
	// Setters Window View
	void setFOV(float range) {
		horizontalFOV = range;
	}
	void setAspectRadio() {

	}
	//void setPlaneDistance();
	// Transformations Camera
	// Holding SHIFT duplicates movement speed
	void increaseCameraSpeed();
	void goUpAndDown();
	void zoomForwardAndBackward();
	void moveLeftAndRight();
	void rotatePitch();
	void rotateYaw();
	void rotateRoll();
	void rotatePitchAndYawWithMouse();
	void doRotation(float3x3& rotationMatrix);

private:
	Frustum frustum;
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float horizontalFOV;
	float aspectRadio;
	float3 position;
	float standardSpeed;
	float actualSpeed;
	float deltaTime;
	float oldTime;

	void printAxis();
};
