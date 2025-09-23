/*
   Rajzoljunk:
	  -egy piros pontot a (10, 15),
	  -egy zöld pontot az (50, 10) és
	  -egy kék pontot a (-30, -10) koordinátákra!

   A színtér forogjon!

   (Az ablak legyen 700*600-as méretû.)
*/

#include <GL/glut.h>
#include <cmath>

static GLfloat spin = 0.0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // a törlõszín a fekete
	glShadeModel(GL_FLAT);
}

static int sizes[] = {1, 1, 2, 3, 5, 8, 13};
static GLfloat D2R = 3.14159 / 180.0;

// ...existing code...
void drawSmallBOx(float scale = 1.0f)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i <= 90; i++)
	{
		glVertex2f((-0.5 + cos(i * D2R)) * scale, (-.05 + sin(i * D2R)) * scale);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0); // red color for the box
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.6 * scale, -0.15 * scale); // Bottom left
	glVertex2f(0.6 * scale, -0.15 * scale);	 // Bottom right
	glVertex2f(0.6 * scale, 1.05 * scale);	 // Top right
	glVertex2f(-0.6 * scale, 1.05 * scale);	 // Top left
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glEnable(GL_POINT_SMOOTH);

	// first box (normal size)
	drawSmallBOx(1.0f);

	// second box (scaled up, e.g. golden ratio)
	glPushMatrix();
	glTranslatef(-0.9, 0.5, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawSmallBOx(1.0f);
	glPopMatrix();

	// third box (scaled down, for example)
	glPushMatrix();
	glTranslatef(0.9, 0.5, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawSmallBOx(1.68f);
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}
// ...existing code...
void keyboard(unsigned char key, int x, int y) // billentûkezelés
{
	switch (key)
	{
	case 27:	 // ha escape-et nyomtam
		exit(0); // lépjen ki a programból
		break;
	}
}

void spinDisplay(void)
{
	spin = spin + 0.5;
	if (spin > 360)
		spin = spin - 360;
	glutPostRedisplay(); // újrarajzolás
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		if (state == GLUT_UP)
			glutIdleFunc(nullptr);
		break;
	default:
		break;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-20, 20, -20 * (GLfloat)h / (GLfloat)w,
				20 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
	else
		glOrtho(-20 * (GLfloat)w / (GLfloat)h,
				20 * (GLfloat)w / (GLfloat)h, -20, 20, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);						 // inicializalja a glut lib-et
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // az ablak kétszeresen pufferelt,és RGB módú
	glutInitWindowSize(700, 600);				 // az ablak 700x600-as
	glutInitWindowPosition(100, 100);			 // az ablak bal felsõ sarkának koordinátája
	glutCreateWindow("Pontok");					 // neve Pontok
	init();										 // inicializálás
	glutDisplayFunc(display);					 // a képernyõ események kezelése
	glutKeyboardFunc(keyboard);					 // billentyûzet események kezelése
	glutReshapeFunc(reshape);					 // újrarajzolás nagyításkor, kicsinyítéskor
	glutMouseFunc(mouse);						 // egérkezelés
	glutMainLoop();								 // belépés az esemény hurokba...
	return 0;
}