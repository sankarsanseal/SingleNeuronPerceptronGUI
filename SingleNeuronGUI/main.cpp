/*

 
 * nnstruct.h
 *
 *  Created on: 21 আগস্ট, 2016
 *      Author: sankarsan
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "nnstruct.h"
#include <unistd.h>
#include <time.h>

#define NENDS 2           /* number of end "points" to draw */

//GLdouble width, height;   /* window width and height */
int wd;                   /* GLUT window handle */
int ends[NENDS][2];       /* array of 2D points */



int n;
twoDem * pc;
float w[NO_OF_PARAMETER];

const int width=640;
const int height=640;
const int no_iter=300;
const float eta=0.3f;

int signum(int i)
{
    int j;
    float sum=0;
    int result=0;
    for(j=0;j<NO_OF_PARAMETER;j++)
    {
        sum+=w[j]*(pc[i].x[j]);
    }
    
    if(sum >= 0.0f)
        result= 1;
    else if (sum < 0.0f)
        result= -1;
    
    return result;
}

void updateW(int i)
{
    int j;
    int result=signum(i);
    for(j=0;j<NO_OF_PARAMETER;j++)
    {
        //fprintf(stdout,"%d : x[%d]:%f  w[%d]:%f , signum: %d, result : %d, ylabel: %d\n"
        //		,i,j,pc[i].x[j],j,w[j], signum(i),result,pc[i].ylabel);
        //fprintf(stdout,"%f\n",eta*((float)((pc[i].ylabel)-result))*(pc[i].x[j]));
        w[j]+=eta*((float)((pc[i].ylabel)-result))*(pc[i].x[j]);
        //fprintf(stdout,"%d : x[%d]:%f  w[%d]:%f , signum: %d, result : %d, ylabel: %d\n"
        //				,i,j,pc[i].x[j],j,w[j], signum(i),result,pc[i].ylabel);
    }
    
}


/* Program initialization NOT OpenGL/GLUT dependent,
 as we haven't created a GLUT window yet */
//void
//init(void)
//{
  //  width  = 1280.0;                 /* initial window width and height, */
   // height = 800.0;                  /* within which we draw. */
   // ends[0][0] = (int)(0.25*width);  /* (0,0) is the lower left corner */
   // ends[0][1] = (int)(0.75*height);
    //ends[1][0] = (int)(0.75*width);
   // ends[1][1] = (int)(0.25*height);
    
   // return;
//}*/

/* Callback functions for GLUT */

/* Draw the window - this is where all the GL actions are */
void
display(void)
{
    int i,j,k;
    
    /* clear the screen to white */
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPointSize(5.0);
    
    for(k=0;k<no_iter;k++)
        
    {
        for(i=0;i<n;i++)
        {
            glClear(GL_COLOR_BUFFER_BIT); // clear the screen
            glLineWidth(1.5f);
            glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex2f(0,-1);
            glVertex2f(0,1);
            
            glEnd();
            
            glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex2f(-1,0);
            glVertex2f(1,0);
            
            glEnd();
            
            for(j=0;j<=i;j++)
            {
                glBegin(GL_POINTS);
                if(pc[j].ylabel==1)
                    glColor3f(0,1,0);
                else if(pc[j].ylabel==-1)
                    glColor3f(1,0,0);
                glVertex2f(pc[j].x[1],pc[j].x[2]);
                
                glEnd();
                
            }
            
            updateW(i);
            
            glBegin(GL_LINES);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(0,-w[0]/w[2]);
            glVertex2f(width,-(w[1]/w[2])*width-w[0]/w[2]);
            glEnd();
            glFlush();
            
            sleep(2);
        }
        
        cout<<"Iteration:"<<k<<" is completed "<<endl;
        
        
        glFlush();
    }
    
    
    return;
}

/* Called when window is resized,
 also when window is first created,
 before the first call to display(). */
void
reshape(int w, int h)
{
    /* save new screen dimensions */
  //  width = (GLdouble) w;
   // height = (GLdouble) h;
    
    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    
    /* do an orthographic parallel projection with the coordinate
     system set to first quadrant, limited by screen/window size */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.f, 1.f);
    
    return;
}

void
kbd(unsigned char key, int x, int y)
{
    switch((char)key) {
        case 'q':
        case 27:    /* ESC */
            glutDestroyWindow(wd);
            exit(0);
        default:
            break;
    }
    
    return;
}

int
main(int argc, char *argv[])
{
    
    
    int i, j;
    fprintf(stdout, "Enter the number of points:");
    fscanf(stdin, "%d", &n);
    
    pc = (twoDem *) malloc(sizeof(twoDem) * n);
    
    for (i = 0; i < n; i++)
    {
        fprintf(stdout,"Enter details of pattern %d:\n",i+1);
        pc[i].x[0] = 1;
        for (j = 1; j < NO_OF_PARAMETER; j++)
        {
            
            fprintf(stdout, "Enter the x%d:", j);
            fscanf(stdin, "%f%*c", &(pc[i].x[j]));
            if(j==1)
                pc[i].x[j]/=width;
            else if(j==2)
                pc[i].x[j]/=height;
        }
        fprintf(stdout,"Enter the target label:");
        fscanf(stdin,"%d%*c",&(pc[i].ylabel));
    }
    
    srand((unsigned int)time(NULL));
    for(j=0;j<NO_OF_PARAMETER;j++)
    {
        w[j]=rand()%width;
        w[j]/=width;
    }
    

    /* perform initialization NOT OpenGL/GLUT dependent,
     as we haven't created a GLUT window yet */
    //init();
    
    /* initialize GLUT, let it extract command-line
     GLUT options that you may provide
     - NOTE THE '&' BEFORE argc */
    glutInit(&argc, argv);
    
    /* specify the display to be single
     buffered and color as RGBA values */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    
    /* set the initial window size */
    glutInitWindowSize((int) width, (int) height);
    
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Single Layer Perceptron" /* title */ );
    
    /* --- register callbacks with GLUT --- */
    
    /* register function to handle window resizes */
    glutReshapeFunc(reshape);
    
    /* register keyboard event processing function */
    //glutKeyboardFunc(kbd);
    
    /* register function that draws in the window */
    glutDisplayFunc(display);
    
    /* init GL */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3.0);
    
    /* start the GLUT main loop */
    glutMainLoop();
    
    
    exit(0);
}