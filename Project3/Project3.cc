#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

// Particle System for Explosion
struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float life;
	float maxLife;
	float size;
	glm::vec4 color; // RGBA
	bool active;
};

const int MAX_PARTICLES = 8000;
std::vector<Particle> particles(MAX_PARTICLES);

// OpenGL Shader IDs
GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;

// Particle attributes
GLuint particleVBO, particleVAO;
GLfloat particlePositions[MAX_PARTICLES * 3];
GLfloat particleSizes[MAX_PARTICLES];
GLfloat particleColors[MAX_PARTICLES * 4];

// Explosion timing
std::chrono::steady_clock::time_point explosionStart;
bool explosionActive = false;
bool spacePressed = false;

// Camera position (for explosion toward camera)
const glm::vec3 cameraPos(0.0f, 5.0f, 15.0f);

// Shader source code
const char *vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSize;
layout (location = 2) in vec4 aColor;

out vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = aSize;
    color = aColor;
}
)glsl";

const char *fragmentShaderSource = R"glsl(
#version 330 core
in vec4 color;
out vec4 FragColor;

void main() {
    // Create circular point sprite with radial gradient
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);
    
    // Smooth circular falloff
    float alpha = 1.0 - smoothstep(0.0, 0.5, dist);
    
    // Add some noise for more organic look
    alpha *= (0.8 + 0.2 * fract(sin(gl_FragCoord.x * 12.9898 + gl_FragCoord.y * 78.233) * 43758.5453));
    
    FragColor = vec4(color.rgb, color.a * alpha);
}
)glsl";

// Function to check OpenGL shader compilation errors
void checkShaderCompilation(GLuint shader, const char *type)
{
	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		printf("%s Shader Compilation Error: %s\n", type, infoLog);
	}
}

// Function to check OpenGL program linking errors
void checkProgramLinking(GLuint program)
{
	GLint success;
	GLchar infoLog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
printf("Program Linking Error: %s\n", infoLog);
	}
}

// Initialize shader program
bool initializeShaders()
{
	// Create and compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShaderCompilation(vertexShader, "Vertex");

	// Create and compile fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilation(fragmentShader, "Fragment");

	// Create and link shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkProgramLinking(shaderProgram);

	// Clean up shaders (linked into program)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

// Initialize particle system
void initializeParticles()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].active = false;
		particles[i].life = 0.0f;
		particles[i].maxLife = 0.0f;
		particles[i].size = 0.0f;
		particles[i].position = glm::vec3(0.0f);
		particles[i].velocity = glm::vec3(0.0f);
		particles[i].color = glm::vec4(0.0f);
	}
}

// Create explosion
void createExplosion()
{
	explosionStart = std::chrono::steady_clock::now();
	explosionActive = true;

	std::mt19937 rng(std::time(0));
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	std::uniform_real_distribution<float> velDist(10.0f, 50.0f);
	std::uniform_real_distribution<float> lifeDist(8.0f, 12.0f);

	// Initialize all particles
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		// Random starting position (center of scene)
		float angle1 = dist(rng) * 2.0f * M_PI;
		float angle2 = dist(rng) * 2.0f * M_PI;
		float radius = dist(rng) * 0.5f; // Small initial radius

		particles[i].position = glm::vec3(
			radius * cos(angle1) * cos(angle2),
			radius * sin(angle2) * 0.5f, // Slightly flattened vertically
			radius * sin(angle1) * cos(angle2));

		// Velocity pointing toward camera with radial explosion
		glm::vec3 toCamera = glm::normalize(cameraPos - particles[i].position);
		glm::vec3 randomDir = glm::vec3(
			dist(rng) - 0.5f,
			dist(rng) - 0.5f,
			dist(rng) - 0.5f);

		// Mix radial explosion with camera direction (80% toward camera, 20% radial)
		particles[i].velocity = (toCamera * 0.8f + randomDir * 0.2f) * velDist(rng);

		particles[i].life = 0.0f;
		particles[i].maxLife = lifeDist(rng);
		particles[i].size = dist(rng) * 10.0f + 5.0f; // Random size between 5-15
		particles[i].active = true;

		// Initial bright color (white/yellow)
		particles[i].color = glm::vec4(1.0f, 0.9f, 0.6f, 1.0f);
	}
}

// Get explosion color based on progress (0.0 = start, 1.0 = end)
glm::vec4 getExplosionColor(float progress)
{
	if (progress < 0.2f)
	{
		// Bright white/yellow (initial blast)
		return glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
	}
	else if (progress < 0.4f)
	{
		// Yellow/orange (expanding)
		return glm::vec4(1.0f, 0.8f, 0.3f, 0.9f);
	}
	else if (progress < 0.7f)
	{
		// Orange/red (mature explosion)
		return glm::vec4(0.9f, 0.4f, 0.1f, 0.7f);
	}
	else if (progress < 0.9f)
	{
		// Red/dark (cooling down)
		return glm::vec4(0.6f, 0.2f, 0.1f, 0.5f);
	}
	else
	{
		// Gray/ash (final stage)
		return glm::vec4(0.3f, 0.3f, 0.3f, 0.3f);
	}
}

// Update particle physics
void updateParticles(float deltaTime)
{
	if (!explosionActive)
		return;

	auto now = std::chrono::steady_clock::now();
	float elapsed = std::chrono::duration<float>(now - explosionStart).count();

	// End explosion after 10 seconds
	if (elapsed >= 10.0f)
	{
		exit(0);
	}

	float gravity = -9.8f * 0.5f; // Reduced gravity for slower effect

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (!particles[i].active)
			continue;

		particles[i].life += deltaTime;

		if (particles[i].life > particles[i].maxLife)
		{
			particles[i].active = false;
			continue;
		}

		// Update physics
		particles[i].velocity.y += gravity * deltaTime; // Apply gravity

		// Apply drag (air resistance)
		particles[i].velocity *= 0.995f;

		// Update position
		particles[i].position += particles[i].velocity * deltaTime;

		// Update color based on life progress
		float progress = particles[i].life / particles[i].maxLife;
		particles[i].color = getExplosionColor(progress);

		// Fade alpha over time
		particles[i].color.a = (1.0f - progress) * 0.8f;

		// Update size (expand then shrink)
		float sizeProgress = sin(progress * M_PI * 2.0f) * 0.5f + 0.5f;
		particles[i].size = (sizeProgress * 15.0f + 5.0f) * (1.0f - progress * 0.3f);
	}
}

// Update GPU buffers with particle data
void updateParticleBuffers()
{
	int activeCount = 0;

	for (int i = 0; i < MAX_PARTICLES && activeCount < MAX_PARTICLES; i++)
	{
		if (particles[i].active)
		{
			int idx = activeCount * 3;
			int cidx = activeCount * 4;

			// Position
			particlePositions[idx] = particles[i].position.x;
			particlePositions[idx + 1] = particles[i].position.y;
			particlePositions[idx + 2] = particles[i].position.z;

			// Size
			particleSizes[activeCount] = particles[i].size;

			// Color
			particleColors[cidx] = particles[i].color.r;
			particleColors[cidx + 1] = particles[i].color.g;
			particleColors[cidx + 2] = particles[i].color.b;
			particleColors[cidx + 3] = particles[i].color.a;

			activeCount++;
		}
	}

	// Update VBO with new data
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, activeCount * 3 * sizeof(GLfloat), particlePositions);
	glBufferSubData(GL_ARRAY_BUFFER, activeCount * 3 * sizeof(GLfloat), activeCount * sizeof(GLfloat), particleSizes);
	glBufferSubData(GL_ARRAY_BUFFER, activeCount * 3 * sizeof(GLfloat) + activeCount * sizeof(GLfloat), activeCount * 4 * sizeof(GLfloat), particleColors);
}

// Display function
void display()
{
	static auto lastTime = std::chrono::steady_clock::now();
	auto currentTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set camera position
	gluLookAt(0.0f, 5.0f, 15.0f, // eye position
			  0.0f, 0.0f, 0.0f,	 // look at position
			  0.0f, 1.0f, 0.0f); // up vector

	// Update particles if explosion is active
	if (explosionActive)
	{
		updateParticles(deltaTime);
		updateParticleBuffers();
	}

	// Use shader program
	glUseProgram(shaderProgram);

	// Set up matrices
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	// Render particles if explosion is active
	if (explosionActive)
	{
		glBindVertexArray(particleVAO);
		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
		glBindVertexArray(0);
	}

	// Instructions on screen
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	if (!spacePressed)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(-0.5f, 0.0f);

		const char *text = "Press SPACEBAR to trigger explosion";
		while (*text)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
			text++;
		}
	}

	glUseProgram(0);
	glutSwapBuffers();

	// Slow down for slow-motion effect
}

// Keyboard function
void keyboard(unsigned char key, int x, int y)
{
	if (key == ' ' && !spacePressed)
	{
		spacePressed = true;
		createExplosion();
	}
}

// Reshape function
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = float(w) / float(h);
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for explosion effect
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Shader Particle Explosion");

	// Initialize GLEW
	glewInit();

	// Initialize shaders
	if (!initializeShaders())
	{
		printf("Failed to initialize shaders\n");
		return -1;
	}

	// Initialize particle system
	initializeParticles();

	// Set up particle VAO/VBO
	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);

	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// Allocate buffer space
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * (3 + 1 + 4) * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	// Position attribute (location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 1 + 4) * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	// Size attribute (location 1)
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, (3 + 1 + 4) * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Color attribute (location 2)
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (3 + 1 + 4) * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Set GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
glutIdleFunc(display); // Enable continuous animation loop

	// Start main loop
	glutMainLoop();
	return 0;
}
