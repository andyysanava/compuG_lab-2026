/****************************************************************************************************
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

// ─── Animación: Pelota ────────────────────────────────────────────────────────
float rotBall  = 0.0f;   // ángulo de revolución de la pelota (grados)
bool  AnimBall = false;

// Radio de órbita (mismo para los dos, círculo compartido)
const float ORBIT_RADIUS = 1.5f;

// ─── Animación: Perro ─────────────────────────────────────────────────────────
// El perro gira en sentido contrario, por eso su ángulo se decrementa.
float rotDog = 180.0f;  // empieza en el lado opuesto de la pelota

// ─── Brinco del perro ────────────────────────────────────────────────────────
bool  dogJumping    = false;
float dogJumpTimer  = 0.0f;
const float JUMP_DURATION    = 0.6f;   // segundos que dura el salto completo
const float JUMP_HEIGHT      = 0.5f;   // altura máxima del brinco
const float HEADBUTT_ANGLE   = 40.0f;  // grados de inclinación de la cabeza al golpear
float dogOffsetY    = 0.0f;            // desplazamiento vertical actual del perro
float dogHeadPitch  = 0.0f;            // ángulo de cabezazo actual (eje X local)

// ─── Rebote de la pelota ──────────────────────────────────────────────────────
bool  ballBouncing    = false;
float ballBounceTimer = 0.0f;
const float BOUNCE_DURATION = 1.2f;   // más largo para que la bajada se sienta natural
const float BOUNCE_HEIGHT   = 0.45f;  // altura del golpe
float ballOffsetY = 0.0f;

// ─── Detección de encuentro ───────────────────────────────────────────────────
// Se dispara cuando la diferencia angular entre perro y pelota < umbral
const float MEET_THRESHOLD = 3.0f;  // grados
bool wasNear = false;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Andrea Saldana Navarrete", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Dog((char*)"Models/RedDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),  0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),  0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),  pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),   lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),   lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),  1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"),  1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"),    0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),   camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),  camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),    0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),    0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),   0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"),   1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"),     0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"),  0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),     glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc  = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // ── Piso ─────────────────────────────────────────────────────────────
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // ── Perro: órbita en sentido contrario ───────────────────────────────
        // El perro gira en sentido negativo; se traslada al borde del círculo
        // y rota sobre su eje Y para mirar siempre hacia el centro.
        // En el pico del salto inclina la cabeza hacia arriba (cabezazo).
        {
            float dogRad = glm::radians(rotDog);
            model = glm::mat4(1);
            // Brinco vertical
            model = glm::translate(model, glm::vec3(0.0f, dogOffsetY, 0.0f));
            // Posición orbital
            model = glm::translate(model, glm::vec3(
                ORBIT_RADIUS * sin(dogRad),
                0.0f,
                ORBIT_RADIUS * cos(dogRad)
            ));
            // Orientación: el perro mira hacia el centro del círculo
            model = glm::rotate(model, dogRad, glm::vec3(0.0f, 1.0f, 0.0f));
            // Cabezazo: inclinación en eje X local (solo cuando salta)
            // Se aplica DESPUÉS de la rotación Y para que sea relativa al frente del perro
            if (dogHeadPitch != 0.0f)
                model = glm::rotate(model, glm::radians(dogHeadPitch), glm::vec3(1.0f, 0.0f, 0.0f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
            Dog.Draw(lightingShader);
        }

        // ── Pelota: órbita normal + rebote vertical ───────────────────────────
        {
            float ballRad = glm::radians(rotBall);
            model = glm::mat4(1);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // Rebote vertical
            model = glm::translate(model, glm::vec3(0.0f, ballOffsetY, 0.0f));
            // Posición orbital
            model = glm::translate(model, glm::vec3(
                ORBIT_RADIUS * sin(ballRad),
                0.0f,
                ORBIT_RADIUS * cos(ballRad)
            ));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
            Ball.Draw(lightingShader);
            glDisable(GL_BLEND);
        }

        glBindVertexArray(0);

        // ── Lámpara ───────────────────────────────────────────────────────────
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc  = glGetUniformLocation(lampShader.Program, "view");
        projLoc  = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// ─── Movimiento de cámara ─────────────────────────────────────────────────────
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD,  deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])   camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])   camera.ProcessKeyboard(LEFT,     deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])  camera.ProcessKeyboard(RIGHT,    deltaTime);

    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// ─── Teclado ──────────────────────────────────────────────────────────────────
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)        keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        Light1 = active ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(0);
    }

    // N activa/desactiva la animación completa
    if (keys[GLFW_KEY_N])
        AnimBall = !AnimBall;
}

// ─── Lógica de animación ──────────────────────────────────────────────────────
void Animation()
{
    if (!AnimBall) return;

    // Velocidad angular en grados por frame
    // La pelota avanza en +, el perro en - (sentido contrario)
    const float SPEED = 0.2f;

    rotBall += SPEED;
    rotDog  -= SPEED;

    // Mantener en [0, 360)
    if (rotBall >= 360.0f) rotBall -= 360.0f;
    if (rotDog  <    0.0f) rotDog  += 360.0f;

    // ── Detección de encuentro ─────────────────────────────────────────────
    float diff = fabs(rotBall - rotDog);
    if (diff > 180.0f) diff = 360.0f - diff;

    bool isNear = (diff < MEET_THRESHOLD);

    // Flanco de subida: dispara una sola vez por encuentro
    if (isNear && !wasNear && !dogJumping)
    {
        dogJumping      = true;
        dogJumpTimer    = 0.0f;
        ballBouncing    = true;
        ballBounceTimer = 0.0f;
    }
    wasNear = isNear;

    // ── Brinco del perro con cabezazo ─────────────────────────────────────
    // Fase 1 (t < 0.5): el perro sube y empieza a inclinar la cabeza hacia arriba.
    // Fase 2 (t >= 0.5): pico del salto → cabeza inclinada al máximo → baja.
    // La inclinación sigue una campana: 0 → HEADBUTT_ANGLE → 0,
    // con el pico centrado en t = 0.55 (ligeramente después del pico de altura)
    // para que visualmente el golpe ocurra justo cuando la cabeza llega arriba.
    if (dogJumping)
    {
        dogJumpTimer += deltaTime;
        float t = dogJumpTimer / JUMP_DURATION;  // [0, 1]

        if (t >= 1.0f)
        {
            dogJumping   = false;
            dogOffsetY   = 0.0f;
            dogHeadPitch = 0.0f;
        }
        else
        {
            // Altura: arco senoidal simétrico
            dogOffsetY = JUMP_HEIGHT * sin(glm::pi<float>() * t);

            // Cabezazo: campana centrada en t=0.55, más estrecha que el salto
            // Usamos sin elevado al cuadrado para suavizar los bordes
            float tHead = (t - 0.55f) / 0.35f;          // normalizado respecto al pico
            float headShape = expf(-tHead * tHead * 4.0f); // gaussiana: cero en bordes, 1 en pico
            dogHeadPitch = HEADBUTT_ANGLE * headShape;
        }
    }

    // ── Rebote de la pelota: sube rápido, baja lento (ease-out) ───────────
    // Se usa una curva asimétrica:
    //   subida  (t < RISE_FRACTION): seno de 0 a π/2  → 0→1  rápido
    //   bajada  (t ≥ RISE_FRACTION): seno de π/2 a π  → 1→0  pero dilatado en el tiempo
    // Esto hace que la pelota llegue arriba en ~30% del tiempo y baje en el 70% restante.
    if (ballBouncing)
    {
        ballBounceTimer += deltaTime;
        float t = ballBounceTimer / BOUNCE_DURATION;  // [0, 1]

        if (t >= 1.0f)
        {
            ballBouncing = false;
            ballOffsetY  = 0.0f;
        }
        else
        {
            const float RISE_FRACTION = 0.28f;  // fracción del tiempo dedicada a subir

            if (t < RISE_FRACTION)
            {
                // Subida rápida: mapea [0, RISE_FRACTION] → [0, π/2]
                float tRise = (t / RISE_FRACTION) * (glm::pi<float>() * 0.5f);
                ballOffsetY = BOUNCE_HEIGHT * sin(tRise);
            }
            else
            {
                // Bajada lenta con ease-out: mapea [RISE_FRACTION, 1] → [π/2, π]
                float tFall = ((t - RISE_FRACTION) / (1.0f - RISE_FRACTION))
                              * (glm::pi<float>() * 0.5f)
                              + (glm::pi<float>() * 0.5f);
                ballOffsetY = BOUNCE_HEIGHT * sin(tFall);
            }
        }
    }
}

// ─── Mouse ────────────────────────────────────────────────────────────────────
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
