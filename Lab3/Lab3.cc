#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

// Draws a clock hand with given length, width and angle
void drawHand(float length, float width, float angle)
{
	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-width, 0.0f);
	glVertex2f(width, 0.0f);
	glVertex2f(width, -length);
	glVertex2f(-width, -length);
	glEnd();
	glPopMatrix();
}

// Draws the clock face with tick marks and numbers
void drawClockFace(float radius)
{
	// Draw clock face (circle)
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float angle = i * D2R;
		glVertex2f(radius * cos(angle), radius * sin(angle));
	}
	glEnd();

	// Draw border
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float angle = i * D2R;
		glVertex2f(radius * cos(angle), radius * sin(angle));
	}
	glEnd();

	// Draw hour tick marks and numbers
	for (int i = 0; i < 12; i++)
	{
		float angle = i * 30 * D2R;
		float x = 0.85f * radius * sin(angle);
		float y = 0.85f * radius * cos(angle);

		// Draw tick mark
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glRotatef(-i * 30, 0.0f, 0.0f, 1.0f);

		glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.15f * radius);
		glEnd();

		glPopMatrix();

		// Draw number
		glRasterPos2f(0.75f * radius * sin(angle), 0.75f * radius * cos(angle));
		std::string number = std::to_string((i == 0) ? 12 : i);
		for (char c : number)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
		}
	}

	// Draw minute tick marks
	for (int i = 0; i < 60; i++)
	{
		if (i % 5 != 0)
		{ // Skip where hour ticks are
			float angle = i * 6 * D2R;
			float x = 0.9f * radius * sin(angle);
			float y = 0.9f * radius * cos(angle);

			glPushMatrix();
			glTranslatef(x, y, 0.0f);
			glRotatef(-i * 6, 0.0f, 0.0f, 1.0f);

			glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.05f * radius);
			glEnd();

			glPopMatrix();
		}
	}
}

// Draws the digital clock and date display
void drawDigitalDisplay(float y_position)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	// Format date: Tue Sep 30
	std::string weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
							"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	std::stringstream dateStr;
	dateStr << weekdays[ltm->tm_wday] << " "
			<< months[ltm->tm_mon] << " "
			<< ltm->tm_mday;

	// Format time: HH:MM:SS (24-hour format)
	std::stringstream timeStr;
	timeStr << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":"
			<< std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
			<< std::setfill('0') << std::setw(2) << ltm->tm_sec;

	// Draw date
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(-5.0f, y_position);
	for (char c : dateStr.str())
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	// Draw time
	glRasterPos2f(-3.0f, y_position - 2.0f);
	for (char c : timeStr.str())
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Get current time
	time_t now = time(0);
	tm *ltm = localtime(&now);

	// Calculate hand angles
	float hour_angle = 90.0f - ((ltm->tm_hour % 12) * 30.0f + ltm->tm_min * 0.5f);
	float min_angle = 90.0f - (ltm->tm_min * 6.0f + ltm->tm_sec * 0.1f);
	float sec_angle = 90.0f - (ltm->tm_sec * 6.0f);

	// Draw clock face
	float clockRadius = 8.0f;
	drawClockFace(clockRadius);

	// Draw center dot
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();

	// Draw hour hand
	glColor3f(0.0f, 0.0f, 0.7f);
	drawHand(clockRadius * 0.5f, 0.15f, hour_angle);

	// Draw minute hand
	glColor3f(0.0f, 0.5f, 0.0f);
	drawHand(clockRadius * 0.7f, 0.1f, min_angle);

	// Draw second hand
	glColor3f(1.0f, 0.0f, 0.0f);
	drawHand(clockRadius * 0.8f, 0.05f, sec_angle);

	// Draw digital display
	drawDigitalDisplay(-12.0f);

	glutSwapBuffers();
	glutPostRedisplay(); // Request continuous updates
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