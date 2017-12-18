//
//  main.cpp
//  shader_hw2
//
//  Created by Krithika Saravanan on 11/5/16.
//  Copyright © 2016 Krithika Saravanan. All rights reserved.
//
#define GLFW_INCLUDE_GLCOREARB
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <unistd.h>
#include <string.h>
#include<stdio.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Opengl/gl.h>
#include <Opengl/gl3ext.h>
#include <vector>
static int spin = 0;


GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };

//Global light model
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_ambient[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[4] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };


// for GLSL
GLuint vsID, fsID, pID, VBO, VAO, EBO;

// for uniform input
GLfloat projectionMat[16], modelViewMat[16];

using namespace std;
float x[80000],y[80000],z[80000], fx[80000], fy[80000], fz[80000];
float vertx[800000],facex[80000];
int xmin=0,ymin=0,zmin=0,xmax=0,ymax=0,zmax=0;
int fc;
// angle of rotation for the camera direction
float angle=0.0;
//u,v,n vectors
float ux=1.0,uy=0.0,uz=0.0,vx=0.0,vy=1.0,vz=0.0,nx=0.0,ny=0.0,nz=1.0;
float fraction=0.1f;
// actual vector representing the camera's direction
float cen_x=0.0f,cen_y=0.0,cen_z=-1.0f;
// XYZ position of the camera
float px=0.0f,py=0.0,pz=1.0;
float nearval=0.0, farval=20.0;
float l= -2.0, r= 2.0, b = -2.0 , t=2.0;
//float p_matrix[16], mv_matrix[16];
 std::vector<float> vectorv;
 std::vector<int> vectorf;

void readShaderFile(const GLchar* shaderPath, std::string& shaderCode)
{
    
    std::ifstream shaderFile;
    
    // ensures ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        
        // Read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        
        // close file handlers
        shaderFile.close();
        
        // Convert stream into GLchar array
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
}
void init(void)
{
    //GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
    //GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
    //GLfloat mat_shininess[] = { 250.0 };
    //GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //glShadeModel (GL_SMOOTH);
    
    //global light model
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseMaterial );
    
    //glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_SHININESS, mat_shininess);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
    
   // glLightfv(GL_LIGHT0, GL_SHININESS, light_specular);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_specular);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
   // glEnable(GL_LIGHT1);
    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
   // glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
}

void setShaders()
{
    char *vs = NULL, *fs = NULL;
    
    std::string vertShaderString; //empty string
    std::string fragShaderString; //empty string
    
    vsID = glCreateShader(GL_VERTEX_SHADER);
    fsID = glCreateShader(GL_FRAGMENT_SHADER);
    
    readShaderFile("/Users/krithika/Desktop/Sem 3/Computer Graphics/OpenGL_Basics/vertexshader.txt", vertShaderString);
    readShaderFile("/Users/krithika/Desktop/Sem 3/Computer Graphics/OpenGL_Basics/fragshader.txt", fragShaderString);
    
    const GLchar * pVertShaderSource = vertShaderString.c_str();
    const GLchar * pFragShaderSource = fragShaderString.c_str();
    
    glShaderSource(vsID, 1, &pVertShaderSource, NULL);
    glShaderSource(fsID, 1, &pFragShaderSource, NULL);
    
    glCompileShader(vsID);
    glCompileShader(fsID);
    
    pID = glCreateProgram();
    glAttachShader(pID, vsID);
    glAttachShader(pID, fsID);
    
    glLinkProgram(pID);
    glUseProgram(pID);
    
    // get uniform input
    GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
    GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");
    
    glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
    glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);
    
    
    //glDeleteShader(vsID);
    //glDeleteShader(fsID);
}

void projection_matrix()
{
    
    projectionMat[0] = 2*nearval/(r-l);
    projectionMat[1] = 0.0;
    projectionMat[2] = 0.0;
    projectionMat[3] = 0.0;
    projectionMat[4] = 0.0;
    projectionMat[5] = 2*nearval/(t-b);
    projectionMat[6] = 0.0;
    projectionMat[7] = 0.0;
    projectionMat[8] = (r+l)/(r-l);
    projectionMat[9] = (t+b)/(t-b);
    projectionMat[10] = -(farval+nearval)/(farval-nearval);
    projectionMat[11] = -1;
    projectionMat[12] = 0.0;
    projectionMat[13] = 0.0;
    projectionMat[14] = (-2*farval*nearval)/(farval-nearval);
    projectionMat[15] = 0.0;
}

void model_view_matrix()
{
    modelViewMat[0] = ux;
    modelViewMat[1] = vx;
    modelViewMat[2] = nx;
    modelViewMat[3] = 0.0;
    modelViewMat[4] = uy;
    modelViewMat[5] = vy;
    modelViewMat[6] = ny;
    modelViewMat[7] = 0.0;
    modelViewMat[8] = uz;
    modelViewMat[9] = vz;
    modelViewMat[10] = nz;
    modelViewMat[11] = 0.0;
    modelViewMat[12] = -(ux*px+uy*py+uz*pz);
    modelViewMat[13] = -(vx*px+vy*py+vz*pz);
    modelViewMat[14] = -(nx*px+ny*py+nz*pz);
    modelViewMat[15] = 1.0;
}


void display_shader(void)
{
    glUseProgram(pID);
    //glUseProgram(0);
    
    glValidateProgram(pID);
    GLint validate = 0;
    glGetProgramiv(pID, GL_VALIDATE_STATUS, &validate);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLint modeViewMatLocation = glGetUniformLocation(pID, "modelViewMatrix");
    GLint projectionMatLocation = glGetUniformLocation(pID, "projectionMatrix");
    
    glUniformMatrix4fv(modeViewMatLocation, 1, GL_FALSE, modelViewMat);
    glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, projectionMat);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_QUADS, vectorf.size()*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glutSwapBuffers();
}


void polygon(int a, int b, int c)
{
    glBegin(GL_POLYGON);
    //glColor3f(1.0,0.0,0.0);
    glVertex3f(x[a],y[a],z[a]);
    //glColor3fv(colors[a]);
    glVertex3f(x[b],y[b],z[b]);
    //glColor3fv(colors[b]);
    glVertex3f(x[c],y[c],z[c]);
    //glColor3fv(colors[c]);
    
    glEnd();
}
void open_gl()
{
    cout << fc << endl;
    int x1,x2,x3;
    for(int k=0;k<fc;k++)
    {
        x1 =fx[k]-1;
        x2 =fy[k]-1;
        x3 =fz[k]-1;
        polygon(x1,x2,x3);
        
    }
    glLoadIdentity();
    
    
}
void initBufferObject(void)
{
    char obj[20], obj1[20], obj2[20],obj3[20],fobj1[20],fobj2[20],fobj3[20];
    
    if (strcmp(obj, "v") == 0)
    {

    vectorv. push_back(atof(obj1));
    vectorv.push_back(atof(obj2));
    vectorv.push_back(atof(obj3));
    vectorv.push_back (1);
    vectorv.push_back (0);
    vectorv.push_back (0);
    }
    
    if(strcmp(obj, "f") == 0 )
    {
        vectorf.push_back (atoi(obj1));
        vectorf.push_back (atoi(obj2));
        vectorf.push_back (atoi(obj3));
    }
    
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // setup VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vectorv), vectorv, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    // Use depth buffering for hidden surface elimination
    glEnable(GL_DEPTH_TEST);
    
    // Setup the view of the cube
    glMatrixMode(GL_PROJECTION);
    gluPerspective(40.0, 1.0, 1.0, 10.0);
    
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMat);
    
    
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,        /* center is at (0,0,0) */
              0.0, 1.0, 0.);        /* up is in positive Y direction */
    

    
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMat);
}
static GLint axis = 2;
void object ()
{
    FILE *infile;
    FILE *ifile;
    char obj[20], obj1[20], obj2[20],obj3[20],fobj1[20],fobj2[20],fobj3[20];
    int i =0, j=0;
    //int nearval, farval;
    infile = fopen ("/Users/krithika/Desktop/Sem 3/Computer Graphics/models/bunny.obj", "r");
    ifile = fopen ("/Users/krithika/Desktop/Sem 3/Computer Graphics/models/cactus.obj" , "r");
    if(infile == NULL)
    {
        cerr<<"Error opening file";
        exit(1);
        
    }
    else{
        while( fscanf(infile,"%s",obj)!= EOF)
        {
            if (strcmp(obj, "v") == 0)
            {
                
                fscanf(infile,"%s",obj1);   //for x value
                fscanf(infile,"%s",obj2);   //for y value
                fscanf(infile,"%s",obj3);   //for z value
                x[i] = atof(obj1);
                y[i] = atof(obj2);
                z[i] = atof(obj3);
                i++;
                
                
            }
            
            if(strcmp(obj, "f") == 0 )
            {
               
                fscanf(infile,"%s",fobj1);   //for x value
                fscanf(infile,"%s",fobj2);   //for y value
                fscanf(infile,"%s",fobj3);   //for z value
                fx[j] = atoi(fobj1);
                fy[j] = atoi(fobj2);
                fz[j] = atoi(fobj3);
                j++;
               
                
            }
            
        }
        fc = j;
        cout << i << endl;
        cout << j << endl;
        
        
    }
    if (xmin > x[i])
    {
        xmin = x[i];
    }
    if (xmax < x[i])
    {
        xmax = x[i];
    }
    if (ymin > y[i])
    {
        ymin = y[i];
    }
    if (ymax < y[i])
    {
        ymax = y[i];
    }
    if (zmin > z[i])
    {
        zmin = z[i];
    }
    if (zmax < z[i])
    {
        zmax = z[i];
    }
    
}

void object1()
{
    FILE *ifile;
    char obj[20], obj1[20], obj2[20],obj3[20],fobj1[20],fobj2[20],fobj3[20];
    int i =0, j=0;
    ifile = fopen ("/Users/krithika/Desktop/Sem 3/Computer Graphics/models/cactus.obj" , "r");
    if(ifile == NULL)
    {
        cerr<<"Error opening file";
        exit(1);
        
    }
    else{
        while( fscanf(ifile,"%s",obj)!= EOF)
        {
            if (strcmp(obj, "v") == 0)
            {
                fscanf(ifile,"%s",obj1);   //for x value
                fscanf(ifile,"%s",obj2);   //for y value
                fscanf(ifile,"%s",obj3);   //for z value
                x[i] = atof(obj1);
                y[i] = atof(obj2);
                z[i] = atof(obj3);
                i++;
                
            }
            
            if(strcmp(obj, "f") == 0 )
            {
                fscanf(ifile,"%s",fobj1);   //for x value
                fscanf(ifile,"%s",fobj2);   //for y value
                fscanf(ifile,"%s",fobj3);   //for z value
                fx[j] = atoi(fobj1);
                fy[j] = atoi(fobj2);
                fz[j] = atoi(fobj3);
                j++;
                
            }
        }
        fc = j;
        cout << i << endl;
        cout << j << endl;
        
        
    }
    
}
void display()
{
    GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glShadeModel(GL_SMOOTH);
    glLoadIdentity ();             // clear the matrix
    gluLookAt(	px, py , pz, px+cen_x, py+cen_y,  pz+cen_z, 0.0f, 1.0f,  0.0f);
    glPushMatrix ();
    glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glEnable (GL_LIGHTING);
    glPopMatrix ();
    open_gl();
    glFlush();
    glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
    if (key == 's' || key == 'S') exit(0);
    //Changing the color of the object
    else if (key == 'w' || key == 'W')
    {
        spin = (spin + 30) % 360;
        glutPostRedisplay();
    }
    else if (key == 'r' || key == 'R')
    {
        glColor3f(1.0,0.0,0.0);
    }
    else if (key == 'g' || key == 'G')
    {
        glColor3f(0.0,1.0,0.0);
    }
    else if (key == 'b' || key == 'B')
    {
        glColor3f(0.0,0.0,1.0);
    }
    //Point
    else if (key == 'p' || key == 'P')
    {
        glPolygonMode(GL_FRONT, GL_POINT);
        glPolygonMode(GL_BACK, GL_POINT);
        
    }
    //Wireframe
    else if (key == 'l' || key == 'L')
    {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        
    }
    //Fill
    else if (key == 'f' || key == 'F')
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        
    }
    //Reading a new model file through the user interface
    else if (key == 'q' || key == 'Q')
    {
        object1();
        
    }
    else if (key == 'u' || key == 'U')
    {
        object();
    }
    //Global_light_interaction
    else if (key == 'x' || key == 'X')
    {
        
        lmodel_ambient[0] += 0.1;
        if (lmodel_ambient[0] > 1.0)
            lmodel_ambient[0] = 0.0;
        glColor4fv(lmodel_ambient);
        glutPostRedisplay();
    }
    else if (key == 'y' || key == 'Y')
    {
        
        lmodel_ambient[1] += 0.1;
        if (lmodel_ambient[1] > 1.0)
            lmodel_ambient[1] = 0.0;
        glColor4fv(lmodel_ambient);
        glutPostRedisplay();
    }
    
    else if (key == 't')
    {
        lmodel_ambient[2] += 0.1;
        if (lmodel_ambient[2] > 1.0)
            lmodel_ambient[2] = 0.0;
        glColor4fv(lmodel_ambient);
        glutPostRedisplay();
    }
    //Reset the camera to its original position
    else if (key == 'a' || key == 'A')
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        px =0.0;py=0.0;pz=0.0;
        cen_x=0.0;cen_y=0.0;cen_z=-1.0;
        
        glMatrixMode(GL_MODELVIEW);
    }
    
  
    //rotation
    //rotation about y axis
  
    else if (key == 'j' || key == 'J')
    {
        /*px -= cen_x * fraction;
         pz = 0.0+ cen_z * fraction;
         angle += 0.01f;
         cen_x = sin(angle);
         cen_z = -cos(angle);*/
        //px-=((ux * fraction)+(uy*fraction)+(uz*fraction));
        // py-=((vx * fraction)+(vy*fraction)+(vz*fraction));
        pz = 0.0-((nx * fraction)+(ny*fraction)+(nz*fraction));
        angle -= 0.01f;
        cen_x = sin(angle);
        cen_z = -cos(angle);
    }
//specular light interaction
   else if (key == 'n' )
    {
        glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        {
        light_specular[0] += 0.1;
        if (light_specular[0] > 1.0)
            light_specular[0] = 0.0;
        glColor4fv(light_specular);
        glutPostRedisplay();
        }}
    else if (key == 'm' )
    {
        glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        {
        light_specular[1] += 0.1;
        if (light_specular[1] > 1.0)
            light_specular[1] = 0.0;
        glColor4fv(light_specular);
        glutPostRedisplay();
        
        }}
    else if (key == 'k' )
    {
        glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        {
        light_specular[2] += 0.1;
        if (light_specular[2] > 1.0)
            light_specular[2] = 0.0;
        glColor4fv(light_specular);
        glutPostRedisplay();

        }}
    else if (key == 'z' )
    {
       glShadeModel (GL_FLAT);
        glProvokingVertex (GL_FIRST_VERTEX_CONVENTION);
       //glGetIntegerv( GLenum(GL_ARRAY_BUFFER) GL_ACCUM_BLUE_BITS, GLint* GL_TRANSPOSE_COLOR_MATRIX);

    }
    else if (key == 'v' )
    {
        glShadeModel (GL_SMOOTH);
    }
    
    else if (key == 'o' | key == 'O')
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
    }
    else if (key == 'c' | key == 'C')
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
    }

    
    
    glutIdleFunc(display);
}


void myReshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-2.0, 2.0, -2.0*(GLfloat)h/(GLfloat)w,
                 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
        glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
                 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Adaption of camera with respect to the dimension of the 3D models
    //gluPerspective(60,w/h,zmin,zmax);
    //glMatrixMode(GL_MODELVIEW);

    
    
}
void myReshape1(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
                 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
        glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
                 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Adaption of camera with respect to the dimension of the 3D models
    //gluPerspective(60,w/h,zmin,zmax);
    //glMatrixMode(GL_MODELVIEW);
    
    
    
}

//Ambient light interaction
void processSpecialKeys(int key, int xx, int yy)
{
    if( key == GLUT_KEY_LEFT)                  /*  change red  */
    {
        light_ambient[0] += 0.1;
        if (light_ambient[0] > 1.0)
            light_ambient[0] = 0.0;
        glColor4fv(light_ambient);
        glutPostRedisplay();
        
    }
    else if (key == GLUT_KEY_RIGHT)          /*  change green  */
    {
        light_ambient[1] += 0.1;
        if (light_ambient[1] > 1.0)
            light_ambient[1] = 0.0;
        glColor4fv(light_ambient);
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_UP )             /*  change blue  */
    {
        light_ambient[2] += 0.1;
        if (light_ambient[2] > 1.0)
            light_ambient[2] = 0.0;
        glColor4fv(light_ambient);
        glutPostRedisplay();
        
    }
    else if (key == GLUT_KEY_DOWN)
    {
        glEnable(GL_LIGHT1);
        
    }

    
    glutIdleFunc(display);
}


void opengl()
{
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {       /*  change red  */
                diffuseMaterial[0] += 0.1;
                if (diffuseMaterial[0] > 1.0)
                    diffuseMaterial[0] = 0.0;
                glColor4fv(diffuseMaterial);
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {       /*  change green  */
                diffuseMaterial[1] += 0.1;
                if (diffuseMaterial[1] > 1.0)
                    diffuseMaterial[1] = 0.0;
                glColor4fv(diffuseMaterial);
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {      /*  change blue  */
               diffuseMaterial[2] += 0.1;
                if (diffuseMaterial[2] > 1.0)
                    diffuseMaterial[2] = 0.0;
                glColor4fv(diffuseMaterial);
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("open_gl");
    
    glutPositionWindow(10,0);
    object();
    //init ();
    glLoadIdentity ();
    //glutSpecialFunc(processSpecialKeys);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
     init ();
    glutIdleFunc(open_gl);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glEnable(GL_DEPTH_TEST);
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("shader");
   
    glutPositionWindow(700,0);
    glewExperimental = GL_TRUE;
    glewInit();
    initBufferObject();
    setShaders();
    // init ();
    glLoadIdentity ();
     //init ();
    glutSpecialFunc(processSpecialKeys);
    glutReshapeFunc(myReshape1);
    glutDisplayFunc(display_shader);
     init ();
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}





