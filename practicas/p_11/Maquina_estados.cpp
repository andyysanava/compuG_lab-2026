/***************************************************************************************************
 * Andrea Saldaña Navarrete
 * 320206384
 ***************************************************************************************************/

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

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos (0.0f,0.0f,0.0f);
float dogRot = 0.0f;
bool step = false;

// ── Máquina de estados del recorrido rectangular ─────────────────────────────
// El perro nace en el centro (0,0,0) mirando hacia +Z (dogRot=0).
// Recorre los 4 lados del piso y regresa al centro, luego reinicia.
//
// Estados:
//   0  = WALK_1  : avanzar hasta borde +Z
//   1  = TURN_1  : girar  90° a la derecha (acumulado: -90°, mirando +X)
//   2  = WALK_2  : avanzar hasta borde +X
//   3  = TURN_2  : girar  90° a la derecha (acumulado: -180°, mirando -Z)
//   4  = WALK_3  : avanzar hasta borde -Z
//   5  = TURN_3  : girar  90° a la derecha (acumulado: -270°, mirando -X)
//   6  = WALK_4  : avanzar hasta borde -X
//   7  = TURN_4  : girar 135° a la derecha (acumulado: -405° ≡ -45°, diagonal al centro)
//   8  = RETURN  : avanzar en diagonal hasta llegar al centro (0,0,0)
//
// "Borde" = cuando cualquier componente de dogPos supera FLOOR_LIMIT.
// "Girar a la derecha desde perspectiva del perro" equivale a decrementar dogRot.

const float FLOOR_LIMIT   = 1.5f;   // radio del piso
const float WALK_SPEED    = 0.003f;  // unidades por frame
const float TURN_SPEED    = 1.2f;    // grados por frame
const float LEG_SPEED     = 1.2f;
const float LEG_MAX       = 20.0f;
const float HEAD_SPEED    = 0.4f;
const float TAIL_SPEED    = 0.6f;
const float CENTER_THRESH = 0.05f;   // distancia al centro para considerar "llegó"

int  dogState     = 0;       // estado actual de la máquina
float turnTarget  = 0.0f;   // dogRot objetivo al girar



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Andrea Saldaña Navarrete", nullptr, nullptr);

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
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

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
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

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


		glm::mat4 model(1);

	
		
		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp= model = glm::translate(model, dogPos);
		modelTemp= model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp; 
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp; 
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218)); 
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp; 
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader); 


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	    Ball.Draw(lightingShader); 
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
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
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
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
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_N])
	{
		AnimBall = !AnimBall;
		
	}
	
	if (keys[GLFW_KEY_B])
  {
		dogAnim = 1;

	}
}
// ── Helpers internos de la animación ─────────────────────────────────────────

// Anima las patas mientras el perro camina (trote cuadrúpedo).
static void animLegsWalk()
{
	if (!step)
	{
		FLegs += LEG_SPEED;  RLegs -= LEG_SPEED;
		head  += HEAD_SPEED; tail  += TAIL_SPEED;
		if (FLegs >= LEG_MAX) step = true;
	}
	else
	{
		FLegs -= LEG_SPEED;  RLegs += LEG_SPEED;
		head  -= HEAD_SPEED; tail  -= TAIL_SPEED;
		if (FLegs <= -LEG_MAX) step = false;
	}
}

// Suaviza las patas hacia la posición neutral (usado al detenerse / girar).
static void smoothLegsToZero()
{
	FLegs *= 0.85f;  RLegs *= 0.85f;
	head  *= 0.85f;  tail  *= 0.85f;
	if (fabs(FLegs) < 0.1f)
	{
		FLegs = 0.0f; RLegs = 0.0f; head = 0.0f; tail = 0.0f;
	}
}

// Devuelve el vector unitario "hacia adelante" dado el ángulo dogRot (grados).
// dogRot=0 → mira +Z,  dogRot=+90 → mira -X,  dogRot=+180 → mira -Z,  etc.
// (valores positivos giran en sentido antihorario visto desde arriba)
static glm::vec3 dogForward()
{
    float rad = glm::radians(dogRot);
    return glm::vec3(sin(rad), 0.0f, cos(rad));
}

// ── Lógica principal de animación ────────────────────────────────────────────
void Animation()
{
	if (AnimBall)
		rotBall += 0.4f;

	if (AnimDog)
		rotDog -= 0.6f;

	if (dogAnim != 1) return;  // animación del perro inactiva

	// ── Máquina de estados del recorrido rectangular ─────────────────────────
	// Los estados pares (0,2,4,6,8) son de CAMINAR.
	// Los estados impares (1,3,5,7) son de GIRAR.
	// Estado 8 es REGRESAR al centro.

	switch (dogState)
	{
	// ── WALK_1: avanzar hacia +Z ──────────────────────────────────────────
	case 0:
	{
		glm::vec3 fwd = dogForward();
		glm::vec3 nextPos = dogPos + fwd * WALK_SPEED;

		// Comprobamos si ya alcanzamos el borde en la dirección que caminamos
		bool hitBorder = (fabs(nextPos.x) >= FLOOR_LIMIT ||
		                  fabs(nextPos.z) >= FLOOR_LIMIT);

		if (!hitBorder)
		{
			dogPos = nextPos;
			animLegsWalk();
		}
		else
		{
			// Clamp al borde
			dogPos.x = glm::clamp(dogPos.x, -FLOOR_LIMIT, FLOOR_LIMIT);
			dogPos.z = glm::clamp(dogPos.z, -FLOOR_LIMIT, FLOOR_LIMIT);
			smoothLegsToZero();

			// Pasar al primer giro cuando las patas ya están quietas
			if (FLegs == 0.0f)
			{
				turnTarget = dogRot + 90.0f;  // 90° a la izquierda del perro
				dogState = 1;
			}
		}
		break;
	}

	// ── TURN_1 ────────────────────────────────────────────────────────────
	case 1:
	{
		smoothLegsToZero();
		float diff = turnTarget - dogRot;
		if (fabs(diff) > TURN_SPEED)
			dogRot += (diff > 0 ? TURN_SPEED : -TURN_SPEED);
		else
		{
			dogRot = turnTarget;
			dogState = 2;  // siguiente tramo
		}
		break;
	}

	// ── WALK_2 ────────────────────────────────────────────────────────────
	case 2:
	{
		glm::vec3 fwd = dogForward();
		glm::vec3 nextPos = dogPos + fwd * WALK_SPEED;

		bool hitBorder = (fabs(nextPos.x) >= FLOOR_LIMIT ||
		                  fabs(nextPos.z) >= FLOOR_LIMIT);

		if (!hitBorder)
		{
			dogPos = nextPos;
			animLegsWalk();
		}
		else
		{
			dogPos.x = glm::clamp(dogPos.x, -FLOOR_LIMIT, FLOOR_LIMIT);
			dogPos.z = glm::clamp(dogPos.z, -FLOOR_LIMIT, FLOOR_LIMIT);
			smoothLegsToZero();
			if (FLegs == 0.0f)
			{
				turnTarget = dogRot + 90.0f;
				dogState = 3;
			}
		}
		break;
	}

	// ── TURN_2 ────────────────────────────────────────────────────────────
	case 3:
	{
		smoothLegsToZero();
		float diff = turnTarget - dogRot;
		if (fabs(diff) > TURN_SPEED)
			dogRot += (diff > 0 ? TURN_SPEED : -TURN_SPEED);
		else
		{
			dogRot = turnTarget;
			dogState = 4;
		}
		break;
	}

	// ── WALK_3 ────────────────────────────────────────────────────────────
	case 4:
	{
		glm::vec3 fwd = dogForward();
		glm::vec3 nextPos = dogPos + fwd * WALK_SPEED;

		bool hitBorder = (fabs(nextPos.x) >= FLOOR_LIMIT ||
		                  fabs(nextPos.z) >= FLOOR_LIMIT);

		if (!hitBorder)
		{
			dogPos = nextPos;
			animLegsWalk();
		}
		else
		{
			dogPos.x = glm::clamp(dogPos.x, -FLOOR_LIMIT, FLOOR_LIMIT);
			dogPos.z = glm::clamp(dogPos.z, -FLOOR_LIMIT, FLOOR_LIMIT);
			smoothLegsToZero();
			if (FLegs == 0.0f)
			{
				turnTarget = dogRot + 90.0f;
				dogState = 5;
			}
		}
		break;
	}

	// ── TURN_3 ────────────────────────────────────────────────────────────
	case 5:
	{
		smoothLegsToZero();
		float diff = turnTarget - dogRot;
		if (fabs(diff) > TURN_SPEED)
			dogRot += (diff > 0 ? TURN_SPEED : -TURN_SPEED);
		else
		{
			dogRot = turnTarget;
			dogState = 6;
		}
		break;
	}

	// ── WALK_4 ────────────────────────────────────────────────────────────
	case 6:
	{
    glm::vec3 fwd = dogForward();
    glm::vec3 nextPos = dogPos + fwd * WALK_SPEED;

    bool hitBorder = (fabs(nextPos.x) >= FLOOR_LIMIT ||
                      fabs(nextPos.z) >= FLOOR_LIMIT);

    if (!hitBorder)
    {
        dogPos = nextPos;
        animLegsWalk();
    }
    else
    {
        dogPos.x = glm::clamp(dogPos.x, -FLOOR_LIMIT, FLOOR_LIMIT);
        dogPos.z = glm::clamp(dogPos.z, -FLOOR_LIMIT, FLOOR_LIMIT);

        smoothLegsToZero();

        if (FLegs == 0.0f)
        {
            // Dirección hacia el centro
            glm::vec3 toCenter = glm::normalize(-dogPos);

            // Ángulo destino
            float targetAngle =
                glm::degrees(atan2f(toCenter.x, toCenter.z));

            // Delta más corto (-180 a +180)
            float delta = targetAngle - dogRot;

            while (delta > 180.0f) delta -= 360.0f;
            while (delta < -180.0f) delta += 360.0f;

            turnTarget = dogRot + delta;

            dogState = 7;
        }
    }
    break;
	}

	// ── TURN_4: orientar hacia el centro ─────────────────────────────────
	case 7:
	{
		smoothLegsToZero();
		float diff = turnTarget - dogRot;
		if (fabs(diff) > TURN_SPEED)
			dogRot += (diff > 0 ? TURN_SPEED : -TURN_SPEED);
		else
		{
			dogRot = turnTarget;
			dogState = 8;  // avanzar hacia el centro
		}
		break;
	}

// ── RETURN: caminar al centro y reiniciar ─────────────────────────────
	case 8:
{
    float distToCenter = glm::length(dogPos);

    if (distToCenter > CENTER_THRESH)
    {
        glm::vec3 toCenter = glm::normalize(-dogPos);

        // Orientación deseada
        float targetAngle =
            glm::degrees(atan2f(toCenter.x, toCenter.z));

        // Girar suave hacia el frente
        float delta = targetAngle - dogRot;

        while (delta > 180.0f) delta -= 360.0f;
        while (delta < -180.0f) delta += 360.0f;

        dogRot += delta * 0.08f; // suavizado

        // Avanzar
        dogPos += toCenter * WALK_SPEED;

        animLegsWalk();
    }
    else
    {
        // Regreso limpio al origen
        dogPos = glm::vec3(0.0f);
        dogRot = 0.0f;
        dogState = 0;

        FLegs = 0.0f;
        RLegs = 0.0f;
        head = 0.0f;
        tail = 0.0f;
        step = false;
    }

    break;
}

	default:
		dogState = 0;
		break;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
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
