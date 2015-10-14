#include <cmath>

#include "GL/glut.h"

#include "nutshell_dynamics/rigidBody.h"

using std::sqrt;

void displayFunc();
void reshapeFunc(int, int);
void timerFunc(int);

unsigned short nut::refineIterations = 24u; // each iteration adds precision

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("Nutshell Dynamics - Humble");
	glutDisplayFunc(&displayFunc);
	glutReshapeFunc(&reshapeFunc);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_FLAT);

	glutTimerFunc(16, &timerFunc, 0);
	glutMainLoop();
}

namespace
{
	unsigned face[][3]{{0u, 1u, 2u}, {0u, 1u, 3u}, {0u, 2u, 3u}, {1u, 2u, 3u}};

	nut::ThreeVector<float> vertices[]{
		nut::ThreeVector<float>{.0f, -1.f / (2.f * sqrt(6.f)), 1.f / sqrt(3.f)},
		nut::ThreeVector<float>{.5f, -1.f / (2.f * sqrt(6.f)), -1.f / (2.f * sqrt(3.f))},
		nut::ThreeVector<float>{-.5f, -1.f / (2.f * sqrt(6.f)), -1.f / (2.f * sqrt(3.f))},
		nut::ThreeVector<float>{.0f, sqrt(3.f / 2.f) / 2.f, .0f}};

	nut::ThreeVector<float> surfaceNormals[]{
		nut::ThreeVector<float>{.0f, -1.f, .0f},
		nut::ThreeVector<float>{sqrt( 2.f / 3.f), 1.f / 3.f, sqrt( 2.f) / 3.f},
		nut::ThreeVector<float>{- sqrt( 2.f / 3.f), 1.f / 3.f, sqrt( 2.f) / 3.f},
		nut::ThreeVector<float>{.0f, 1.f / 3.f, -2.f * sqrt( 2.f) / 3.f}};

	const nut::Body::Pool bodyPool{face, 4}; // '4' means 4 triangles and surface normals
	const nut::RigidBody::Pool rigidBodyPool{vertices, surfaceNormals, 4}; // here vertices
}

class Tetrahedron : public nut::RigidBody // regular, solid and of uniform density
{
	public:

		Tetrahedron(float mass, float edgeLength, const float(& position)[3],
			const nut::ThreeVector<float>& velocity, float angularFrequency,
			const nut::ThreeVector<float>& rotationAxis) :
			nut::RigidBody{mass,
			{.05f * mass * edgeLength * edgeLength,
			 .05f * mass * edgeLength * edgeLength,
			 .05f * mass * edgeLength * edgeLength},
			{{1.f, .0f, .0f, .0f,
			  .0f, 1.f, .0f, .0f,
			  .0f, .0f, 1.f, .0f,
			  position[0], position[1], position[2], 1.f}},
			velocity, angularFrequency, rotationAxis, bodyPool, rigidBodyPool} {}

		void display()
		{
			float modelViewMatrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

			glMultMatrixf(this->modelViewMatrix);

			glBegin(GL_TRIANGLE_STRIP);
				glNormal3fv(this->getSurfaceNormal()[0]);
				glVertex3fv(this->getVertex()[2]);
				glVertex3fv(this->getVertex()[1]);
				glVertex3fv(this->getVertex()[0]);
				glNormal3fv(this->getSurfaceNormal()[1]);
				glVertex3fv(this->getVertex()[3]);
				glNormal3fv(this->getSurfaceNormal()[2]);
				glVertex3fv(this->getVertex()[2]);
				glNormal3fv(this->getSurfaceNormal()[3]);
				glVertex3fv(this->getVertex()[1]);
			glEnd();

			glLoadMatrixf(modelViewMatrix);
		}
};

void displayFunc()
{
	static Tetrahedron foo{9.f, 6.f, {.01f, .5f, .0f}, {.0f, .0f, .0f}, .0f, {.0f, .0f, .0f}};

	static Tetrahedron bar{1.f, 6.f, {.0f, -2.f, .0f}, {.0f, .0f, .0f}, .0f, {.0f, .0f, .0f}};

	nut::ThreeVector<float> displacement =
		 nut::ThreeVector<float>{&bar.getObjectMatrix()[12]} -
		 nut::ThreeVector<float>{&foo.getObjectMatrix()[12]};

	foo.getVelocity() += .001f / 9.f * displacement.getUnitVector();
	bar.getVelocity() += -.001f * displacement.getUnitVector();

	nut::advanceState(); // all the work is done here

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(.0f, .0f, -7.f); // camera transformation
	foo.display(); bar.display();
	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45., static_cast<double>(width) / height, 1., 200.);
}

void timerFunc(int)
{
	glutPostRedisplay();
	glutTimerFunc(16, &timerFunc, 0);
}

// vim: tw=90 ts=2 sts=-1 sw=0 noet
