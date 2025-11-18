#include <GL/glut.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set camera position
	gluLookAt(0.0f, 5.0f, 15.0f, // eye position
			  0.0f, 0.0f, 0.0f,	 // look at position
			  0.0f, 1.0f, 0.0f); // up vector

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set light position (sun)
	GLfloat lightPos[] = {10.0f, 10.0f, -10.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Set light properties
	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat diffuseLight[] = {1.0f, 1.0f, 0.8f, 1.0f};
	GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Draw sun
	glPushMatrix();
	glTranslatef(10.0f, 10.0f, -10.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();

	// Draw clouds
	glDisable(GL_LIGHTING); // Clouds don't need lighting
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-5.0f, 8.0f, -15.0f);
	glutSolidSphere(1.5f, 20, 20);
	glTranslatef(2.0f, 0.0f, 0.0f);
	glutSolidSphere(1.2f, 20, 20);
	glTranslatef(2.0f, 0.5f, 0.0f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0f, 9.0f, -12.0f);
	glutSolidSphere(1.3f, 20, 20);
	glTranslatef(1.5f, 0.0f, 0.0f);
	glutSolidSphere(1.1f, 20, 20);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	// Create quadric for cylinders
	GLUquadric *quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);

	// Draw ground
	glColor3f(0.0f, 0.8f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-20.0f, 0.0f, -20.0f);
	glVertex3f(20.0f, 0.0f, -20.0f);
	glVertex3f(20.0f, 0.0f, 20.0f);
	glVertex3f(-20.0f, 0.0f, 20.0f);
	glEnd();

	// Draw house
	glColor3f(0.8f, 0.6f, 0.4f); // Brown walls
	glPushMatrix();
	glTranslatef(0.0f, 2.5f, 0.0f);
	glScalef(4.0f, 5.0f, 4.0f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Roof
	glColor3f(0.6f, 0.3f, 0.1f);
	glPushMatrix();
	glTranslatef(0.0f, 6.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(3.5f, 3.0f, 20, 20);
	glPopMatrix();

	// Door
	glColor3f(0.4f, 0.2f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, 1.5f, 2.01f);
	glScalef(0.8f, 3.0f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Windows
	glColor3f(0.7f, 0.9f, 1.0f);
	glPushMatrix();
	glTranslatef(-1.5f, 3.0f, 2.01f);
	glScalef(0.8f, 1.0f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 3.0f, 2.01f);
	glScalef(0.8f, 1.0f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Draw trees
	glColor3f(0.4f, 0.2f, 0.0f); // Brown trunk
	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, -8.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, 0.3f, 0.3f, 4.0f, 20, 20);
	glPopMatrix();

	glColor3f(0.0f, 0.6f, 0.0f); // Green leaves
	glPushMatrix();
	glTranslatef(-8.0f, 5.0f, -8.0f);
	glutSolidSphere(1.5f, 20, 20);
	glPopMatrix();

	// Another tree
	glColor3f(0.4f, 0.2f, 0.0f);
	glPushMatrix();
	glTranslatef(8.0f, 0.0f, -8.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, 0.3f, 0.3f, 4.0f, 20, 20);
	glPopMatrix();

	glColor3f(0.0f, 0.6f, 0.0f);
	glPushMatrix();
	glTranslatef(8.0f, 5.0f, -8.0f);
	glutSolidSphere(1.5f, 20, 20);
	glPopMatrix();

	// Draw flowers
	glColor3f(1.0f, 0.0f, 0.0f); // Red flower
	glPushMatrix();
	glTranslatef(-3.0f, 0.0f, 5.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, 0.05f, 0.05f, 1.0f, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0f, 1.0f, 5.0f);
	glutSolidSphere(0.2f, 10, 10);
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 0.0f); // Yellow flower
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, 4.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, 0.05f, 0.05f, 1.0f, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0f, 1.0f, 4.0f);
	glutSolidSphere(0.2f, 10, 10);
	glPopMatrix();

	glColor3f(0.8f, 0.0f, 0.8f); // Purple flower
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 6.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quad, 0.05f, 0.05f, 1.0f, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 6.0f);
	glutSolidSphere(0.2f, 10, 10);
	glPopMatrix();

	gluDeleteQuadric(quad);

	glutSwapBuffers();
}

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
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Garden Scene");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
