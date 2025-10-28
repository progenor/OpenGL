#include <GL/glut.h>
#include <cmath>

// Rotation angles
float angleX = 25.0f;
float angleY = 30.0f;

// Function to draw a single dot on a cube face
void drawDot(float x, float y)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 20; i++)
	{
		float theta = i * 2.0f * 3.14159f / 20.0f;
		glVertex3f(x + 0.1f * cos(theta), y + 0.1f * sin(theta), 0.0f);
	}
	glEnd();
}

// Draw a die face with n dots (like on a real dobókocka)
void drawFace(int n)
{
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();

	glColor3f(0, 0, 0);
	switch (n)
	{
	case 1:
		drawDot(0, 0);
		break;
	case 2:
		drawDot(-0.5f, -0.5f);
		drawDot(0.5f, 0.5f);
		break;
	case 3:
		drawDot(-0.6f, -0.6f);
		drawDot(0, 0);
		drawDot(0.6f, 0.6f);
		break;
	case 4:
		drawDot(-0.5f, -0.5f);
		drawDot(0.5f, -0.5f);
		drawDot(0.5f, 0.5f);
		drawDot(-0.5f, 0.5f);
		break;
	case 5:
		drawDot(-0.5f, -0.5f);
		drawDot(0.5f, -0.5f);
		drawDot(0, 0);
		drawDot(0.5f, 0.5f);
		drawDot(-0.5f, 0.5f);
		break;
	case 6:
		drawDot(-0.5f, -0.6f);
		drawDot(0.5f, -0.6f);
		drawDot(-0.5f, 0);
		drawDot(0.5f, 0);
		drawDot(-0.5f, 0.6f);
		drawDot(0.5f, 0.6f);
		break;
	}
}

void drawCube()
{
	// Each face with correct orientation and number
	glPushMatrix();
	glTranslatef(0, 0, 1); // front
	drawFace(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, 1); // back
	drawFace(6);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 1); // top
	drawFace(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 1); // bottom
	drawFace(2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, 1); // right
	drawFace(3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, 0, 1); // left
	drawFace(4);
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	drawCube();

	glutSwapBuffers();
}

// Simple rotation animation
void timer(int value)
{
	angleX += 0.6f;
	angleY += 0.8f;
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // ~60 FPS
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Spinning Dobókocka");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.25f, 1.0f);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}
