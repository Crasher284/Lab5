#include <GL/glut.h>
#include "RigidBody.h"

RigidBody* body = nullptr;
float angleX = 0.0f;
float angleY = 0.0f;
int lastX = 0, lastY = 0;
bool mouseLeftDown = false;
bool mouseRightDown = false;
float zoom = -10.0f;
int phase = 0;
double force = 0.0, dforce = 0.01;
double timeSpeed = 0.01;

void initBody(int nx, int ny, int nz, double mass, double k, double b, double c, double d, Lattice type) {
    body = new RigidBody(nx, ny, nz, mass, k, b, c, d, type);
}

void updatePhysics() {
    body->update(phase, force);
    body->move(timeSpeed);
    glutPostRedisplay();
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < body->getNum(); i++) {
        Vector<double> coords = body->getPointCoords(i);
        glVertex3f(coords.getCoord(0), coords.getCoord(1), coords.getCoord(2));
    }
    glEnd();

    glBegin(GL_LINES);

    for (int i = 0; i < body->getNum(); i++) {
        Vector<double> coords1 = body->getPointCoords(i);
        for (int j = 0; j < body->getPointConnects(i); j++) {
            int neighborIdx = body->getPointNeighborIndex(i, j);
            if (neighborIdx > i) {
                Vector<double> coords2 = body->getPointCoords(neighborIdx);
                double colorK = body->getPointLinkDeformation(i, j);
                if(colorK>=0){
                    glColor3f(colorK, 1.0f-colorK, 0.0f);
                }else{
                    glColor3f(0.0f, 1.0f+colorK, -colorK);
                }
                glVertex3f(coords1.getCoord(0), coords1.getCoord(1), coords1.getCoord(2));
                glVertex3f(coords2.getCoord(0), coords2.getCoord(1), coords2.getCoord(2));
            }
        }
    }
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);

    int textPosX = 20;
    int textPosY = glutGet(GLUT_WINDOW_HEIGHT) - 30;

    std::string phaseText;

    switch (phase){
        case 0:
            phaseText = "Deformation: none";
            break;
        case 1:
            phaseText = "Deformation: compression";
            break;
        case 2:
            phaseText = "Deformation: stretching";
            break;
        case 3:
            phaseText = "Deformation: shift";
            break;
        case 4:
            phaseText = "Deformation: bend";
            break;
    }

    glRasterPos2i(textPosX, textPosY);
    for (char c : phaseText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    textPosY = glutGet(GLUT_WINDOW_HEIGHT) - 60;

    std::string forceText = "Force: " +
                            std::to_string(force).substr(0, 4);

    glRasterPos2i(textPosX, textPosY);
    for (char c : forceText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) // ESC
        exit(0);
    else if (key == 'r' || key == 'R') // Сброс
        angleX = angleY = 0.0f;
    else if (key == '1')
        phase = 0;
    else if (key == '2')
        phase = 1;
    else if (key == '3')
        phase = 2;
    else if (key == '4')
        phase = 3;
    else if (key == '5')
        phase = 4;
    else if (key == '+'){
        if(force>=dforce*100){
            dforce*=10;
        }
        force += dforce;
    }else if (key == '-'){
        if(force<=dforce*10 && dforce>0.01){
            dforce/=10;
        }
        if(force - dforce < 0){
            force = 0;
        }else{
            force -= dforce;
        }
    }else if (key == '*'){
        force = 0;
    }
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            break;
    }
    glutPostRedisplay();
}

// Обработка мыши
void mouseMotion(int x, int y) {
    if (mouseLeftDown) {
        angleY += (x - lastX);
        angleX += (y - lastY);
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
    if (mouseRightDown) {
        zoom += (y - lastY) * 0.1f;
        lastY = y;
        glutPostRedisplay();
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            lastX = x;
            lastY = y;
            mouseLeftDown = true;
        } else if (state == GLUT_UP) {
            mouseLeftDown = false;
        }
    } else if (button == 3) {
        // mousewheel up
        if (state == GLUT_UP) return;
        zoom += 0.5f;
        glutPostRedisplay();
    } else if (button == 4) {
        // mousewheel down
        if (state == GLUT_UP) return;
        zoom -= 0.5f;
        glutPostRedisplay();
    }
}

void renderStrokeString(float x, float y, void *font, const char *string) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    while (*string) {
        glutStrokeCharacter(font, *string);
        string++;
    }
    glPopMatrix();
}

int main(int argc, char **argv) {
    int cnt, nx, ny, nz;
    double k, mass, b, c, d, f;
    std::cout << "Enter type of crystal lattice (0 - cubic, 1 - cubic-facescentered, 2 - cubic-volumecentered):";
    std::cin >> cnt;
    if(cnt!=0 && cnt!=1 && cnt!=2){throw std::invalid_argument("Invalid input.");}
    Lattice type;
    if(cnt==0){type = CUBE;}
    if(cnt==1){type = CUBEFACES;}
    if(cnt==2){type = CUBEVOLUME;}
    std::cout << "Enter total mass of body:";
    std::cin >> mass;
    std::cout << "Enter size of body (by x):";
    std::cin >> nx;
    std::cout << "Enter size of body (by y):";
    std::cin >> ny;
    std::cout << "Enter size of body (by z):";
    std::cin >> nz;
    std::cout << "Enter elasticy of body: ";
    std::cin >> k;
    std::cout << "Enter unbreakability of body:";
    std::cin >> b;
    std::cout << "Enter viscosity of body:";
    std::cin >> c;
    std::cout << "Enter distance between points:";
    std::cin >> d;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("RigidBody Visualization");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initBody(nx, ny, nz, mass, k, b, c, d, type);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(updatePhysics);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();

    delete body;
    return 0;
}