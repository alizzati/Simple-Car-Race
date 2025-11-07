#include <glut.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;

// windows
int wind_w = 1000, wind_h = 480;
const float wind_kiri = 0.0f, wind_kanan = 160.0f, wind_bawah = 0.0f, wind_atas = 50.0f;

const float jalan_kiri = 0.0f;              // biar nempel ke kiri
const float jalan_kanan = wind_kanan;      // biar pas ke kanan
const float jalan_bawah = wind_bawah;    // biar nempel ke bawah
const float jalan_atas = jalan_bawah + 25.0f; // tinggi jalan 
const float garisFinish = jalan_kanan - 3.0f;   // finish
bool pasMenang = false;
int winner = 0;
int frameCount = 0;

struct Car {
	float x, y;
	float w, h;
	float vx, vy;
	float accel;
	float friction;
	float maxSpeed;
	float r, g, b;
	float laneMin, laneMax;
	string name;
};
Car car1, car2;

bool keyStates[256];
bool specialKeyStates[512];

//  UTILITAS
float clampf(float v, float a, float b) { 
	return v < a ? a : (v > b ? b : v); 
}
void drawRect(float cx, float cy, float w, float h) {
	glBegin(GL_QUADS);
	glVertex2f(cx - w / 2, cy - h / 2);
	glVertex2f(cx + w / 2, cy - h / 2);
	glVertex2f(cx + w / 2, cy + h / 2);
	glVertex2f(cx - w / 2, cy + h / 2);
	glEnd();
}
void drawText(float x, float y, const string& s) {
	glRasterPos2f(x, y);
	for (char c : s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void resetGame() {
	car1 = { 15, jalan_atas - 3.0f, 4.5f, 2.2f, 0,0, 0.025f, 0.02f, 0.3f,
	0.9f,0.2f,0.2f, jalan_bawah + 13.0f, jalan_atas, "Mobil 1" };
	car2 = { 15, jalan_bawah + 8.0f, 4.5f, 2.2f, 0,0, 0.025f, 0.02f, 0.3f,
		0.18f,0.6f,0.95f, jalan_bawah, jalan_atas - 13.0f, "Mobil 2" };
	pasMenang = false;
	winner = 0;
	frameCount = 0;
	memset(keyStates, 0, sizeof(keyStates));
	memset(specialKeyStates, 0, sizeof(specialKeyStates));
}

// Background
void drawMoon() {
	float cx = 140.0f;   // x bulan
	float cy = 42.0f;    // y bulan
	float rx = 6.0f;
	float ry = 4.0f;

	glPushMatrix();
	glColor3f(0.98f, 0.96f, 0.82f); // warna kekuningan
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy); // titik pusat bulan
	for (int i = 0; i <= 120; i++) {
		float a = 2.0f * 3.1415f * i / 120.0f;
		glVertex2f(cx + cos(a) * rx, cy + sin(a) * ry);
	}
	glEnd();
	glPopMatrix();
}

void drawBuildings() {
	srand(123);
	float x = wind_kiri + 2;
	while (x < wind_kanan - 2) {
		float bw = 8 + (rand() % 20) / 10.0f;
		float bh = 10 + (rand() % 40) / 4.0f;
		float baseY = jalan_atas + 2.0f;
		glColor3f(0.06f, 0.06f, 0.09f);
		glBegin(GL_QUADS);
		glVertex2f(x, baseY);
		glVertex2f(x + bw, baseY);
		glVertex2f(x + bw, baseY + bh);
		glVertex2f(x, baseY + bh);
		glEnd();
		int cols = (int)(bw / 0.8f), rows = (int)(bh / 1.2f);
		for (int c = 0; c < cols; c++) {
			for (int r = 0; r < rows; r++) {
				float wx = x + 0.4f + c * (bw / cols);
				float wy = baseY + 0.5f + r * (bh / rows);
				bool on = ((frameCount / 20 + c + r) % 6) < 3;
				if (on) glColor3f(1.0f, 0.9f, 0.5f);
				else glColor3f(0.08f, 0.08f, 0.08f);
				drawRect(wx, wy, 0.4f, 0.5f);
			}
		}
		x += bw + 4;
	}
}

void drawStreetLights() {
	float x = jalan_kiri + 10.0f;
	while (x < jalan_kanan - 10.0f) {
		glColor3f(0.4f, 0.4f, 0.45f);
		drawRect(x, jalan_atas + 3.0f, 0.4f, 6.0f);
		glColor3f(0.95f, 0.95f, 0.7f);
		drawRect(x, jalan_atas + 6.0f, 1.2f, 0.6f);

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(1.0f, 0.95f, 0.65f, 0.25f);
		glVertex2f(x, jalan_atas + 6.0f);
		glColor4f(1.0f, 0.95f, 0.65f, 0.0f);
		for (int i = 0; i <= 40; i++) {
			float a = 3.1415f + (3.1415f * i / 40);
			glVertex2f(x + 5.5f * cos(a), jalan_atas + 6.0f + 4.0f * sin(a));
		}
		glEnd();

		x += 16.0f;
	}
}

void drawStreet() {
	glColor3f(0.08f, 0.08f, 0.1f); // warna jalan
	drawRect((jalan_kiri + jalan_kanan) / 2, (jalan_bawah + jalan_atas) / 2,
		jalan_kanan - jalan_kiri, jalan_atas - jalan_bawah);

	glColor3f(0.95f, 0.9f, 0.2f); // garis kuningn putus putus
	float x = jalan_kiri + 3;
	while (x < jalan_kanan - 3) {
		drawRect(x, (jalan_bawah + jalan_atas) / 2, 2.4f, 0.18f);
		x += 6;
	}

	glColor3f(1, 1, 1); // garis finish
	drawRect(garisFinish, (jalan_bawah + jalan_atas) / 2, 1.0f, jalan_atas - jalan_bawah + 3);
}

void drawBackground() {
	glBegin(GL_QUADS);
	glColor3f(0.03f, 0.05f, 0.1f); glVertex2f(wind_kiri, wind_atas);
	glColor3f(0.02f, 0.03f, 0.08f); glVertex2f(wind_kanan, wind_atas);
	glColor3f(0.01f, 0.01f, 0.03f); glVertex2f(wind_kanan, jalan_atas + 3);
	glColor3f(0.02f, 0.02f, 0.05f); glVertex2f(wind_kiri, jalan_atas + 3);
	glEnd();
	drawMoon();
	drawBuildings();
	drawStreetLights();
	drawStreet();
}

void drawCar(const Car& c) {
	glPushMatrix();
	glTranslatef(c.x, c.y, 0);

	glColor3f(c.r, c.g, c.b);
	glBegin(GL_QUADS);
	glVertex2f(-c.w * 0.9f, -c.h * 0.3f);
	glVertex2f(c.w * 0.9f, -c.h * 0.3f);
	glVertex2f(c.w * 0.9f, c.h * 0.3f);
	glVertex2f(-c.w * 0.9f, c.h * 0.3f);
	glEnd();

	glColor3f(0.7f, 0.9f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(c.w * 0.5f, c.h * 0.3f);
	glVertex2f(-c.w * 0.7f, c.h * 0.3f);
	glVertex2f(-c.w * 0.5f, c.h * 0.7f);
	glVertex2f(c.w * 0.2f, c.h * 0.7f);
	glEnd();

	glColor3f(0.05f, 0.05f, 0.05f);
	float rw = c.w * 0.25f, rh = c.h * 0.25f;
	for (int i = 0; i < 2; i++) {
		float offsetX = (i == 0) ? -c.w * 0.55f : c.w * 0.55f;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(offsetX, -c.h * 0.35f);
		for (int j = 0; j <= 30; j++) {
			float a = j * 2 * 3.1415f / 30;
			glVertex2f(offsetX + cos(a) * rw, -c.h * 0.35f + sin(a) * rh);
		}
		glEnd();
		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(offsetX, -c.h * 0.35f);
		for (int j = 0; j <= 20; j++) {
			float a = j * 2 * 3.1415f / 20;
			glVertex2f(offsetX + cos(a) * rw * 0.4f, -c.h * 0.35f + sin(a) * rh * 0.4f);
		}
		glEnd();
		glColor3f(0.05f, 0.05f, 0.05f);
	}

	// Lampu depan
	glColor3f(1.0f, 1.0f, 0.6f);
	drawRect(c.w * 0.95f, 0.0f, 0.3f, 0.4f);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 1.0f, 0.8f, 0.3f);
	glVertex2f(c.w * 0.95f, 0.0f);
	glColor4f(1.0f, 1.0f, 0.8f, 0.0f);
	glVertex2f(c.w * 2.2f, c.h * 0.8f);
	glVertex2f(c.w * 2.2f, -c.h * 0.8f);
	glEnd();

	// Lampu belakang
	glColor3f(1.0f, 0.2f, 0.2f);
	drawRect(-c.w * 0.95f, 0.0f, 0.25f, 0.4f);
	glPopMatrix();
}

// Fisika-nya
void applyPhysics(Car& c) {
	// gerakan friksi
	if (fabs(c.vx) > c.friction)
		c.vx -= c.friction * (c.vx / fabs(c.vx));
	else
		c.vx = 0;

	if (fabs(c.vy) > c.friction)
		c.vy -= c.friction * (c.vy / fabs(c.vy));
	else
		c.vy = 0;

	// Kecepatan Max
	c.vx = clampf(c.vx, -c.maxSpeed, c.maxSpeed);
	c.vy = clampf(c.vy, -c.maxSpeed, c.maxSpeed);

	// Update posisi mobil 
	c.x += c.vx;
	c.y += c.vy;

	// Batas mobil tidak keluar windows
	float hw = c.w / 2.0f;   // setengah lebar mobil
	float hh = c.h / 2.0f;   // setengah tinggi mobil

	// Margin tambahan agar lampu belakang tetap di dalam batas
	float backMargin = c.w * 1.0f;
	float frontMargin = c.w * 1.0f;

	// Batas posisi horizontal (kiri-kanan)
	c.x = clampf(c.x, jalan_kiri + backMargin, jalan_kanan - frontMargin);

	// Batas posisi vertikal (atas-bawah)
	c.y = clampf(c.y, c.laneMin + hh, c.laneMax - hh);
}

void processInput() {
	if (pasMenang) return;
	if (specialKeyStates[GLUT_KEY_UP]) car1.vy += car1.accel;
	if (specialKeyStates[GLUT_KEY_DOWN]) car1.vy -= car1.accel;
	if (specialKeyStates[GLUT_KEY_RIGHT]) car1.vx += car1.accel;
	if (specialKeyStates[GLUT_KEY_LEFT]) car1.vx -= car1.accel;
	if (keyStates['w'] || keyStates['W']) car2.vy += car2.accel;
	if (keyStates['s'] || keyStates['S']) car2.vy -= car2.accel;
	if (keyStates['d'] || keyStates['D']) car2.vx += car2.accel;
	if (keyStates['a'] || keyStates['A']) car2.vx -= car2.accel;
}

void checkWinner() {
	if (pasMenang) return;

	// Lampu depan digambar pada posisi x + (lebar mobil * 0.95f)
	float frontLightOffset = car1.w * 0.95f;

	if (car1.x + frontLightOffset >= garisFinish) {
		pasMenang = true;
		winner = 1;
	}
	if (car2.x + frontLightOffset >= garisFinish) {
		pasMenang = true;
		winner = 2;
	}
}

// CALLBACK 
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawBackground();
	drawCar(car1);
	drawCar(car2);
	if (pasMenang) {
		glColor4f(0, 0, 0, 0.7f);
		drawRect((wind_kiri + wind_kanan) / 2, (wind_bawah + wind_atas) / 2,
			wind_kanan - wind_kiri, wind_atas - wind_bawah);
		glColor3f(1, 1, 1);
		string msg = (winner == 1) ? "Winner goes to Mobil 1" : "Winner goes to Mobil 2";
		drawText(72, 32, msg);
		drawText(70, 28, "Press R to Restart");
	}
	glutSwapBuffers();
}
void idleFunc() {
	processInput();
	applyPhysics(car1);
	applyPhysics(car2);
	checkWinner();
	//frameCount+=1;	
	glutPostRedisplay();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(wind_kiri, wind_kanan, wind_bawah, wind_atas);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void keyboardDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
	if ((key == 'r' || key == 'R') && pasMenang) resetGame();
	if (key == 27) exit(0);
}

void keyboardUp(unsigned char key, int x, int y) { keyStates[key] = false; }
void specialDown(int key, int x, int y) { specialKeyStates[key] = true; }
void specialUp(int key, int x, int y) { specialKeyStates[key] = false; }

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(wind_w, wind_h);
	glutCreateWindow("Medan's Fast Furious 2D - Chapter 1");
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialDown);
	glutSpecialUpFunc(specialUp);
	glClearColor(0.01f, 0.02f, 0.03f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	resetGame();
	glutMainLoop();
	return 0;
}