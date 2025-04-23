#include "Textures.h"
#include "resource.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <CoreFoundation/CoreFoundation.h>
// Single header image loading library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#else
#include <windows.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glaux.h>
#endif

#include <stdio.h>
#include <string.h>

GLuint texture[4];

#ifdef __APPLE__
// Image-loading texture implementation for macOS
void LoadGLTextures()
{
	// Texture filenames matching the Windows version resource IDs
	const char* textureFiles[] = {
		"brick.png",
		"wall.png",
		"help.png",
		"snake.png"
	};
	int textureCount = 4;
	
	glGenTextures(textureCount, &texture[0]);
	
	char filepath[PATH_MAX];
	for (int i = 0; i < textureCount; i++) {
		// Try to load from Resources directory
		snprintf(filepath, PATH_MAX, "Resources/%s", textureFiles[i]);
		
		// Load the image
		int width, height, channels;
		unsigned char* imageData = stbi_load(filepath, &width, &height, &channels, 0);
		
		if (imageData) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			
			// Set appropriate format based on channels in the image
			GLint format = GL_RGB;
			if (channels == 4) format = GL_RGBA;
			
			// Create the texture
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
			
			// Set filtering parameters to match Windows version
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			
			// Generate mipmaps
			gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, imageData);
			
			// Free the image data
			stbi_image_free(imageData);
			
			printf("loaded texture %s\n", textureFiles[i]);
		} else {
			printf("failed to load texture %s\n", textureFiles[i]);
			abort();
		}
	}
}

// Helper function to create procedural textures as fallback
void CreateProceduralTexture(int i)
{
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	
	// Create textures with different patterns to simulate the different textures in Windows
	unsigned char textureData[64][64][3];
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			// Different pattern based on texture index
			unsigned char c;
			switch(i) {
				case 0: // Brick
					c = ((x & 0x8) == 0) ^ ((y & 0x8) == 0) ? 255 : 0;
					break;
				case 1: // Wall
					c = ((x & 0xC) == 0) ^ ((y & 0xC) == 0) ? 200 : 100;
					break;
				case 2: // Help
					c = (x < 32) ? 180 : 80;
					break;
				case 3: // Snake
					c = ((x*x + y*y) % 256) > 128 ? 180 : 80;
					break;
			}
			
			// Set different colors based on texture type
			if (i == 0) { // Brick - reddish
				textureData[y][x][0] = c;
				textureData[y][x][1] = c/2;
				textureData[y][x][2] = c/2;
			} else if (i == 1) { // Wall - grayish
				textureData[y][x][0] = c;
				textureData[y][x][1] = c;
				textureData[y][x][2] = c;
			} else if (i == 2) { // Help - bluish
				textureData[y][x][0] = c/2;
				textureData[y][x][1] = c/2;
				textureData[y][x][2] = c;
			} else { // Snake - greenish
				textureData[y][x][0] = c/2;
				textureData[y][x][1] = c;
				textureData[y][x][2] = c/2;
			}
		}
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}
#else
void LoadGLTextures()											// Creates Textures From Bitmaps In The Resource File
{
	HBITMAP hBMP;												// Handle Of The Bitmap
	BITMAP	BMP;												// Bitmap Structure

	// The ID Of The 3 Bitmap Images We Want To Load From The Resource File
	byte	Texture[]={	IDB_BITMAP_BRICK, IDB_BITMAP_WALL, IDB_BITMAP_HELP, 
						IDB_BITMAP_SNAKE};

	glGenTextures(sizeof(Texture), &texture[0]);				
	for (int loop=0; loop<sizeof(Texture); loop++)
	{
		hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(Texture[loop]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		if (hBMP)												// Does The Bitmap Exist?
		{														// If So...
			GetObject(hBMP,sizeof(BMP), &BMP);					// Get The Object
																// hBMP: Handle To Graphics Object
																// sizeof(BMP): Size Of Buffer For Object Information
																// Buffer For Object Information
			glPixelStorei(GL_UNPACK_ALIGNMENT,4);				// Pixel Storage Mode (Word Alignment / 4 Bytes)
			glBindTexture(GL_TEXTURE_2D, texture[loop]);		// Bind Our Texture
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // Mipmap Linear Filtering

			// Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
			DeleteObject(hBMP);									// Delete The Bitmap Object
		}
	}
}

/*
void LoadGLTextures(char* fn, int index)									// Load Bitmaps And Convert To Textures
{
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(fn))
	{
		glGenTextures(1, &texture[index]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	else
		exit(0);

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}
}
*/
#endif

void initTextures()
{
	LoadGLTextures();
	/*
	LoadGLTextures("miro73.bmp", 0);
	LoadGLTextures("wall2.bmp", 1);
	LoadGLTextures("help.bmp", 2);
	*/
}

void drawBrick()
{
	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	{
		// Front Face
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f(0,0,-1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f(0,-1,0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f(1,0,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1,0,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawWalls(double cubeSize, bool openFront)
{
	//return;
	glPushMatrix();
	glTranslated(cubeSize/2, cubeSize/2, cubeSize/2);
	glScaled(cubeSize/2, cubeSize/2, cubeSize/2);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glBegin(GL_QUADS);
	{
		// Front Face
		glNormal3f(0,0,-1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
		glTexCoord2f(10.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(10.0f, 10.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f(0,0,1);
		glTexCoord2f(10.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(10.0f, 10.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face

		glNormal3f(0,-1,0);
		glTexCoord2f(0.0f, 10.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(10.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(10.0f, 10.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f(0,1,0);
		glTexCoord2f(10.0f, 10.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(10.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		if (!openFront)
		{
			glNormal3f(-1,0,0);
			glTexCoord2f(10.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
			glTexCoord2f(10.0f, 10.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 10.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		}
		// Left Face
		glNormal3f(1,0,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(10.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(10.0f, 10.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void drawHelp()
{
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPushMatrix();
	glLoadIdentity();
	glScaled(0.08, 0.08, 0.22);

	glBegin(GL_QUADS);
	{
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  -1.0f);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawSnakeSphere(GLUquadricObj* quadObj)
{
	gluQuadricTexture(quadObj, true);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	gluSphere(quadObj, 0.5, 32, 32); 
	glBindTexture(GL_TEXTURE_2D, 0);
}
