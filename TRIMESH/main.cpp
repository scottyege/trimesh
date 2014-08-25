
#include <iostream>
using namespace std;

#include <TriMesh.h>
using namespace trimesh;

#include <GL\freeglut.h>

#include <ctime>

#include<cstdio>

TriMesh *g_mesh = NULL;
int g_width, g_height;

void initFunc()
{
    srand((unsigned) time(0));

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH);
}

void displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex3f(1.0f, -0.3f, 0.0f);
    glVertex3f(-1.0f, -0.3f, 0.0f);
    glEnd();

    float b = g_mesh->bsphere.r + 200.0f;
    float r = g_mesh->bsphere.r;
    float fov = 2.0f * atanf(r / b) * 180.0f / M_PIf;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, ((double)g_width / g_height), 0.001f, 1000.0f);


    point mCenter = g_mesh->bsphere.center;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, b,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glTranslatef(-mCenter[0], -mCenter[1], -mCenter[2]);

    vector<point> &vert = g_mesh->vertices;
    vector<TriMesh::Face> &faces = g_mesh->faces;

    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < faces.size(); i++)
    {
        glColor3f(float((rand() % 256)) / 255.0f, float(rand() % 256) / 255.0, float(rand() % 256) / 255.0f);
        glVertex3fv(vert[faces[i][0]]);
        glVertex3fv(vert[faces[i][1]]);
        glVertex3fv(vert[faces[i][2]]);
    }
    glEnd();

    glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
    glViewport(0, 0, width, height);
    g_width = width;
    g_height = height;
}

void idleFunc()
{
    //glutPostRedisplay();
}

int main(int argc, char *argv[])
{

    glutInitWindowSize(512, 512);
    glutInitWindowPosition(720, 450);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_ALPHA);
    glutInit(&argc, argv);

    if (argc < 2)
    {
        cerr << "need a file name" << endl;
        return 1;
    }

    g_mesh = TriMesh::read(argv[1]);
    if (!g_mesh)
        exit(1);

    g_mesh->need_faces();
    g_mesh->need_bbox();
    g_mesh->need_bsphere();
    g_mesh->need_tstrips();


    glutCreateWindow(argv[1]);
    glutDisplayFunc(displayFunc);

    glutReshapeFunc(reshapeFunc);

    //glutMouseFunc(mousebuttonfunc);
    //glutMotionFunc(mousemotionfunc);
    //glutKeyboardFunc(keyboardfunc);

    glutIdleFunc(idleFunc);

    initFunc();

    glutMainLoop();

    return 0;
}