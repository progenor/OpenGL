#include <GL/glut.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

GLfloat vertices[8][3] = {
	{-0.5f, -0.5f, -0.5f},
	{0.5f, -0.5f, -0.5f},
	{0.5f, 0.5f, -0.5f},
	{-0.5f, 0.5f, -0.5f},
	{-0.5f, -0.5f, 0.5f},
	{0.5f, -0.5f, 0.5f},
	{0.5f, 0.5f, 0.5f},
	{-0.5f, 0.5f, 0.5f}};

// Each face is split into 2 triangles, 12 triangles total
int faces[12][3] = {
	// Front face (z = 0.5)
	{4, 5, 6},
	{4, 6, 7},
	// Back face (z = -0.5)
	{0, 3, 2},
	{0, 2, 1},
	// Left face (x = -0.5)
	{0, 4, 7},
	{0, 7, 3},
	// Right face (x = 0.5)
	{1, 2, 6},
	{1, 6, 5},
	// Top face (y = 0.5)
	{3, 7, 6},
	{3, 6, 2},
	// Bottom face (y = -0.5)
	{0, 1, 5},
	{0, 5, 4}};

// 12 different colors for triangles
GLfloat colors[12][3] = {
	{1, 0, 0}, {1, 0.5, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {0.5, 0, 1}, {1, 1, 0}, {1, 0, 1}, {0, 1, 0.5}, {0, 0.5, 1}, {0.5, 1, 0}, {1, 0, 0.5}};

float angle = 0.0f; // Rotation angle

void drawCubeTriangles()
{
	glPushMatrix();
	glScalef(8.0f, 8.0f, 8.0f);			// Scale cube to fit nicely in view
	glRotatef(angle, 1.0f, 1.0f, 0.0f); // Rotate cube

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 12; ++i)
	{
		glColor3fv(colors[i]);
		for (int j = 0; j < 3; ++j)
		{
			int v = faces[i][j];
			glVertex3fv(vertices[v]);
		}
	}
	glEnd();
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -20); // Move cube into view
	drawCubeTriangles();
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = float(w) / float(h);
	gluPerspective(45.0, aspect, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		angle += 30.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;
		glutPostRedisplay();
	}
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Triangulated Rotating Cube");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}