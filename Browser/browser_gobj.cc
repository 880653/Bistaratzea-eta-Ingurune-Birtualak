#include <cstdio>
#include "mg.h"
#include "scenes.h"
#include "skybox.h"

// global variables
static float step = 0.1; // advance/retreat step
const static float angle_step = 1.0f * Constants::degree_to_rad; // angular step (rotations, etc)
static GObject *gObj = 0;
static Trfm3D *T = 0;

// Init OpenGL rendering context (including rendering window)

static void InitRenderContext(int argc, char** argv,
							  int width, int height,
							  int wposx, int wposy) {

	GLenum glew_err;

	// Init openGL and create a window
	glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize ( width, height );
	glutInitWindowPosition ( wposx, wposy );
	// create one window for OpenGL graphics
	glutCreateWindow("browser");

	// Uncomment following line if you have problems initiating GLEW
	//
	// glewExperimental = GL_TRUE;

	glew_err = glewInit();

	if (glew_err != GLEW_OK) {
		fprintf(stderr, "Error when calling glewInit: %s\n", glewGetString(glew_err));
		exit(1);
	}

	// set OpenGL state values
	glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
	glViewport(0, 0, width, height);              // Reset The Current Viewport And Perspective Transformation

	// Enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Turn Depth Testing On
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f); // Also, sets GLSL fragmen shader gl_DepthRange variable

	// Aliasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void InitShaders() {
	ShaderManager *mgr = ShaderManager::instance();
	mgr->create("dummy", "Shaders/dummy.vert", "Shaders/dummy.frag");
}


static void Display() {

	RenderState *rs = RenderState::instance();

	// draw the background color
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rs->loadIdentity(RenderState::modelview);
	rs->addTrfm(RenderState::modelview, T);

	rs->setShader(ShaderManager::instance()->find("dummy"));

	if (gObj) gObj->draw();
	glutSwapBuffers();
}


// Keyboard dispatcher when ALT key is pressed
static void Keyboard_alt(unsigned char key) {

	static bool line_aliasing = true;
	static bool drawBB = false;
	RenderState *rs;

	switch(key)
		{
		case 'a':
			// TODO: render context
			line_aliasing = !line_aliasing;
			if (line_aliasing) {
				glEnable(GL_LINE_SMOOTH);
			} else {
				glDisable(GL_LINE_SMOOTH);
			}
			break;
		case 'v':
			RenderState::instance()->top(RenderState::modelview).print();
			break;
		case 'p':
			RenderState::instance()->top(RenderState::projection).print();
			break;
		}
	glutPostRedisplay( );
}


// General keyboard dispatcher
static void Keyboard (unsigned char key, int x, int y) {

	int key_mod = glutGetModifiers();
	if (key_mod & GLUT_ACTIVE_ALT) {
		// If ALT key pressed, call Keyboard_alt and exit
		Keyboard_alt(key);
		return;
	}

	switch(key)
		{
		case 's':
			// Enable Shading
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 'S':
			// Disable Shading
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 'z':
			glEnable(GL_CULL_FACE);
			break;
		case 'Z':
			glDisable(GL_CULL_FACE);
			break;
		case 'a':
			T->addRotY(-angle_step);
			break;
		case 'd':
			T->addRotY(angle_step);
			break;
		case 'w':
			T->addRotX(-angle_step);
			break;
		case 'x':
			T->addRotX(angle_step);
			break;
		case 'A':
			T->addRotAxis(Vector3::UNIT_Y, T->getTranslation(), -angle_step);
			break;
		case 'D':
			T->addRotAxis(Vector3::UNIT_Y, T->getTranslation(), angle_step);
			break;
		case 'W':
			T->addRotAxis(Vector3::UNIT_X, T->getTranslation(), -angle_step);
			break;
		case 'X':
			T->addRotAxis(Vector3::UNIT_X, T->getTranslation(), angle_step);
			break;
		case 'i':
			T->addTrans(Vector3(0.0, 0.0, -step));
			break;
		case 'k':
			T->addTrans(Vector3(0.0, 0.0, step));
			break;
		case 27: // ESC
			exit(0);
			break;
		}
	glutPostRedisplay( );
}

void create_scene_tmesh() {

	float P[3];
	TriangleMesh *mesh;

	mesh = new TriangleMesh;

	mesh->addPoint(Vector3(-0.25,  0.25, -0.25)); // P0
	mesh->addPoint(Vector3(-0.25, -0.25, -0.25)); // P1
	mesh->addPoint(Vector3(0.25, -0.25, -0.25)); // P2
	mesh->addPoint(Vector3(0.25,  0.25, -0.25)); // P3

	mesh->addPoint(Vector3(0.0,  0.5, 0.0)); // P4
	mesh->addPoint(Vector3(0.0, 0.0, 0.0)); // P5
	mesh->addPoint(Vector3(0.5, 0.0, 0.0)); // P6
	mesh->addPoint(Vector3(0.5,  0.5, 0.0)); // P7

	// AURREKO ALDEA
	mesh->addTriangle(0, 1, 2);	// P0 - P1 - P2
	mesh->addTriangle(2, 3, 0); // P2 - P3 - P0

	// ATZEKO ALDEA
	mesh->addTriangle(4, 7, 6); // P4 - P7 - P6
	mesh->addTriangle(4, 6, 5); // P4 - P6 - P5

	// ESKUINEKO ALDEA
	mesh->addTriangle(7, 3, 2); // P7 - P3 - P2
	mesh->addTriangle(7, 2, 6); // P7 - P2 - P6

	// GOIKO ALDEA
	mesh->addTriangle(3, 7, 4); // P3 - P7 - P4
	mesh->addTriangle(0, 3, 4); // P0 - P3 - P4

	// EZKERREKO ALDEA
	mesh->addTriangle(0, 4, 5); // P0 - P4 - P5
	mesh->addTriangle(5, 1, 0); // P5 - P1 - P0

	// BEHEKO ALDEA
	mesh->addTriangle(5, 6, 2); // P5 - P6 - P2
	mesh->addTriangle(2, 1, 5); // P2 - P1 - P5

	gObj = GObjectManager::instance()->create("MG_GOBJ");
	gObj->add(mesh);
}

int main(int argc, char** argv) {

	srand(time(0));
	InitRenderContext(argc, argv, 900, 700, 100, 0);
	// set GLUT callback functions
	glutDisplayFunc( Display );
	glutKeyboardFunc( Keyboard );
	// Init shaders
	InitShaders();
	T = new Trfm3D; // global variable
	// create Scene
	create_scene_tmesh();
	glutMainLoop();
	delete T;
	return 0;
}
