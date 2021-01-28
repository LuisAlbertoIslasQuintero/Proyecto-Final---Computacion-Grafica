#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 9.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
bool act = true;
bool reco = false;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;

//Auto Recorrido
float movX = 0.0;
float movZ = 0.0;
float rotK = 0.0;

bool cir = false;
bool reco1 = true;
bool reco2 = false;
bool reco3 = false;
bool reco4 = false;
bool reco5 = false;
bool reco6 = true;
bool reco7 = false;
bool reco8 = false;
bool reco9 = false;
bool reco10 = false;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 1.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
bool active;

int luz = 0;

//Variables SkyBox
bool sky = false;
GLuint cubemapTexture;
int numsky;

//Animación del coche
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

//Animación del coche 2
float movKitX2 = 0.0;
float movKitZ2 = 0.0;
float rotKit2 = 0.0;

bool circuito2 = false;
bool recorrido10 = true;
bool recorrido20 = false;
bool recorrido30 = false;
bool recorrido40 = false;
bool recorrido50 = false;



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
//Variables globales
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;
float rotcabeza=0, rotpieizq=0, rotpieder = 0, rotbraizq = 0, rotbrader = 0, rotojo=0, rotship=0;
float MposX = 0, MposY = 0, MposZ = 0;
float rotdesin = 0, rotsilla=0, rotcristal=0;
#define MAX_FRAMES 10
int i_max_steps = 50;
int i_curr_steps = 0;
int i_curr_steps2 = 0;
int i_curr_steps3 = 0;

typedef struct _frame //Primer Animacion Compleja
{
	
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotcabeza;
	float rotInc;
	float rotpieizq;
	float rotInc2;
	float rotpieder;
	float rotInc3;
	float rotbraizq;
	float rotInc4;
	float rotbrader;
	float rotInc5;

}FRAME;

typedef struct _frame2 //Segunda Animacion Compleja
{

	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotojo;
	float rotInc6;

}FRAME2;

typedef struct _frame3 //Tercera Animacion Compleja
{

	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotship;
	float rotInc7;

}FRAME3;


FRAME KeyFrame[MAX_FRAMES];
FRAME2 KeyFrame2[MAX_FRAMES];
FRAME3 KeyFrame3[MAX_FRAMES];
int FrameIndex = 7;			//introducir datos
bool play = false;
bool play2 = false;
bool play3 = false;
bool play4 = false;
bool play5 = false;
bool play6 = false;
int playIndex = 0;
int playIndex2 = 0;
int playIndex3 = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;


void resetElements(void)
{
	MposX = KeyFrame[0].posX;
	MposY = KeyFrame[0].posY;
	MposZ = KeyFrame[0].posZ;

	rotcabeza = KeyFrame[0].rotcabeza;
	rotpieizq = KeyFrame[0].rotpieizq;
	rotpieder = KeyFrame[0].rotpieder;
	rotbraizq = KeyFrame[0].rotbraizq;
	rotbrader = KeyFrame[0].rotbrader;

}

void resetElements2(void)
{
	MposX = KeyFrame2[0].posX;
	MposY = KeyFrame2[0].posY;
	MposZ = KeyFrame2[0].posZ;

	rotojo = KeyFrame2[0].rotojo;
}

void resetElements3(void)
{
	MposX = KeyFrame3[0].posX;
	MposY = KeyFrame3[0].posY;
	MposZ = KeyFrame3[0].posZ;

	rotship = KeyFrame3[0].rotship;
}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotcabeza - KeyFrame[playIndex].rotcabeza) / i_max_steps;
	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotpieizq - KeyFrame[playIndex].rotpieizq) / i_max_steps;
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotpieder - KeyFrame[playIndex].rotpieder) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotbraizq - KeyFrame[playIndex].rotbraizq) / i_max_steps;
	KeyFrame[playIndex].rotInc5 = (KeyFrame[playIndex + 1].rotbrader - KeyFrame[playIndex].rotbrader) / i_max_steps;

}

void interpolation2(void)
{

	KeyFrame2[playIndex2].incX = (KeyFrame2[playIndex2 + 1].posX - KeyFrame2[playIndex2].posX) / i_max_steps;
	KeyFrame2[playIndex2].incY = (KeyFrame2[playIndex2 + 1].posY - KeyFrame2[playIndex2].posY) / i_max_steps;
	KeyFrame2[playIndex2].incZ = (KeyFrame2[playIndex2 + 1].posZ - KeyFrame2[playIndex2].posZ) / i_max_steps;

	KeyFrame2[playIndex2].rotInc6 = (KeyFrame2[playIndex2 + 1].rotojo - KeyFrame2[playIndex2].rotojo) / i_max_steps;
}

void interpolation3(void)
{

	KeyFrame3[playIndex3].incX = (KeyFrame3[playIndex3 + 1].posX - KeyFrame3[playIndex3].posX) / i_max_steps;
	KeyFrame3[playIndex3].incY = (KeyFrame3[playIndex3 + 1].posY - KeyFrame3[playIndex3].posY) / i_max_steps;
	KeyFrame3[playIndex3].incZ = (KeyFrame3[playIndex3 + 1].posZ - KeyFrame3[playIndex3].posZ) / i_max_steps;

	KeyFrame3[playIndex3].rotInc7 = (KeyFrame3[playIndex3 + 1].rotship - KeyFrame3[playIndex3].rotship) / i_max_steps;
}


int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	
		glfwSetKeyCallback(window, KeyCallback);
		if (act) {
		glfwSetCursorPosCallback(window, MouseCallback);
		}
		printf("%f", glfwGetTime());
	
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	Model Garage((char*)"Models/Garage/Garage.obj");
	Model mesa((char*)"Models/mesa/mesa.obj");
	Model lavabo((char*)"Models/lavabo/lavabo.obj");
	Model lavadora2((char*)"Models/lavadora2/lavadora2.obj");
	Model lavadora((char*)"Models/lavadora/lavadora.obj");
	Model estante((char*)"Models/estante/estante.obj");
	Model pizarron((char*)"Models/pizarron/pizarron.obj");
	Model pizarron2((char*)"Models/pizarron2/pizarron2.obj");
	Model reloj((char*)"Models/reloj/Obj.obj");
	Model pt((char*)"Models/pt/pt.obj");
	Model silla((char*)"Models/silla/silla.obj");
	Model desintegradora((char*)"Models/desintegradora/desintegradora.obj");
	Model desintegradora2((char*)"Models/desintegradora/desintegradora2.obj");
	Model buro((char*)"Models/buro/buro.obj");
	Model casco((char*)"Models/Casco/Casco1.obj");
	Model caja((char*)"Models/caja/caja.obj");
	Model caja2((char*)"Models/caja2/caja2.obj");
	Model ojo((char*)"Models/ojo/ojo.obj");
	Model ojo2((char*)"Models/ojo/ojo2.obj");
	Model cristal((char*)"Models/cristal/cristal.obj");
	Model cristal2((char*)"Models/cristal/cristal2.obj");
	Model sala((char*)"Models/sala/sala.obj");
	Model librero((char*)"Models/librero/librero.obj");
	Model lampara((char*)"Models/lampara/lampara.obj");
	/*Model Torso((char*)"Models/Personaje/cabeza.obj");*/
	Model cabeza((char*)"Models/cabeza/cabeza.obj");
	Model torso((char*)"Models/torso/torso.obj");
	Model brader((char*)"Models/brader/brader.obj");
	Model braizq((char*)"Models/braizq/braizq.obj");
	Model pieder((char*)"Models/pieder/pieder.obj");
	Model pieizq((char*)"Models/pieizq/pieizq.obj");
	Model Piso((char*)"Models/pasto/pasto.obj");
	Model entrada((char*)"Models/entrada/entrada.obj");
	Model cuartos((char*)"Models/cuartos/cuartos.obj");
	Model cuarto((char*)"Models/cuarto/cuarto.obj");
	Model vecindario((char*)"Models/vecindario/vecindario.obj");
	Model nave((char*)"Models/nave/nave.obj");
	Model carro((char*)"Models/carro/carro.obj");
	Model carro2((char*)"Models/carro2/carro2.obj");
	Model alberca((char*)"Models/alberca/alberca.obj");
	// Build and compile our shader program

	//Inicialización de KeyFrames

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotcabeza = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotpieizq = 0;
		KeyFrame[i].rotInc2 = 0;
		KeyFrame[i].rotpieder = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotbraizq = 0;
		KeyFrame[i].rotInc4 = 0;
		KeyFrame[i].rotbrader = 0;
		KeyFrame[i].rotInc5 = 0;

		KeyFrame2[i].rotojo = 0;
		KeyFrame2[i].rotInc6 = 0;

		KeyFrame3[i].rotship = 0;
		KeyFrame3[i].rotInc7 = 0;
	}

	KeyFrame[0].rotcabeza = 0;
	KeyFrame[1].rotcabeza = 45;
	KeyFrame[2].rotcabeza = -45;
	KeyFrame[3].rotcabeza = 45;
	KeyFrame[4].rotcabeza = -45;
	KeyFrame[0].rotbraizq = 0;
	KeyFrame[1].rotbraizq = 55;
	KeyFrame[2].rotbraizq = -35;
	KeyFrame[3].rotbraizq = 55;
	KeyFrame[4].rotbraizq =  -35;
	KeyFrame[0].rotbrader = 0;
	KeyFrame[1].rotbrader = -35;
	KeyFrame[2].rotbrader = 55;
	KeyFrame[3].rotbrader = -35;
	KeyFrame[4].rotbrader = 55;
	KeyFrame[0].rotpieizq = 0;
	KeyFrame[1].rotpieizq = 55;
	KeyFrame[2].rotpieizq = -35;
	KeyFrame[3].rotpieizq = 55;
	KeyFrame[4].rotpieizq = -35;
	KeyFrame[0].rotpieder = 0;
	KeyFrame[1].rotpieder = -35;
	KeyFrame[2].rotpieder = 55;
	KeyFrame[3].rotpieder = -35;
	KeyFrame[4].rotpieder = 55;

	//Ojo
	KeyFrame2[0].rotojo = 0;
	KeyFrame2[1].rotojo = 45;
	KeyFrame2[2].rotojo = -15;
	KeyFrame2[3].rotojo = 45;
	KeyFrame2[4].rotojo = -15;

	//Nave
	KeyFrame3[0].rotship = 0;
	KeyFrame3[1].rotship = 2160;
	KeyFrame3[2].rotship = -2160;
	KeyFrame3[3].rotship = 2160;
	KeyFrame3[4].rotship = -2160;
	KeyFrame3[5].rotship = 2160;
	KeyFrame3[6].rotship = 2160;
	KeyFrame3[7].rotship = -2160;
	KeyFrame3[8].rotship = 2160;
	KeyFrame3[9].rotship = -2160;



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f

			};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	vector<const GLchar*> faces2;
	faces2.push_back("SkyBox2/derecha.tga");
	faces2.push_back("SkyBox2/izquierda.tga");
	faces2.push_back("SkyBox2/superior.tga");
	faces2.push_back("SkyBox2/inferior.tga");
	faces2.push_back("SkyBox2/atras.tga");
	faces2.push_back("SkyBox2/frente.tga");

	//Arreglo de los skyboxes

	vector<const GLchar*> mysky[] = { faces,faces2 };



	cubemapTexture = TextureLoading::LoadCubemap(mysky[0]);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	//glm::mat4 projection = glm::perspective(camera.GetZoom(), movX, 0.1f, movZ);



	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);

		switch(luz)
		{ 
		case 0:
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.40f, 0.40f, 0.40f);
			break;
		default:
			break;
		}
		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		
		

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelo 
		//mesa
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		if (reco) {
			camera.Recorrido(rotK);//Funcion rotación de la camara
			camera.MovimientoAutomatico(movCamera);//Función de movimiento automatico
		}
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		/*Torso.Draw(lightingShader);*/
		//Mesa
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(40.0f, -2.0f, 10.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);
		//Lavabo
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(56.0f, -2.0f, 25.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lavabo.Draw(lightingShader);
		//Garage
	/*	Camera camera(glm::vec3(-100.0f, 2.0f, -45.0f), glm::vec3(0.0f, 1.0f, 0.0f), rotK);*/
		
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Garage.Draw(lightingShader);
		
	
		/*view = glm::rotate(view, glm::radians(rotK), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));*/
		
		//Lavadora2
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(48.0f, -2.2f, 25.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lavadora2.Draw(lightingShader);
		//Lavadora
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(48.0f, -1.3f, 25.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lavadora.Draw(lightingShader);
		//Estante
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(30.0f, 6.0f, 40.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		estante.Draw(lightingShader);
		//Pizarron
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(52.3f, 10.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pizarron.Draw(lightingShader);
		//Pizarron2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(41.0f, 14.0f, 16.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pizarron2.Draw(lightingShader);
		//Reloj
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(52.0f, 15.0f, 25.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		reloj.Draw(lightingShader);
		//Silla
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(40.0f, 3.9f, 15.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotsilla), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		silla.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.2f, -1.0f, 17.4f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pt.Draw(lightingShader);
		//Desintegradora
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(31.2f, 9.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotdesin), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		desintegradora.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(31.2f, 9.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		desintegradora2.Draw(lightingShader);
		//Buro
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(44.0f, 8.5f, 7.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		buro.Draw(lightingShader);
		//Casco
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(40.0f, 8.0f, 6.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		casco.Draw(lightingShader);
		//Caja
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(30.5f, 0.5f, 42.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		caja.Draw(lightingShader);
		//Caja2
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(29.5f, 0.6f, 50.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		caja2.Draw(lightingShader);
		//Ojo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(31.0f, 6.9f, 40.7f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ojo.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(32.5f, 8.0f, 40.7f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotojo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ojo2.Draw(lightingShader);
		//Cristal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(31.0f, 13.4f, 42.4f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cristal.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(32.0f, 17.5f, 42.3f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotcristal), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cristal2.Draw(lightingShader);
		//Sala
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sala.Draw(lightingShader);
		//Librero
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(54.0f, 1.3f, 29.43f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		/*model = glm::scale(tmp, glm::vec3(0.02f, 0.02f, 0.02f));*/
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		librero.Draw(lightingShader);
		//Lampara
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(71.0f, -1.7f, -16.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lampara.Draw(lightingShader);
		//Torso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(81.5f, 9.7f, 24.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		torso.Draw(lightingShader);
		//Cabeza
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(81.5f, 14.5f, 25.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotcabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cabeza.Draw(lightingShader);
		//Brazo Derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(82.6f, 14.3f, 25.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotbrader), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		brader.Draw(lightingShader);
		//Brazo Izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(80.7f, 14.3f, 25.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotbraizq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		braizq.Draw(lightingShader);
		//Pie Derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(80.6f, 10.0f, 25.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotpieder), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pieder.Draw(lightingShader);
		//Pie Izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(82.4f, 10.0f, 25.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotpieizq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pieizq.Draw(lightingShader);
		//Piso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.02f, 1.02f, 1.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		//Entrada
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		entrada.Draw(lightingShader);
		//Cuartos
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuartos.Draw(lightingShader);
		//Cuarto
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuarto.Draw(lightingShader);
		//Vecindario
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		vecindario.Draw(lightingShader);
		//Nave
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-98.0f, 32.7f, 134.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotship), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		nave.Draw(lightingShader);
		//Carro
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-18.0f, -1.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carro.Draw(lightingShader);
		//Carro2
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -1.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, PosIni + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		carro2.Draw(lightingShader);
		//Alberca
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(38.0f, -0.7f, 24.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		alberca.Draw(lightingShader);
		glBindVertexArray(0);
		//Perro
		glBindVertexArray(lightVAO);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//Cuerpo
		model = glm::translate(model, glm::vec3(12.2f, 3.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::scale(model, glm::vec3(9.0f, 3.0f, 3.7f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//Cabeza
		model = glm::translate(model, glm::vec3(9.6f, 5.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::scale(model, glm::vec3(3.5f, 3.0f, 3.7f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//pata delantera derecha
		model = glm::translate(model, glm::vec3(9.0f, 0.5f, 13.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.8f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//pata delantera izquierda
		model = glm::translate(model, glm::vec3(9.0f, 0.5f, 11.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.8f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//pata trasera derecha
		model = glm::translate(model, glm::vec3(16.0f, 0.5f, 13.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.8f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);//pata trasera izquierda
		model = glm::translate(model, glm::vec3(16.0f, 0.5f, 11.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.8f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);//Oreja izquierda
		model = glm::translate(model, glm::vec3(9.3f, 6.5f, 13.3f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);//Oreja derecha
		model = glm::translate(model, glm::vec3(9.3f, 6.5f, 10.8f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);//trompa
		model = glm::translate(model, glm::vec3(7.53f, 5.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);//cola
		model = glm::translate(model, glm::vec3(19.0f, 3.0f, 12.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		cubemapTexture = TextureLoading::LoadCubemap(mysky[numsky]); //Esto es necesario para que el skybox cambie dinamicamente
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{
	//Autorecorrido
	if (cir)
	{
		if (reco1)
		{
			movCamera += 0.006f;
			rotK = 90;
			if (movCamera > 0.5)
			{
				reco1 = false;
				reco2 = true;
			}
		}

		if (reco2)
		{
			movCamera += 0.006f;
			rotK = 0;
			if (movCamera > 1.1)
			{
				reco2 = false;
				reco3 = true;
			}
		}

		if (reco3)
		{
			movCamera += 0.006f;
			rotK = 90;
			if (movCamera > 1.5)
			{
				reco3 = false;
				reco4 = true;
			}
		}

		if (reco4)
		{
			movCamera += 0.006f;
			rotK = 180;
			if (movCamera > 2.0)
			{
				reco4 = false;
				reco5 = true;
			}
		}

		if (reco5)
		{
			movCamera -= 0.006f;
			rotK = 90;
			if (movCamera > 2.6)
			{
				reco5 = false;
				reco6 = true;
			}
		}

		if (reco6)
		{
			movCamera -= 0.006f;
			rotK = 90;
			sky = !sky;
			if (movCamera > -1.0)
			{
				reco6 = false;
				reco1 = true;
			}
		}

	}
	//Movimiento del coche
	if (circuito)
	{
		if (recorrido1)
		{
			movKitZ += 2.5f;
			rotKit = 0;
			if (movKitZ > 250)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotKit = 90;
			movKitX += 2.5f;
			if (movKitX > 40)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}

		if (recorrido3)
		{
			rotKit = 180;
			movKitZ -= 2.5f;
			if (movKitZ < -70)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotKit = -90;
			movKitX -= 2.5f;
			if (movKitX < 0)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			rotKit = 90;
			movKitZ += 2.5f;
			if (movKitZ < 0)
			{
				recorrido5 = false;
				recorrido1 = true;
			}
		}


	}

	//Movimiento del coche 2
	if (circuito2)
	{
		if (recorrido10)
		{
			movKitZ2 -= 2.5f;
			rotKit2 = 0;
			if (movKitZ2 < -70)
			{
				recorrido10 = false;
				recorrido20 = true;
			}
		}
		if (recorrido20)
		{
			rotKit2 = 90;
			movKitX2 -= 2.5f;
			if (movKitX2 < -40)
			{
				recorrido20 = false;
				recorrido30 = true;

			}
		}

		if (recorrido30)
		{
			rotKit2 = 180;
			movKitZ2 += 2.5f;
			if (movKitZ2 > 230)
			{
				recorrido30 = false;
				recorrido40 = true;
			}
		}

		if (recorrido40)
		{
			rotKit2 = -90;
			movKitX2 += 2.5f;
			if (movKitX2 > 0)
			{
				recorrido40 = false;
				recorrido50 = true;
			}
		}
		if (recorrido50)
		{
			rotKit2 = 0;
			movKitZ2 -= 2.5f;
			if (movKitZ2 < 0)
			{
				recorrido50 = false;
				recorrido10 = true;
			}
		}


	}

	if (play3)
	{
		if (rotdesin>-360.0f) {
				rotdesin -= 3.0f;
			
		}
	
		else {
			play3 = false;
		}
	}
	if (play4)
	{
		if (rotsilla > -360.0f) {
			rotsilla -= 2.5f;

		}

		else {
			play4 = false;
		}
	}

	if (play5)
	{
		if (rotcristal > -360.0f) {
			rotcristal -= 3.0f;

		}

		else {
			play5 = false;
		}
	}
	//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
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
			MposX += KeyFrame[playIndex].incX;
			MposY += KeyFrame[playIndex].incY;
			MposZ += KeyFrame[playIndex].incZ;

			rotcabeza += KeyFrame[playIndex].rotInc;
			rotpieizq += KeyFrame[playIndex].rotInc2;
			rotpieder += KeyFrame[playIndex].rotInc3;
			rotbraizq += KeyFrame[playIndex].rotInc4;
			rotbrader += KeyFrame[playIndex].rotInc5;
			i_curr_steps++;
		}

	}


	if (play2)
	{
		if (i_curr_steps2 >= i_max_steps) //end of animation between frames?
		{
			playIndex2++;
			if (playIndex2 > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex2 = 0;
				play2 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps2 = 0; //Reset counter
								  //Interpolation
				interpolation2();
			}
		}
		else
		{
			//Draw animation
			MposX += KeyFrame2[playIndex2].incX;
			MposY += KeyFrame2[playIndex2].incY;
			MposZ += KeyFrame2[playIndex2].incZ;

			rotojo += KeyFrame2[playIndex2].rotInc6;
			i_curr_steps2++;
		}

	}

	if (play6)
	{
		if (i_curr_steps3 >= i_max_steps) //end of animation between frames?
		{
			playIndex3++;
			if (playIndex3 > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex3 = 0;
				play6 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps3 = 0; //Reset counter
								  //Interpolation
				interpolation3();
			}
		}
		else
		{
			//Draw animation
			MposX += KeyFrame3[playIndex3].incX;
			MposY += KeyFrame3[playIndex3].incY;
			MposZ += KeyFrame3[playIndex3].incZ;

			rotship += KeyFrame3[playIndex3].rotInc7;
			i_curr_steps3++;
		}

	}
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_P])
	{
		if (play3 == false)
		{
			rotdesin = 0.0f;
			play3 = true;
		}
	}
	if (keys[GLFW_KEY_I])
	{
		if (play4 == false)
		{
			rotsilla = 0.0f;
			play4 = true;
		}
	}
	if (keys[GLFW_KEY_K])
	{
		if (play5 == false)
		{
			rotcristal = 0.0f;
			play5 = true;
		}
	}
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotcabeza = 0;
			rotbraizq = 0;
			rotbrader = 0;
			rotpieizq = 0;
			rotpieder = 0;
			
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_O])
	{
		if (play2 == false && (FrameIndex > 1))
		{
			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotojo = 0;
			resetElements2();
			//Second Interpolation				
			interpolation2();

			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			play2 = false;
		}

	}

	if (keys[GLFW_KEY_X])
	{
		if (play6 == false && (FrameIndex > 1))
		{
			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotship = 0;
			resetElements3();
			//Third Interpolation				
			interpolation3();

			play6 = true;
			playIndex3 = 0;
			i_curr_steps3 = 0;
		}
		else
		{
			play6 = false;
		}

	}

	if (keys[GLFW_KEY_E])
	{
		sky = !sky;
		printf("El numero de sky es: %i", numsky);
		if (sky)
		{
			numsky = 1;


		}
		else
		{
			numsky = 0;
		}

		luz = !luz;
		if (luz)
		{
			luz = 1;


		}
		else
		{
			luz = 0;
		}
	}

	if (keys[GLFW_KEY_R])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		circuito = true;
		circuito2 = true;
	}

	if (keys[GLFW_KEY_T])
	{
		circuito = false;
		circuito2 = false;
	}

	if (keys[GLFW_KEY_F])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		reco = true;
		cir = true;
		act = false;



		if (play3 == false)
		{
			rotdesin = 0.0f;
			play3 = true;
		}
	
		if (play4 == false)
		{
			rotsilla = 0.0f;
			play4 = true;
		}
	
		if (play5 == false)
		{
			rotcristal = 0.0f;
			play5 = true;
		}
	
		if (play == false && (FrameIndex > 1))
		{

			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotcabeza = 0;
			rotbraizq = 0;
			rotbrader = 0;
			rotpieizq = 0;
			rotpieder = 0;

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	
		if (play2 == false && (FrameIndex > 1))
		{
			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotojo = 0;
			resetElements2();
			//First Interpolation				
			interpolation2();

			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			play2 = false;
		}

		//rotK = -25.0f;//Variable que maneja el giro de la camara
	 
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		circuito = true;
		circuito2 = true;

		if (play6 == false && (FrameIndex > 1))
		{
			MposX = 0;
			MposY = 0;
			MposZ = 0;
			rotship = 0;
			resetElements3();
			//Third Interpolation				
			interpolation3();

			play6 = true;
			playIndex3 = 0;
			i_curr_steps3 = 0;
		}
		else
		{
			play6 = false;
		}
	}

	if (keys[GLFW_KEY_Q])
	{
		reco = false;
		act = true;
		cir = false;
	
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{

		rot += 1;

	}
	if (keys[GLFW_KEY_0])
	{

		rot -= 1;

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotpieizq < 80.0f)
			rotpieizq += 0.5f;

	}

	if (keys[GLFW_KEY_3])
	{
		if (rotpieizq > -45)
			rotpieizq -= 0.5f;

	}
	if (keys[GLFW_KEY_4])
	{
		if (rotpieder < 80.0f)
			rotpieder += 0.5f;

	}

	if (keys[GLFW_KEY_5])
	{
		if (rotpieder > -45)
			rotpieder -= 0.5f;

	}
	if (keys[GLFW_KEY_6])
	{
		if (rotbrader < 80.0f)
			rotbrader += 0.5f;

	}

	if (keys[GLFW_KEY_7])
	{
		if (rotbrader > -45)
			rotbrader -= 0.5f;

	}
	if (keys[GLFW_KEY_8])
	{
		if (rotbraizq < 80.0f)
			rotbraizq += 0.5f;

	}

	if (keys[GLFW_KEY_9])
	{
		if (rotbraizq > -45)
			rotbraizq -= 0.5f;

	}
	if (keys[GLFW_KEY_Q])
	{
		if (rotcabeza < 80.0f)
			rotcabeza += 0.5f;

	}

	if (keys[GLFW_KEY_Z])
	{
		if (rotcabeza > -45)
			rotcabeza -= 0.5f;

	}

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}





}
