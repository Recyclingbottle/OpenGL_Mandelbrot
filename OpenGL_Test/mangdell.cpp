#define CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <glut.h>
#include <math.h>

#define M 500
#define ITER 100

typedef double complex[2];

double h = 2., w = 2.;
double cx = 0.5, cy = 0.0;
int deltal, mouseFlag = 1, initFlag = 1;
unsigned char image[M][M];

int TLX, TLY, BRX, BRY;


void CalPixel(void)
{
    int i, j, k;
    double x, y, v = 0;
    double cRe, clm, zRe, zlm, z2Re, z2lm;

    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++) {

            //start point 
            y = i * (h / (500.0 - 1)) + cy - h / 2;
            x = j * (w / (500.0 - 1)) + cx - w / 2;

            zRe = zlm = 0;
            cRe = x; clm = y;

            for (k = 0; k < ITER + deltal; k++) {
                z2Re = zRe * zRe - zlm * zlm + cRe;
                z2lm = 2. * zRe * zlm + clm;
                v = z2Re * z2Re + z2lm * z2lm;
                if (v > 2.) break;
                zRe = z2Re;
                zlm = z2lm;

            }
            if (v > 1.) v = 1.;
            image[i][j] = (unsigned char)(v * 255.);


        }

}

void DrawPixel(void)
{
    int i, j;
    glBegin(GL_POINTS);
    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++) {
            glColor3ub(image[i][j], image[i][j], image[i][j]);
            glVertex3i(j, i, 0);
        }
    glEnd();

}

void Display(void)
{
    //int i, j;
    glClearColor(1.0, 1.0, 1.0, 1.0);

    if (initFlag) {
        glClear(GL_COLOR_BUFFER_BIT);
        CalPixel();
    }
    initFlag = 0;

    DrawPixel();

    if (mouseFlag) {
        glColor4f(1.0, 0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex3i(TLX, M - TLY, 0);
        glVertex3i(TLX, M - BRY, 0);
        glVertex3i(BRX, M - BRY, 0);
        glVertex3i(BRX, M - TLY, 0);
        glEnd();
    }

    glutSwapBuffers();

}

void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 500.0, 0, 500.0, -1.0, 1.0);

}

void MouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
    int px, py;
    int wideW;

    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        TLX = X;
        TLY = Y;
        mouseFlag = 1;
        printf("Left button down : %d %d \n", X, Y);
    }

    if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP) {
        BRX = X;
        BRY = Y;
        printf("Left button up : %d %d\n", X, Y);

        px = (TLX + BRX) / 2; py = (TLY + BRY) / 2;
        if (abs(BRX - TLX) > abs(BRY - TLY))
            wideW = abs(BRX - TLX);
        else wideW = abs(BRY - TLY);

        cx += (px - 500.0 / 2) * w / 500.0; cy -= (py - 500.0 / 2) * h / 500.0;
        h = w = wideW * h / 500.0;
        deltal = 10 * (int)(1 - log10(h) / log10(2.));


        initFlag = 1;
        mouseFlag = 0;
        glutPostRedisplay();
    }
}

void MouseMove(GLint X, GLint Y)
{
    BRX = X;
    BRY = Y;
    glutPostRedisplay();
}

void init()
{
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(M, M);
    glutCreateWindow("Mangdell");
    init();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMove);


    glutMainLoop();

    return 0;
}