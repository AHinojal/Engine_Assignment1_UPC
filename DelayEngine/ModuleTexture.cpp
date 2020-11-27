#include "ModuleTexture.h"
#include <IL/il.h>
#include <GL/gl.h>
#include <SDL.h> 

ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{

}

bool ModuleTexture::Init()
{
	ilInit();
	// To Flip image that is top -left and we need bottom-up
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}

unsigned ModuleTexture::LoadTexture(char* fileData)
{
	ILuint texid;
	GLuint image;
	boolean success;

	ilGenImages(1, &texid); /* Generation of one image name */
	ilBindImage(texid); /* Binding of image name */
	//success = ilLoadImage("..\\Game\\test_lenna.png"); /* Loading of image "image.jpg" */
	success = ilLoadImage(fileData); /* Loading of image "image.jpg" */

	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			SDL_Quit();
		}
		glGenTextures(1, &image); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */
		// Flippin texture 

		glTexImage2D(GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_BPP),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */
	}
	else
	{
		/* Error occured */
		SDL_Quit();
	}
	ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data
   we can release memory used by image. */

	return image;
}
