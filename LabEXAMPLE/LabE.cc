#include <GL/glut.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = float(w) / float(h);
	float halfExtent = sceneExtent / 2.0f;

	if (aspect >= 1.0f)
	{
		gluOrtho2D(-halfExtent * aspect, halfExtent * aspect,
				   -halfExtent, halfExtent);
	}
	else
	{
		gluOrtho2D(-halfExtent, halfExtent,
				   -halfExtent / aspect, halfExtent / aspect);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Fibonacci");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}