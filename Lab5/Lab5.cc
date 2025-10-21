#include <GL/glut.h>
#include <cmath>

static GLfloat D2R = 3.141529f / 180.0f;
float sceneExtent = 20.0f;

struct Joint
{
	float x, y;
};

Joint head = {0.0f, 7.0f};
Joint neck = {0.0f, 5.0f};
Joint bodyTop = {0.0f, 5.0f};
Joint bodyBottom = {0.0f, 0.0f};
Joint leftShoulder = {-2.0f, 5.0f};
Joint rightShoulder = {2.0f, 5.0f};
Joint leftHand = {-4.0f, 2.0f};
Joint rightHand = {4.0f, 2.0f};
Joint leftHip = {-1.0f, 0.0f};
Joint rightHip = {1.0f, 0.0f};
Joint leftKnee = {-1.5f, -2.5f};
Joint rightKnee = {1.5f, -2.5f};
Joint leftFoot = {-2.0f, -5.0f};
Joint rightFoot = {2.0f, -5.0f};

bool animate = false;
float szog = 0.0;
int elojel = 1;
float t = 0.0f;

float parentLen = 3.0f, childLen = 3.0f;

void drawCircle(float cx, float cy, float r, int num_segments = 32)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; ++i)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}

void computeLeg(const Joint &hip, Joint &knee, Joint &foot, float angle)
{
	float targetX = hip.x + (parentLen + childLen - 0.1f) * sinf(angle * D2R);
	float targetY = hip.y - (parentLen + childLen - 0.1f) * cosf(angle * D2R);

	float posX = targetX - hip.x;
	float posY = targetY - hip.y;

	float d2 = posX * posX + posY * posY;
	float d = sqrtf(d2);

	d = fminf(d, parentLen + childLen - 0.001f);

	float theta2 = acosf((d2 - parentLen * parentLen - childLen * childLen) / (2 * parentLen * childLen));
	float theta1 = atan2f(posY * (parentLen + childLen * cosf(theta2)) - posX * (childLen * sinf(theta2)),
						  posX * (parentLen + childLen * cosf(theta2)) + posY * (childLen * sinf(theta2)));

	knee.x = hip.x + parentLen * cosf(theta1);
	knee.y = hip.y + parentLen * sinf(theta1);

	foot.x = knee.x + childLen * cosf(theta1 + theta2);
	foot.y = knee.y + childLen * sinf(theta1 + theta2);
}

void Timer(int value)
{
	t += 0.05f;
	szog += elojel * 0.5;
	if (szog > 30)
		elojel = -elojel;
	if (szog < -30)
		elojel = -elojel;

	head.x = 0.5f * sinf(t);
	head.y = 7.0f + 0.3f * cosf(t);

	neck.x = 0.3f * sinf(t + 0.5f);
	neck.y = 5.0f + 0.1f * cosf(t + 0.5f);

	bodyTop.x = 0.2f * sinf(t + 1.0f);
	bodyTop.y = 5.0f + 0.1f * cosf(t + 1.0f);

	bodyBottom.x = 0.2f * sinf(t + 1.5f);
	bodyBottom.y = 0.0f + 0.1f * cosf(t + 1.5f);

	leftShoulder.x = -2.0f + 0.2f * sinf(t + 2.0f);
	leftShoulder.y = 5.0f + 0.1f * cosf(t + 2.0f);

	rightShoulder.x = 2.0f + 0.2f * sinf(t + 2.5f);
	rightShoulder.y = 5.0f + 0.1f * cosf(t + 2.5f);

	leftHand.x = leftShoulder.x - 2.0f * cosf(t);
	leftHand.y = leftShoulder.y - 3.0f * fabs(sinf(t));

	rightHand.x = rightShoulder.x + 2.0f * cosf(t + 1.0f);
	rightHand.y = rightShoulder.y - 3.0f * fabs(sinf(t + 1.0f));

	leftHip.x = -1.0f + 0.2f * sinf(t + 3.0f);
	leftHip.y = 0.0f + 0.1f * cosf(t + 3.0f);

	rightHip.x = 1.0f + 0.2f * sinf(t + 3.5f);
	rightHip.y = 0.0f + 0.1f * cosf(t + 3.5f);

	computeLeg(leftHip, leftKnee, leftFoot, szog + 10.0f * sinf(t));
	computeLeg(rightHip, rightKnee, rightFoot, -szog + 10.0f * cosf(t));

	if (animate)
		glutTimerFunc(30, Timer, value);
	glutPostRedisplay();
}

void drawStickFigure()
{
	drawCircle(head.x, head.y, 2.0f);
	glBegin(GL_LINES);
	glVertex2f(head.x, head.y);
	glVertex2f(neck.x, neck.y);
	glVertex2f(neck.x, neck.y);
	glVertex2f(bodyTop.x, bodyTop.y);
	glVertex2f(bodyTop.x, bodyTop.y);
	glVertex2f(bodyBottom.x, bodyBottom.y);
	glVertex2f(leftShoulder.x, leftShoulder.y);
	glVertex2f(leftHand.x, leftHand.y);
	glVertex2f(rightShoulder.x, rightShoulder.y);
	glVertex2f(rightHand.x, rightHand.y);
	glVertex2f(leftHip.x, leftHip.y);
	glVertex2f(leftKnee.x, leftKnee.y);
	glVertex2f(leftKnee.x, leftKnee.y);
	glVertex2f(leftFoot.x, leftFoot.y);
	glVertex2f(rightHip.x, rightHip.y);
	glVertex2f(rightKnee.x, rightKnee.y);
	glVertex2f(rightKnee.x, rightKnee.y);
	glVertex2f(rightFoot.x, rightFoot.y);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	drawStickFigure();
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

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		animate = !animate;
		if (animate)
			glutTimerFunc(30, Timer, 0);
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Stick Figure IK");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
	return 0;
}