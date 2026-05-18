/*---------------------------------------------------------------------*/
/*------------------------   Proyecto Final   -------------------------*/
/*----------------------  Edificio A Entrada  -------------------------*/
/*---------------------------    2026-2    ----------------------------*/
/*--------------------------    Alumnos:    ---------------------------*/
/*                                                                     */
/*   García Hernández Laura Lucía    No. 319031470                     */
/*   García Huerta Erick Bernardo    No. 317346222                     */
/*   Rosas Hernández Nahim           No. 314126900                     */
/*                                                                     */
/*---------------------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>

#include <miniaudio.h>
#include <vector>
#include <string>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
void animarBot(void);
void brazoBot(void);
void animarTren(void);
void LoadAnimFrames(void);
void animarMaquinita(void);

//=================Experimento de animacion =================//
std::vector<unsigned int> animFrames;

int currentFrame = 0;

float animTimer = 0.0f;
float animFPS = 8.0f;
boolean animacionMaquinita = false;
//=============== FIN Experimento de animacion ===============//
// tiempo entre frames
float animSpeed = 1.0f / animFPS;

//Para audio
ma_engine engine;
ma_sound music;

// settings
unsigned int SCR_WIDTH = 900;
unsigned int SCR_HEIGHT = 700;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(-1000.0f, 150.0f, 1000.0f));
float MovementSpeed = 0.1f;
GLfloat lastX = SCR_WIDTH / 2.0f,
lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

void getResolution(void);
void myData(void);							// De la practica 4
void LoadTextures(void);					// De la práctica 6

unsigned int generateTextures(char*, bool, bool);	// De la práctica 6

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	t_smile,
t_toalla,
t_unam,
t_white,
t_ladrillos;

//Posiciones de columnas
float Posicion_columnas_X,
Posicion_columnas_Z;
int Pasos_X[] = { 0, 3 },
Pasos_Z[] = { 0, 3, 6, 9, 12 };

//================ Para Botsito =================//

bool animacionBot = false;

int estadoBot = 1;
int estadoBrazo_Bot = 1;

float	movBot_x = 0.0f,
movBot_y = 0.0f,
movBot_z = 0.0f,
orientaBot = 90.0f,
giroBrazo_Bot = 0.0f,
giroBola_Bot = 0.0f;

// tiro parabólico
float Vini = 25.0f;
float angleInit = glm::radians(45.0f);
float g = 9.81f;

float mytime = 0.0f;
float dt = 0.1f;

// control de saltos
int numSaltos = 0;

// posiciones iniciales de cada salto
float inicioX = 0.0f;
float inicioZ = 0.0f;

//==================================================//
//================ Para Trenecito =================//

bool animacionTren = false;
bool puerta = false;

int estado_Tren = 1;

float	movTren_x = 0.0f,
movTren_y = 0.0f,
movTren_z = 0.0f,
orientaTren = 0.0f,
TrenSize = 0.0f;

// posiciones iniciales usables
float inicioTren_X = 0.0f;
float inicioTren_Y = 0.0f;
float inicioTren_Z = 0.0f;

//==================================================//


//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;

int npc2 = 0;
float npc2x = 0.0f;
float npc2z = 0.0f;
float npc2r = 0.0f;
bool npc2anim = true;

int npc3 = 0;
float npc3x = 0.0f;
float npc3z = 0.0f;
float npc3r = 0.0f;
bool npc3anim = true;

//Unidad cuadro Negro
float   Unidad_X = 244.0f,
Unidad_Z = 170.0f;


//Keyframes (Manipulación y dibujo)
float	posX = -220.0f,
posY = 0.0f,
posZ = 500.0f,
rotY = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f,
giroCabeza = 0.0f,
giroBrazoDer = 0.0f,
giroBrazoIzq = 0.0f;

float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotYInc = 0.0f,
rotRodIzqInc = 0.0f,
giroMonitoInc = 0.0f,
giroCabezaInc = 0.0f,
giroBrazoDerInc = 0.0f,
giroBrazoIzqInc = 0.0f;

#define MAX_FRAMES 30
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotY;		//Variable para RotacionY
	/*float rotRodIzq;
	float giroMonito;
	float giroCabeza;
	float giroBrazoDer;
	float giroBrazoIzq;*/

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
bool walk1 = false;
bool walk2 = true;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].rotY = rotY;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotY = KeyFrame[0].rotY;

}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotYInc = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;
}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	if (isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{

	t_smile = generateTextures("Texturas/awesomeface.png", 1, true);
	t_toalla = generateTextures("Texturas/toalla.tga", 0, true);
	t_unam = generateTextures("Texturas/escudo_unam.jpg", 0, true);
	t_ladrillos = generateTextures("Texturas/bricks.jpg", 0, true);
	//This must be the last
	t_white = generateTextures("Texturas/white.jpg", 0, false);
}


//=========================================================//
//                 Animar a botsito

void animarBot()
{
	if (estadoBot == 1) // 1. Seis saltos en x positiva
	{
		mytime += dt;

		movBot_x = inicioX + (Vini * mytime * cos(angleInit));

		movBot_y = (Vini * mytime * sin(angleInit))
			- (0.5f * g * pow(mytime, 2));

		if (movBot_y <= 0.0f)
		{
			movBot_y = 0.0f;

			mytime = 0.0f;
			inicioX = movBot_x;

			numSaltos++;

			if (numSaltos >= 6)
			{
				numSaltos = 0;
				estadoBot = 2;
			}
		}
	}

	else if (estadoBot == 2) // 2. Rotar que mire a Z+ (180°)
	{
		orientaBot += 3.0f;

		if (orientaBot >= 180.0f)
		{
			orientaBot = 180.0f;

			inicioZ = movBot_z;

			estadoBot = 3;
		}
	}

	else if (estadoBot == 3) // 3. Dos saltos en Z positiva
	{
		mytime += dt;

		movBot_z = inicioZ - (Vini * mytime * cos(angleInit));

		movBot_y = (Vini * mytime * sin(angleInit))
			- (0.5f * g * pow(mytime, 2));

		if (movBot_y <= 0.0f)
		{
			movBot_y = 0.0f;

			mytime = 0.0f;
			inicioZ = movBot_z;

			numSaltos++;

			if (numSaltos >= 2)
			{
				numSaltos = 0;
				estadoBot = 4;
			}
		}
	}

	else if (estadoBot == 4) // 4. Rotar que mire a X- (270°)
	{
		orientaBot += 3.0f;

		if (orientaBot >= 270.0f)
		{
			orientaBot = 270.0f;

			inicioX = movBot_x;

			estadoBot = 5;
		}
	}

	else if (estadoBot == 5) // 5. Seis saltos en x negativa
	{
		mytime += dt;

		movBot_x = inicioX - (Vini * mytime * cos(angleInit));

		movBot_y = (Vini * mytime * sin(angleInit))
			- (0.5f * g * pow(mytime, 2));

		if (movBot_y <= 0.0f)
		{
			movBot_y = 0.0f;

			mytime = 0.0f;
			inicioX = movBot_x;

			numSaltos++;

			if (numSaltos >= 6)
			{
				numSaltos = 0;
				estadoBot = 6;
			}
		}
	}

	else if (estadoBot == 6) 	// 6. Rotar que mire a Z- (360°)
	{
		orientaBot += 3.0f;

		if (orientaBot >= 360.0f)
		{
			orientaBot = 360.0f;

			inicioZ = movBot_z;

			estadoBot = 7;
		}
	}

	else if (estadoBot == 7) // 7. Dos saltos en z negativa
	{
		mytime += dt;

		movBot_z = inicioZ + (Vini * mytime * cos(angleInit));

		movBot_y = (Vini * mytime * sin(angleInit))
			- (0.5f * g * pow(mytime, 2));

		if (movBot_y <= 0.0f)
		{
			movBot_y = 0.0f;

			mytime = 0.0f;
			inicioZ = movBot_z;

			numSaltos++;

			if (numSaltos >= 2)
			{
				numSaltos = 0;
				estadoBot = 8;
			}
		}
	}

	else if (estadoBot == 8) // 8. Regresar a edo inicial (90°)
	{
		orientaBot += 3.0f;

		if (orientaBot >= 450.0f)
		{
			orientaBot = 90.0f;

			estadoBot = 1;
		}
	}
}
void brazoBot()
{
	if (animacionBot)
	{
		if (estadoBrazo_Bot == 1)
		{
			giroBrazo_Bot += 3.0f;
			if (giroBrazo_Bot >= 90.0f)
			{
				estadoBrazo_Bot = 2;
			}
		}
		else if (estadoBrazo_Bot == 2)
		{
			giroBrazo_Bot -= 3.0f;
			if (giroBrazo_Bot <= -90.0f)
			{
				estadoBrazo_Bot = 1;
			}
		}

	}
}
//=========================================================//
//                 Animar Trenecito
void animarTren()
{

	if (estado_Tren == 1)	// 1. Avanza hasta Z = 1000
	{
		movTren_z += 10.0f;

		if (movTren_z >= 1000.0f)
		{
			estado_Tren = 2;
		}
	}


	else if (estado_Tren == 2) 	// 2. Se hace pequeño 5 veces
	{
		// Escala original = 0.25
		// Queremos llegar aprox a 0.05
		TrenSize += 0.004f;

		// Compensación en Y para que no flote
		// altura aprox = 300
		movTren_y -= 1.35f;

		if (TrenSize >= 0.2f)
		{
			estado_Tren = 3;
		}
	}


	else if (estado_Tren == 3)	// 3. Regresa hasta Z = 500
	{
		movTren_z -= 6.0f;

		if (movTren_z <= 500.0f)
		{
			estado_Tren = 4;
		}
	}


	else if (estado_Tren == 4)	// 4. Recupera tamaño original
	{
		TrenSize -= 0.004f;

		movTren_y += 1.35f;

		if (TrenSize <= 0.0f)
		{
			TrenSize = 0.0f;
			movTren_y = 0.0f;

			estado_Tren = 5;
		}
	}


	else if (estado_Tren == 5)	// 5. Regresa al origen
	{
		movTren_z -= 6.0f;

		if (movTren_z <= 0.0f)
		{
			movTren_z = 0.0f;

			estado_Tren = 1;
		}
	}
}
//=========================================================//
//                 Animar Maquinita
void animarMaquinita()
{
	animTimer += dt;
	if (animTimer >= animSpeed)
	{
		currentFrame++;
		if (currentFrame >= animFrames.size())
		{
			currentFrame = 0;
			animacionMaquinita = false;
		}
		animTimer = 0.0f;
	}
}

void animate(void)
{
	if (puerta)
	{
		giroMonito += 5;
		if (giroMonito >= 425.0f)
		{
			puerta = false;
		}
	}
	

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;
			rotY += rotYInc;

			rotRodIzq += rotRodIzqInc;
			giroMonito += giroMonitoInc;
			giroCabeza += giroCabezaInc;
			giroBrazoDer += giroBrazoDerInc;
			giroBrazoIzq += giroBrazoIzqInc;

			i_curr_steps++;
		}
	}

	if (animacionBot) {
		animarBot();
		brazoBot();
		giroBola_Bot += 3.0f;
	}
	// Animación Trenecito
	if (animacionTren)
	{
		animarTren();
	}
	if (animacionMaquinita)
	{
		animarMaquinita();
	}
	// NPC 2
	if (npc2anim)
	{
		switch (npc2)
		{

		case 0: // Inicializar posición
			npc2x = 244.0f;
			npc2z = 170.0f;
			npc2r = 90.0f;

			npc2 = 1;
			break;

		case 1:
			npc2x += 2.5f;
			npc2r = 90.0f;

			if (npc2x >= 750.0f)
			{
				npc2 = 2;
			}
			break;

		case 2:
			npc2z += 2.5f;
			npc2r = 0.0f;

			if (npc2z >= 850.0f)
			{
				npc2 = 3;
			}
			break;

		case 3:
			npc2x += 2.5f;
			npc2r = 90.0f;

			if (npc2x >= 1500.0f)
			{
				npc2 = 4;
			}
			break;

		case 4:
			npc2z += 2.5f;
			npc2r = 0.0f;

			if (npc2z >= 1360.0f)
			{
				npc2 = 5;
			}
			break;

		case 5:
			npc2x += 2.5f;
			npc2r = 90.0f;

			if (npc2x >= 1800.0f)
			{
				npc2 = 6;
			}
			break;

		case 6:
			npc2z -= 2.5f;
			npc2r = 180.0f;

			if (npc2z <= 680.0f)
			{
				npc2 = 7;
			}
			break;

		case 7:
			npc2x -= 1.5f;
			npc2z -= 2.5f;
			npc2r = 225.0f;

			if (npc2x <= 1500.0f && npc2z <= 170.0f)
			{
				npc2 = 8;
			}
			break;

		case 8:
			// Regreso a la posición (750, 0, 170)
			// desde (1500, 170)

			npc2x -= 2.5f;
			npc2r = 270.0f;

			if (npc2x <= 750.0f)
			{
				npc2x = 750.0f;
				npc2z = 170.0f;
				npc2r = 0.0f;

				npc2 = 2;
			}
			break;

		}
	}

	// NPC 3
	if (npc3anim)
	{
		switch (npc3)
		{

		case 0: // Inicializar posición
			npc3x = 1708.0f;
			npc3z = 1870.0f;
			npc3r = 180.0f;

			npc3 = 1;
			break;

		case 1:
			npc3z -= 2.5f;
			npc3r = 180.0f;

			if (npc3z <= 340.0f)
			{
				npc3 = 2;
			}
			break;

		case 2:
			npc3x -= 2.5f;
			npc3r = 270.0f;

			if (npc3x <= 244.0f)
			{
				npc3 = 3;
			}
			break;

		case 3:
			npc3z += 2.5f;
			npc3r = 0.0f;

			if (npc3z >= 850.0f)
			{
				npc3 = 4;
			}
			break;

		case 4:
			npc3x -= 2.5f;
			npc3r = 270.0f;

			if (npc3x <= -120.0f)
			{
				npc3 = 5;
			}
			break;

		case 5:
			npc3x += 2.5f;
			npc3z -= 0.015f;
			npc3r = 90.0f;

			if (npc3x >= 1708.0f)
			{
				npc3x = 1708.0f;
				npc3z = 840.0f;

				npc3 = 9;
			}
			break;

		case 9:
			npc3z -= 2.5f;
			npc3r = 180.0f;

			if (npc3z <= 340.0f)
			{
				npc3 = 3;
			}
			break;

		}
	}
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData() {
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float verticesPiso[] = {
		// positions          // texture coords
		 10.5f,  10.5f, 0.0f,   4.0f, 4.0f, // top right
		 10.5f, -10.5f, 0.0f,   4.0f, 0.0f, // bottom right
		-10.5f, -10.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-10.5f,  10.5f, 0.0f,   0.0f, 4.0f  // top left 
	};
	unsigned int indicesPiso[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,	//V6
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V1
	};

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para Piso
	//glBindVertexArray(VAO[2]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPiso), verticesPiso, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPiso), indicesPiso, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//PARA CUBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pratica 6 2026-2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadAnimFrames();
	LoadTextures();

	// Este comentario es de otra rama
	myData();
	glEnable(GL_DEPTH_TEST);

	// Inicializar audio
	if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
	{
		std::cout << "Error inicializando audio" << std::endl;
		return -1;
	}

	if (ma_sound_init_from_file(
		&engine,
		"Bilewater.flac",
		MA_SOUND_FLAG_STREAM,
		NULL,
		NULL,
		&music) != MA_SUCCESS)
	{
		std::cout << "No se pudo cargar musica" << std::endl;
		return -1;
	}

	ma_sound_set_looping(&music, MA_TRUE);

	ma_sound_start(&music);

	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 

	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model carro_Seguridad("resources/objects/Lambo_Seguridad/carroceria.obj");
	Model llanta_Seguridad("resources/objects/Lambo_Seguridad/Wheel.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");

	//Escenario Edificio A
	Model Cubo_papel("resources/objects/cubo_papel/cubo_papel.obj");

	Model TechoFIA("resources/objects/Techo/Techo.obj");
	Model Letrero_A("resources/objects/Techo/Letrero_A/Letrero_A.obj");
	Model Letras_FI("resources/objects/Techo/Letras_FI/Letras_FI.obj");
	Model Barandal("resources/objects/Techo/Barandal/Barandal.obj");

	Model Columna_Eliptica("resources/objects/Columna_Eliptica/Columna_Eliptica.obj");
	Model Piso_Ver1("resources/objects/piso_A/piso_ver1.obj");
	Model Grass("resources/objects/Grass/Grass.obj");

	Model Plataforma_B("resources/objects/Plataforma_B/Plataforma_B.obj");
	Model Pared_negra("resources/objects/Pared_negra/Pared_negra.obj");
	//Stand y mesa
	Model Stand_Blanco("resources/objects/Stand_Blanco/Stand_Blanco.obj");
	Model Mesa("resources/objects/Stand_Blanco/Mesa/Mesa.obj");

	Model Maquinita("resources/objects/Maquinita/Maquinita.obj");



	Model Puerta_Reja("resources/objects/Puerta_Reja/Puerta_Reja.obj");
	Model Puerta_Animada("resources/objects/Puerta_Reja/Puerta_Animada/Puerta_Animada.obj");

	//Bicicleta
	Model Bici_Cuerpo("resources/objects/Bici/Bici_Cuerpo/Bici_cuerpo.obj");
	Model Bici_Rueda("resources/objects/Bici/Rueda/Rueda.obj");
	Model Bici_Pedal_Izq("resources/objects/Bici/Pedal_Der/Pedal_Der.obj");
	Model Bici_Pedal_Der("resources/objects/Bici/Columna_Eliptica/Columna_Eliptica.obj");
	Model Bici_Base_Pedal_Izq("resources/objects/Bici/Base_Pedal_Izq/Base_Pedal_Izq.obj");
	Model Bici_Base_Pedal_Der("resources/objects/Bici/Base_Pedal_Der/Base_Pedal_Der.obj");
	//Fin bicicleta
	// BOT
	Model BotBrazoDer("resources/objects/BOT/BotBrazoDer/BotBrazoDer.obj");
	Model BotBrazoIzq("resources/objects/BOT/BotBrazoIzq/BotBrazoIzq.obj");
	Model BotCentro("resources/objects/BOT/BotCentro/BotCentro.obj");
	Model BotBolita("resources/objects/BOT/BotBolita/BotBolita.obj");
	// FINBOT

	// Trenecito
	Model Trenecito("resources/objects/Trenecito/Trenecito.obj");
	Model Rieles("resources/objects/Trenecito/Rieles/Rieles.obj");
	//Animados
	// 
	//ModelAnim animacionPersonaje("resources/objects/Personaje1/Arm.dae");
	ModelAnim walking1("resources/objects/Remy/Walking.dae");
	ModelAnim idle1("resources/objects/Remy/Idle.dae");

	walking1.initShaders(animShader.ID);
	idle1.initShaders(animShader.ID);

	ModelAnim walking2("resources/objects/Remy/Walking.dae");
	ModelAnim idle2("resources/objects/Remy/Idle.dae");

	ModelAnim walking3("resources/objects/Megan/Walking.dae");
	ModelAnim idle3("resources/objects/Megan/Idle.dae");

	walking2.initShaders(animShader.ID);
	idle2.initShaders(animShader.ID);
	walking3.initShaders(animShader.ID);
	idle3.initShaders(animShader.ID);
	// animacionPersonaje.initShaders(animShader.ID);


	/*/Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotY = 0;

	}*/

	KeyFrame[0] = { -220.0f, 0.0f, 500.0f,  0.0f };
	KeyFrame[1] = { -220.0f, 0.0f, 627.5f,  0.0f };
	KeyFrame[2] = { -220.0f, 0.0f, 755.0f, 90.0f };
	KeyFrame[3] = {  -20.0f, 0.0f, 755.0f, 90.0f };
	KeyFrame[4] = {  180.0f, 0.0f, 755.0f, 90.0f };
	KeyFrame[5] = {  380.0f, 0.0f, 755.0f, 90.0f };
	KeyFrame[6] = {  580.0f, 0.0f, 755.0f, 90.0f};
	KeyFrame[7] = {  780.0f, 0.0f, 655.0f, 120.0f };
	KeyFrame[8] = { 976.0f, 0.0f, 510.0f, 120.0f };
	KeyFrame[9] = { 976.0f, 0.0f, 310.0f, 180.0f };
	KeyFrame[10] = { 976.0f, 0.0f, 170.0f, 180.0f };
	KeyFrame[11] = { 1176.0f, 0.0f, 170.0f,  90.0f };
	KeyFrame[12] = { 1376.0f, 0.0f, 170.0f,  90.0f };
	KeyFrame[13] = { 1576.0f, 0.0f, 170.0f,  90.0f };
	KeyFrame[14] = { 1776.0f, 0.0f, 170.0f,  90.0f };
	KeyFrame[15] = { 1952.0f, 0.1f, 170.0f,  90.0f };

	FrameIndex = 12;

	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", ambientColor);
		staticShader.setVec3("dirLight.diffuse", diffuseColor);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp_Bici = glm::mat4(1.0f);
		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/

		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		animacionPersonaje.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.38f));	// it's a bit too big for our scene, so scale it down
		modelOp = glm::rotate(modelOp, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		if (walk1)
		{
			walking1.Draw(animShader);
		}
		else
		{
			idle1.Draw(animShader);
		}

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(npc2x, 1.75f, npc2z));
		modelOp = glm::scale(modelOp, glm::vec3(0.38f));
		modelOp = glm::rotate(modelOp, glm::radians(npc2r), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		walking2.Draw(animShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(npc3x, 1.75f, npc3z));
		modelOp = glm::scale(modelOp, glm::vec3(0.85f));
		modelOp = glm::rotate(modelOp, glm::radians(npc3r), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		walking3.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		staticShader.setMat4("model", modelOp);
		//piso.Draw(staticShader);

		//Columnas Elípticas
		
		

		for (int x = 0; x < 2; x++)
		{
			Posicion_columnas_X = Unidad_X * 2.5f + Pasos_X[x] * Unidad_X; //Unidad_X = 244
			for (int z = 0; z < 5; z++)
			{
				Posicion_columnas_Z = Unidad_Z / 2.0f + Pasos_Z[z] * Unidad_Z; // Unidad_Z = 170
				modelOp = glm::mat4(1.0f);
				modelOp = glm::translate(modelOp, glm::vec3(
					Posicion_columnas_X,
					0.0f,
					Posicion_columnas_Z
				));
				//modelOp = glm::scale(modelOp, glm::vec3(1.59f, 1.34f, 1.12f));
				//modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				staticShader.setMat4("model", modelOp);
				Columna_Eliptica.Draw(staticShader);
			}
		}
		// Todo lo que sea piso **********************************************************
		// Piso Version1
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1952.0f, 0.0f, 0.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Piso_Ver1.Draw(staticShader);
		// Plataforma Blanca
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(Unidad_X / 3, 0.0f, Unidad_Z * 12));
		staticShader.setMat4("model", modelOp);
		Plataforma_B.Draw(staticShader);
		//Pastito
		for (int j = 0; j <= 6; j++) {
			for (int i = 1; i <= 5; i++) {
				modelOp = glm::translate(tmp, glm::vec3(-290 * i - 11, -5.0f, -290 * j));
				staticShader.setMat4("model", modelOp);
				Grass.Draw(staticShader);
			}
		}

		// FIN Todo lo que sea piso **********************************************************

		// Techo ***********************************************************************
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-244.0f, 300.0f, 0.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		TechoFIA.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(1423.6f, 17.33f, 5.0f));
		staticShader.setMat4("model", modelOp);
		Letrero_A.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(910.0f, 15.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Letras_FI.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Barandal.Draw(staticShader);
		// Fin Techo ***********************************************************************

		// Pared negra completa
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, 114.0f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Pared_negra.Draw(staticShader);

		// Puerta Reja amarilla
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, Unidad_Z / 2));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Puerta_Reja.Draw(staticShader);
		// Puerta Animada amarilla
		modelOp = glm::translate(tmp, glm::vec3(0.0f, 0.0f, giroMonito - 510.0f));
		staticShader.setMat4("model", modelOp);
		Puerta_Animada.Draw(staticShader);


		//Stands Blancos
		//Stand 1
		tmp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(Unidad_X * 4.0f - 100.0f, 17.0f, Unidad_Z * 9.0f - 50.0f)); //(4,9) unidad_cuadro
		staticShader.setMat4("model", modelOp);
		Stand_Blanco.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(70.0f, 0.0f, -160.0f)); //(4,9) unidad_cuadro
		modelOp = glm::scale(modelOp, glm::vec3(1.3f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Mesa.Draw(staticShader);

		///////////////////////////////// Botsito experimento /////////////////////
		modelOp = glm::translate(tmp, glm::vec3(movBot_x / 5 + 10, movBot_y / 5 + 60.0f, movBot_z / 5 - 10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaBot), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		BotCentro.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(-8.549f, 25.125f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroBrazo_Bot), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BotBrazoDer.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(8.549f, 25.125f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-giroBrazo_Bot), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BotBrazoIzq.Draw(staticShader);
		/*
		modelOp = glm::translate(tmp, glm::vec3(2.2f, 0.085f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroBola_Bot), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BotBolita.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(-2.2f, 0.085f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroBola_Bot), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BotBolita.Draw(staticShader);
		*/



		//Stand 2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(Unidad_X * 4.0f + 100.0f, 17.0f, Unidad_Z * 9.0f - 50.0f));//(4,9)
		staticShader.setMat4("model", modelOp);
		Stand_Blanco.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(Unidad_X * 4.0f + 100.0f, 17.0f, Unidad_Z * 9.0f - 60.0f));//(4,9)
		tmp=modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Maquinita.Draw(staticShader);

		// =====================================================================
		// Pantalla animada
		myShader.use();

		myShader.setMat4("projection", projectionOp);
		myShader.setMat4("view", viewOp);

		// Posición del plano
		modelOp = glm::translate(tmp, glm::vec3( 42.142f, 123.328f, -31.495f));
		// Rotación
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// Escala real
		modelOp = glm::scale(modelOp,glm::vec3(42.069f, 32.726f ,1.0f));

		myShader.setMat4("model", modelOp);

		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);

		// Activar textura animada
		glBindTexture(GL_TEXTURE_2D, animFrames[currentFrame]);

		// Dibujar plano
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);
		//modelOp = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto_x - 150.0f, -1.0f, movAuto_z + 150.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", modelOp);
		carro.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(8.5f * 5, 2.5f * 5, 12.9f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		llanta.Draw(staticShader);	//Izq delantera

		modelOp = glm::translate(tmp, glm::vec3(-8.5f * 5, 2.5f * 5, 12.9f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		llanta.Draw(staticShader);	//Der delantera

		modelOp = glm::translate(tmp, glm::vec3(-8.5f * 5, 2.5f * 5, -14.5f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		llanta.Draw(staticShader);	//Der trasera

		modelOp = glm::translate(tmp, glm::vec3(8.5f * 5, 2.5f * 5, -14.5f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro de puma seguridad
		// -------------------------------------------------------------------------------------------------------------------------
		//modelOp = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto_x - 350.0f, -1.0f, movAuto_z + 150.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orienta - 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", modelOp);
		carro_Seguridad.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(8.5f * 5, 2.5f * 5, 12.9f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		llanta_Seguridad.Draw(staticShader);	//Izq delantera

		modelOp = glm::translate(tmp, glm::vec3(-8.5f * 5, 2.5f * 5, 12.9f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		llanta_Seguridad.Draw(staticShader);	//Der delantera

		modelOp = glm::translate(tmp, glm::vec3(-8.5f * 5, 2.5f * 5, -14.5f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		llanta_Seguridad.Draw(staticShader);	//Der trasera

		modelOp = glm::translate(tmp, glm::vec3(8.5f * 5, 2.5f * 5, -14.5f * 5));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		llanta_Seguridad.Draw(staticShader);	//Izq trase

		// -------------------------------------------------------------------------------------------------------------------------
		// Bicicleta
		// -------------------------------------------------------------------------------------------------------------------------

		float EscalaBici = 2.0f;
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(Unidad_X * 2 / 3, 17.0f, Unidad_Z * 9.0f));
		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 1.0f, 300.0f));

		tmp_Bici = modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(EscalaBici));
		staticShader.setMat4("model", modelOp);
		Bici_Cuerpo.Draw(staticShader);

		modelOp = glm::translate(tmp_Bici, glm::vec3(17.5f * EscalaBici, 15.55f * EscalaBici, -0.14f * EscalaBici));
		modelOp = glm::rotate(modelOp, glm::radians(giroCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(EscalaBici));
		staticShader.setMat4("model", modelOp);
		Bici_Rueda.Draw(staticShader);

		modelOp = glm::translate(tmp_Bici, glm::vec3(-4.993f * EscalaBici, 12.191f * EscalaBici, -3.345f * EscalaBici));
		modelOp = glm::scale(modelOp, glm::vec3(EscalaBici));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(giroCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		Bici_Base_Pedal_Der.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(-2.403f, -5.039f, -2.228f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f));
		staticShader.setMat4("model", modelOp);
		Bici_Pedal_Izq.Draw(staticShader);

		modelOp = glm::translate(tmp_Bici, glm::vec3(-4.993f * EscalaBici, 12.77f * EscalaBici, 3.343f * EscalaBici));
		modelOp = glm::scale(modelOp, glm::vec3(EscalaBici));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(giroCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		Bici_Base_Pedal_Izq.Draw(staticShader);

		modelOp = glm::translate(tmp, glm::vec3(2.483f, 5.313f, 2.406f));
		modelOp = glm::rotate(modelOp, glm::radians(-giroCabeza), glm::vec3(0.0f, 0.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f));
		staticShader.setMat4("model", modelOp);
		Bici_Pedal_Izq.Draw(staticShader);

		modelOp = glm::translate(tmp_Bici, glm::vec3(0.0f)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.5f));
		staticShader.setMat4("model", modelOp);
		llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin Bicicleta
		// -------------------------------------------------------------------------------------------------------------------------

		// Trenecito
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(11 * Unidad_X - 66.0f, 20.0f, 0.6f * Unidad_Z));
		modelOp = glm::rotate(modelOp, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.25f));
		staticShader.setMat4("model", modelOp);
		Rieles.Draw(staticShader);



		//Lo ultimo que se dibuja es aquello que es transparente

		glEnable(GL_BLEND); //Caro computacionalmente
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Cubo papel
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 15.0f, 50.0f));
		//modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Cubo_papel.Draw(staticShader);

		// Trenecito
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movTren_x + 11 * Unidad_X, 130.0f + movTren_y, movTren_z + Unidad_Z));
		modelOp = glm::rotate(modelOp, glm::radians(orientaTren - 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.25f - TrenSize));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", modelOp);
		Trenecito.Draw(staticShader);

		glDisable(GL_BLEND);


		// -------------------------------------------------------------------------------------------------------------------------
		// Just in case
		// -------------------------------------------------------------------------------------------------------------------------
		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", modelOp);
		torso.Draw(staticShader);

		//Pierna Der
		modelOp = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		modelOp = glm::rotate(modelOp, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		piernaDer.Draw(staticShader);

		//Pie Der
		modelOp = glm::translate(modelOp, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", modelOp);
		botaDer.Draw(staticShader);

		//Pierna Izq
		modelOp = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		modelOp = glm::translate(modelOp, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", modelOp);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::translate(modelOp, glm::vec3(-0.75f, 2.5f, 0));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.75f, 2.5f, 0));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		brazoIzq.Draw(staticShader);

		//Cabeza
		modelOp = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", modelOp);
		cabeza.Draw(staticShader);*/

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------


		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	glfwTerminate();
	return 0;

	// liberar audio
	ma_engine_uninit(&engine);

	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		giroCabeza++;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		// Posición
		movBot_x = 0.0f;
		movBot_y = 0.0f;
		movBot_z = 0.0f;
		// Rotaciones
		orientaBot = 90.0f;
		giroBrazo_Bot = 0.0f;
		// Estados
		estadoBot = 1;
		estadoBrazo_Bot = 1;
		// Variables físicas
		mytime = 0.0f;
		numSaltos = 0;
		inicioX = 0.0f;
		inicioZ = 0.0f;
		// Flags
		animacionBot = false;
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		giroBrazoDer++;
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		camera.Position = (glm::vec3(-1000.0f, 150.0f, 1000.0f));
		camera.Yaw = 0.0f;
		camera.Pitch = 0.0f;
		camera.Front = glm::vec3(1.0f, 0.0f, 0.0f);
		camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Up = camera.WorldUp;
		firstMouse = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		camera.Position = (glm::vec3(Unidad_X * 4.0f - 100.0f, 160.0f, Unidad_Z * 7.0f));
		camera.Yaw = 90.0f;
		camera.Pitch = 0.0f;
		camera.Front = glm::vec3(0.0f, 0.0f, 1.0f);
		camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Up = camera.WorldUp;
		firstMouse = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		//En este transporte hay un pequeño error. Si nada más aparecer, presionas D, la camara se mueve adelante, si presionas A se mueve hacia atrás.
		// Si te mueves en otras direcciones se arregla
	{
		camera.Position = (glm::vec3(7 * Unidad_X, 180.0f, 5 * Unidad_Z));
		camera.Yaw = 0.0f;
		camera.Pitch = 0.0f;
		camera.Front = glm::vec3(1.0f, 0.0f, 0.0f);
		camera.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Up = camera.WorldUp;
		firstMouse = true;
	}

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacionTren ^= true;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		animacionBot ^= true;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		animacionMaquinita ^= true;
	}	

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			walk1 = true;
			puerta = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			walk1 = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	if (posY > 0)
	{
		walk1 = false;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void LoadAnimFrames()
{
    for (int i = 1; i <= 141; i++)
    {
        std::string path =
            "Texturas/Asteroids/text_" + std::to_string(i) + ".png";

        animFrames.push_back(
            generateTextures(path.c_str(), 1, true)
        );
    }
}