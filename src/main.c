#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLFWwindow* window = 0;
int endflag = 0;

double xPoints[4], yPoints[4];
int bezierSegments = 100;

double currDisplayedT = 0.5;
double currTime = 0;

void keyCallback(GLFWwindow* window,
                 int key,
                 int scancode,
                 int action,
                 int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    endflag = 1;
  }
}

void initGL() {
  glfwInit();
  window = glfwCreateWindow(800, 600, "Bezier", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyCallback);
  glClearColor(0, 0, 0, 1);
}

void initBezierPoints() {
  int ii = 0;
  time_t t;
  srand((unsigned) time(&t));
  for (; ii < sizeof(xPoints)/sizeof(double); ++ii) {
    xPoints[ii] =  2*(double)rand()/(double)RAND_MAX - 1;
    yPoints[ii] =  2*(double)rand()/(double)RAND_MAX - 1;
  }

  xPoints[0] = 0.0;
  yPoints[0] = 0.0;
  xPoints[1] = 0.5;
  yPoints[1] = 0.5;
}

double lerp(double p0, double p1, double t) {
  return p0+(p1 - p0)*t;
  //return (1-t)*p0 + t*p1;
}

typedef struct {
  double x, y;
} Vec2d;

Vec2d getBezierPoint(double* xPoints, double* yPoints, int numPoints, double t) {
  int ii = 0;
  // Create temporary storage for points
  double* xTemp = (double*) malloc(sizeof(double)*numPoints);
  double* yTemp = (double*) malloc(sizeof(double)*numPoints);
  // Initialise temp points
  for (; ii < numPoints; ++ii) {
    xTemp[ii] = xPoints[ii];
    yTemp[ii] = yPoints[ii];
  }

  ii = 0;
  // Set temp points to lerp values
  for (; ii < numPoints; ++ii) {
    int jj = 0;
    for (; jj < numPoints - ii - 1; ++jj) {
      xTemp[jj] = lerp(xTemp[jj], xTemp[jj+1], t);
      yTemp[jj] = lerp(yTemp[jj], yTemp[jj+1], t);
    }
  }

  Vec2d result;
  result.x = xTemp[0];
  result.y = yTemp[0];

  free(xTemp);
  free(yTemp);

  return result;
}

int main(int argc, char** argv) {
  initGL();
  initBezierPoints();

  while (!endflag) {
    glClear(GL_COLOR_BUFFER_BIT);

    int ii = 0;
    int numPoints = sizeof(xPoints)/sizeof(double);

    glColor3f(1,1,1);
    // Draw the curve
    glBegin(GL_LINE_STRIP);
    for (; ii < bezierSegments; ++ii) {
      double t = (double)ii/(double)bezierSegments;
      Vec2d point = getBezierPoint(xPoints, yPoints, numPoints, t);
      glVertex2d(point.x, point.y);
    }
    glEnd();


    glColor3f(0, 1, 0);
    // Create temporary memory for points
    double* xTemp = (double*) malloc(sizeof(double)*numPoints);
    double* yTemp = (double*) malloc(sizeof(double)*numPoints);
    ii = 0;
    // Initialise temporary points
    for (; ii < numPoints; ++ii) {
      xTemp[ii] = xPoints[ii];
      yTemp[ii] = yPoints[ii];
    }
    // Begin drawing the lines
    glBegin(GL_LINES);
    ii = 0;
    for (; ii < numPoints; ++ii) {
      int jj = 0;
      for (; jj < numPoints - ii - 1; ++jj) {
        glVertex2d(xTemp[jj], yTemp[jj]);
        glVertex2d(xTemp[jj+1], yTemp[jj+1]);
        xTemp[jj] = lerp(xTemp[jj], xTemp[jj+1], currDisplayedT);
        yTemp[jj] = lerp(yTemp[jj], yTemp[jj+1], currDisplayedT);
      }
    }
    glEnd();

    glColor3f(1, 0, 0);
    glPointSize(10);
    // Draw the points of the lerp values
    // Initialise temporary points
    ii = 0;
    for (; ii < numPoints; ++ii) {
      xTemp[ii] = xPoints[ii];
      yTemp[ii] = yPoints[ii];
    }
    // Begin drawing the points
    ii = 0;
    glBegin(GL_POINTS);
    for (; ii < numPoints; ++ii) {
      int jj = 0;
      for (; jj < numPoints - ii; ++jj) {
        glVertex2d(xTemp[jj], yTemp[jj]);
        xTemp[jj] = lerp(xTemp[jj], xTemp[jj+1], currDisplayedT);
        yTemp[jj] = lerp(yTemp[jj], yTemp[jj+1], currDisplayedT);
      }
    }

    glEnd();
       
    free(xTemp);
    free(yTemp);

    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) {
      endflag = 1;
    }
    // Increment time, set currT
    currTime += 0.05;
    currDisplayedT = (1+sin(currTime))/2;
  }

	return 0;
}

