#include <GL/glut.h>
#include "RigidBody.h"

RigidBody* body = nullptr;
float angleX = 0.0f;
float angleY = 0.0f;
int lastX = 0, lastY = 0;
bool mouseLeftDown = false;
bool mouseRightDown = false;
float zoom = -10.0f;

// Инициализация RigidBody
void initBody(int nx, int ny, int nz, double mass, double k, double b, double c, double d, Lattice type) {
    body = new RigidBody(nx, ny, nz, mass, k, b, c, d, type);
}

// Обновление физики тела
void updatePhysics() {
    body->update(1, 0.05);
    body->move(0.01);
    glutPostRedisplay();
}

// Функция отрисовки
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Позиция камеры
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Оси координат
    glBegin(GL_LINES);
    // X - красный
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);
    // Y - зеленый
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    // Z - синий
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();

    // Отрисовка точек тела
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // Белый цвет для точек

    for (int i = 0; i < body->getNum(); i++) {
        Vector<double> coords = body->getPointCoords(i);
        glVertex3f(coords.getCoord(0), coords.getCoord(1), coords.getCoord(2));
    }
    glEnd();

    // Отрисовка связей между точками
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f); // Серый цвет для связей

    for (int i = 0; i < body->getNum(); i++) {
        Vector<double> coords1 = body->getPointCoords(i);
        for (int j = 0; j < body->getPointConnects(i); j++) {
            int neighborIdx = body->getPointNeighborIndex(i, j);
            if (neighborIdx > i) { // Чтобы не рисовать одну связь дважды
                Vector<double> coords2 = body->getPointCoords(neighborIdx);
                glVertex3f(coords1.getCoord(0), coords1.getCoord(1), coords1.getCoord(2));
                glVertex3f(coords2.getCoord(0), coords2.getCoord(1), coords2.getCoord(2));
            }
        }
    }
    glEnd();

    glutSwapBuffers();
}

// Обработка изменения размеров окна
void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Обработка нажатий клавиш
void processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) // ESC
        exit(0);
    else if (key == 'r' || key == 'R') // Сброс
        angleX = angleY = 0.0f;
    else if (key == ' ') // Пауза/продолжение
        glutIdleFunc(updatePhysics);

    glutPostRedisplay();
}

// Обработка специальных клавиш
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            angleX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            angleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            angleY += 5.0f;
            break;
        case GLUT_KEY_PAGE_UP:
            zoom += 0.5f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            zoom -= 0.5f;
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
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            lastY = y;
            mouseRightDown = true;
        } else if (state == GLUT_UP) {
            mouseRightDown = false;
        }
    }
}

int main(int argc, char **argv) {
    // Инициализация GLUT
    int cnt, mass, nx, ny, nz;
    double k, b, c, d, f;
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
//    std::cout << "Enter type of deformation ( 0 - nothing, 1 - compression, 2 - stretching, 3 - shift, 4 - bend): ";
//    std::cin >> cnt;
//    if(cnt!=0 && cnt!=1 && cnt!=2 && cnt!=3 && cnt!=4){throw std::invalid_argument("Invalid input.");}
//    std::cout << "Enter power of force : ";
//    std::cin >> f;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("RigidBody Visualization");

    // Инициализация OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Инициализация тела
    initBody(nx, ny, nz, mass, k, b, c, d, type);

    // Регистрация callback-функций
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(updatePhysics);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    // Главный цикл GLUT
    glutMainLoop();

    delete body;
    return 0;
}