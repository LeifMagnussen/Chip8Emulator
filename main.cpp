#include "stdafx.h"


#include <stdio.h> // including stdio.h here to ensure its included before glut.h
#include <glut.h>
#include "Chip8.h"
#include "opCodes.h"



// injection point for emulator and sets up OpenGL using GLut
using namespace std;

#define WIDTH 64
#define HEIGHT 32

const int MODIFIER = 10;
Chip8 myChip8; // global chip8 object

int displayWidth = WIDTH * MODIFIER;
int displayHeight  = HEIGHT * MODIFIER;

// main functions for GLUT mostly using the same structure as @Laurence Muller (See README)
void display();
void reshapeWindow(GLsizei width, GLsizei height);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);

// use new drawing method
#define DRAWWITHTEXTURE
typedef unsigned __int8 u8;
u8 screenData[HEIGHT][WIDTH][3];

// setup texture method
void setupTexture();


int main(int argc, char* argv[])
{
    if (argc < 2)
        myChip8.exit = true;
    // set up render system and regsiter input callback
    myChip8.initialize();
    myChip8.loadGame(argv[0]);

    // Setup OpenGL
	glutInit(&argc, argv);          
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(displayWidth, displayHeight);
    glutInitWindowPosition(320, 320);
	glutCreateWindow("Chip8Emu");
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
    glutReshapeFunc(reshapeWindow);        
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp); 

    #ifdef DRAWWITHTEXTURE
        setupTexture();			
    #endif	


    // Emulation Loop
    for (;;)
    {
        myChip8.emulateCycle();
        break;

        // check for draw flag and update the screen
        if (myChip8.drawFlag)
            myChip8.drawGraphics();

        // check for exit on esc press
        if (myChip8.exit)
            break;
    }

    return(0);

} 

// Setup Texture
void setupTexture()
{
	// Clear screen
	for(int y = 0; y < HEIGHT; ++y)		
		for(int x = 0; x < WIDTH; ++x)
			screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;

	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void updateTexture(const Chip8& myChip)
{	
	// Update pixels
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
			if(myChip.gfx[(y * 64) + x] == 0)
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;	// Disabled
			else 
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;  // Enabled
		
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(displayWidth, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(displayWidth, displayHeight);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  displayHeight);
	glEnd();
}

// Old gfx code
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + 0.0f,	 0.0f);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const Chip8& myChip)
{
	// Draw
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
		{
			if(myChip.gfx[(y*64) + x] == 0) 
				glColor3f(0.0f,0.0f,0.0f);			
			else 
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display()
{
	myChip8.emulateCycle();
		
	if(myChip8.drawFlag)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
        
#ifdef DRAWWITHTEXTURE
		updateTexture(myChip8);
#else
		updateQuads(myChip8);		
#endif			

		// Swap buffers!
		glutSwapBuffers();    

		// Processed frame
		myChip8.drawFlag = false;
	}

    if (myChip8.exit)
        exit(EXIT_FAILURE);
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	displayWidth = w;
	displayHeight = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		myChip8.exit = true;

	if(key == '1')		myChip8.key[0x1] = 1;
	else if(key == '2')	myChip8.key[0x2] = 1;
	else if(key == '3')	myChip8.key[0x3] = 1;
	else if(key == '4')	myChip8.key[0xC] = 1;

	else if(key == 'q')	myChip8.key[0x4] = 1;
	else if(key == 'w')	myChip8.key[0x5] = 1;
	else if(key == 'e')	myChip8.key[0x6] = 1;
	else if(key == 'r')	myChip8.key[0xD] = 1;

	else if(key == 'a')	myChip8.key[0x7] = 1;
	else if(key == 's')	myChip8.key[0x8] = 1;
	else if(key == 'd')	myChip8.key[0x9] = 1;
	else if(key == 'f')	myChip8.key[0xE] = 1;

	else if(key == 'z')	myChip8.key[0xA] = 1;
	else if(key == 'x')	myChip8.key[0x0] = 1;
	else if(key == 'c')	myChip8.key[0xB] = 1;
	else if(key == 'v')	myChip8.key[0xF] = 1;
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		myChip8.key[0x1] = 0;
	else if(key == '2')	myChip8.key[0x2] = 0;
	else if(key == '3')	myChip8.key[0x3] = 0;
	else if(key == '4')	myChip8.key[0xC] = 0;

	else if(key == 'q')	myChip8.key[0x4] = 0;
	else if(key == 'w')	myChip8.key[0x5] = 0;
	else if(key == 'e')	myChip8.key[0x6] = 0;
	else if(key == 'r')	myChip8.key[0xD] = 0;

	else if(key == 'a')	myChip8.key[0x7] = 0;
	else if(key == 's')	myChip8.key[0x8] = 0;
	else if(key == 'd')	myChip8.key[0x9] = 0;
	else if(key == 'f')	myChip8.key[0xE] = 0;

	else if(key == 'z')	myChip8.key[0xA] = 0;
	else if(key == 'x')	myChip8.key[0x0] = 0;
	else if(key == 'c')	myChip8.key[0xB] = 0;
	else if(key == 'v')	myChip8.key[0xF] = 0;
}


