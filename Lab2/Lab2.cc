#include <GL/glut.h>
#include <cmath>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

void drawSquare()
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
}

void drawArc()
{
	glBegin(GL_LINE_STRIP);
	for (int i = 90; i <= 180; ++i)
	{
		glVertex2f(0.5 + cos(i * D2R), -0.5 + sin(i * D2R));
	}
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	float size = 8.0f;
	float phi = 1.618f;
	for (int i = 0; i < 8; ++i)
	{
		glPushMatrix();

		glScalef(size, size, 0);

		glColor3f(0, 1, 0);
		drawSquare();

		glColor3f(1, 0, 0);
		drawArc();
		glPopMatrix();
		float nextSize = size / phi;
		glTranslatef((size + nextSize) / 2, (size - nextSize) / 2, 0);
		glRotatef(-90, 0, 0, 1);
		size = nextSize;
	}

	glutSwapBuffers();
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