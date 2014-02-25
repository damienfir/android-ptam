#include "MapViewer.h"
#include "MapPoint.h"
#include "KeyFrame.h"
#include "LevelHelpers.h"
#include <iomanip>

#include "OpenGL.h"

using namespace CVD;
using namespace std;


MapViewer::MapViewer(Map &map, GLWindow2 &glw):
  mMap(map), mGLWindow(glw)
{
  mse3ViewerFromWorld = 
    SE3<>::exp(makeVector(0,0,2,0,0,0)) * SE3<>::exp(makeVector(0,0,0,0.8 * M_PI,0,0));
}

void MapViewer::DrawMapDots()
{
  SetupFrustum();
  SetupModelView();
  
  int nForMass = 0;
  glColor4f(0,1,1,1);
  glPointSize(3);
  mv3MassCenter = Zeros;
  GLfloat pts[3*mMap.vpPoints.size()];
  GLfloat col[4*mMap.vpPoints.size()];
  for(size_t i=0; i<mMap.vpPoints.size(); i++)
    {
      Vector<3> v3Pos = mMap.vpPoints[i]->v3WorldPos;
      Vector<3> v3Col = gavLevelColors[mMap.vpPoints[i]->nSourceLevel];
      if(v3Pos * v3Pos < 10000)
	{
	  nForMass++;
	  mv3MassCenter += v3Pos;
	}
	  for (int j=0; j<3; j++)
	{
	  pts[3*i+j]= v3Pos[j];
	  col[4*i+j]= v3Col[j];
    }
      col[4*i+3]= 1.0f;
    }
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pts);
  glColorPointer(4, GL_FLOAT, 0, col);
  glDrawArrays(GL_POINTS,0,mMap.vpPoints.size());
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  mv3MassCenter = mv3MassCenter / (0.1 + nForMass);
}


void MapViewer::DrawGrid()
{
  SetupFrustum();
  SetupModelView();
  glLineWidth(1);
  
  GLfloat pts[3*84];
  GLfloat col[4*84];
  
  // Draw a larger grid around the outside..
  double dGridInterval = 0.1;
  
  double dMin = -100.0 * dGridInterval;
  double dMax =  100.0 * dGridInterval;
  
  for(int x=-10;x<=10;x+=1)
    {
      float val;
      if(x==0)
	val=1.0f;
      else
	val=0.3;
	  for (int j=0; j<3; j++)
	{
	  col[8*(x+10)+j]=val;
	  col[8*(x+10)+j+4]=val;
	}
	  col[8*(x+10)+3]=1;
	  col[8*(x+10)+7]=1;

	  pts[6*(x+10)]=x * 10 * dGridInterval;
	  pts[6*(x+10)+1]=dMin;
	  pts[6*(x+10)+3]=x * 10 * dGridInterval;
	  pts[6*(x+10)+4]=dMax;
    }
  for(int y=-10;y<=10;y+=1)
    {
      float val;
      if(y==0)
	val=1.0f;
      else
	val=0.3;
	  for (int j=0; j<3; j++)
	{
	  col[8*21+8*(y+10)+j]=val;
	  col[8*21+8*(y+10)+j+4]=val;
	}
	  col[8*21+8*(y+10)+3]=1;
	  col[8*21+8*(y+10)+7]=1;

	  pts[6*21+6*(y+10)]=dMin;
	  pts[6*21+6*(y+10)+1]=y * 10 * dGridInterval;
	  pts[6*21+6*(y+10)+3]=dMax;
	  pts[6*21+6*(y+10)+4]=y * 10 * dGridInterval;
    }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pts);
  glColorPointer(4, GL_FLOAT, 0, col);
  glDrawArrays(GL_LINES,0,84);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  GLfloat pts2[3*84+3*6];
  GLfloat col2[4*84+4*6];
  dMin = -10.0 * dGridInterval;
  dMax =  10.0 * dGridInterval;
  
  for(int x=-10;x<=10;x++)
    {
      float val;
      if(x==0)
	val=1.0f;
      else
	val=0.5;
      for (int j=0; j<3; j++)
	{
	  col2[8*(x+10)+j]=val;
	  col2[8*(x+10)+j+4]=val;
	}
	  col2[8*(x+10)+3]=1;
	  col2[8*(x+10)+7]=1;
      
      pts2[6*(x+10)]=x * dGridInterval;
	  pts2[6*(x+10)+1]=dMin;
	  pts2[6*(x+10)+3]=x * dGridInterval;
	  pts2[6*(x+10)+4]=dMax;
    }
  for(int y=-10;y<=10;y++)
    {
      float val;
      if(y==0)
	val=1.0f;
      else
	val=0.5;
	  for (int j=0; j<3; j++)
	{
	  col2[8*21+8*(y+10)+j]=val;
	  col2[8*21+8*(y+10)+j+4]=val;
	}
	  col2[8*21+8*(y+10)+3]=1;
	  col2[8*21+8*(y+10)+7]=1;
	  pts2[6*21+6*(y+10)]=dMin;
	  pts2[6*21+6*(y+10)+1]=y * dGridInterval;
	  pts2[6*21+6*(y+10)+3]=dMax;
	  pts2[6*21+6*(y+10)+4]=y * dGridInterval;
    }
  
  col2[8*42]=1.0f;
  col2[8*42+3]=1.0f;
  col2[8*42+4]=1.0f;
  col2[8*42+7]=1.0f;
  pts2[8*42+3]=1.0f;
  col2[8*42+9]=1.0f;
  col2[8*42+11]=1.0f;
  col2[8*42+13]=1.0f;
  col2[8*42+15]=1.0f;
  pts2[8*42+10]=1.0f;
  col2[8*42+16]=1.0f;
  col2[8*42+17]=1.0f;
  col2[8*42+18]=1.0f;
  col2[8*42+19]=1.0f;
  col2[8*42+20]=1.0f;
  col2[8*42+21]=1.0f;
  col2[8*42+22]=1.0f;
  col2[8*42+23]=1.0f;
  pts2[8*42+17]=1.0f;
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pts2);
  glColorPointer(4, GL_FLOAT, 0, col2);
  glDrawArrays(GL_LINES,0,90);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  
//   glColor3f(0.8,0.8,0.8);
//   glRasterPos3f(1.1,0,0);
//   mGLWindow.PrintString("x");
//   glRasterPos3f(0,1.1,0);
//   mGLWindow.PrintString("y");
//   glRasterPos3f(0,0,1.1);
//   mGLWindow.PrintString("z");
}

void MapViewer::DrawMap(SE3<> se3CamFromWorld)
{
  mMessageForUser.str(""); // Wipe the user message clean
  
  // Update viewer position according to mouse input:
  {
    pair<Vector<6>, Vector<6> > pv6 = mGLWindow.GetMousePoseUpdate();
    SE3<> se3CamFromMC;
    se3CamFromMC.get_translation() = mse3ViewerFromWorld * mv3MassCenter;
    mse3ViewerFromWorld = SE3<>::exp(pv6.first) * 
      se3CamFromMC * SE3<>::exp(pv6.second) * se3CamFromMC.inverse() * mse3ViewerFromWorld;
  }

  mGLWindow.SetupViewport();
  glClearColor(0,0,0,0);
  glClearDepthf(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColorMask(1,1,1,1);

  glEnable(GL_DEPTH_TEST);
  DrawGrid();
  DrawMapDots();
  DrawCamera(se3CamFromWorld);
  for(size_t i=0; i<mMap.vpKeyFrames.size(); i++)
    DrawCamera(mMap.vpKeyFrames[i]->se3CfromW, true);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  mMessageForUser << " Map: " << mMap.vpPoints.size() << "P, " << mMap.vpKeyFrames.size() << "KF";
  mMessageForUser << setprecision(4);
  mMessageForUser << "   Camera Pos: " << se3CamFromWorld.inverse().get_translation();
}

string MapViewer::GetMessageForUser()
{
  return mMessageForUser.str();
}

void MapViewer::SetupFrustum()
{
  glMatrixMode(GL_PROJECTION);  
  glLoadIdentity();
  double zNear = 0.03;
  glFrustumf(-zNear, zNear, 0.75*zNear,-0.75*zNear,zNear,50);
  glScalef(1,1,-1);
  return;
};

void MapViewer::SetupModelView(SE3<> se3WorldFromCurrent)
{
  glMatrixMode(GL_MODELVIEW);  
  glLoadIdentity();
  glMultMatrix(mse3ViewerFromWorld * se3WorldFromCurrent);
  return;
};

void MapViewer::DrawCamera(SE3<> se3CfromW, bool bSmall)
{
  
  SetupModelView(se3CfromW.inverse());
  SetupFrustum();
  
  if(bSmall)
    glLineWidth(1);
  else
    glLineWidth(3);
  
  GLfloat pts[] = {
    0,   0,   0,
    0.1f,0,   0,
    0,   0,   0,
    0,   0.1f,0,
    0,   0,   0,
    0,   0,   0.1f
  };
  GLfloat col[] = {
    1,0,0,1,
    1,0,0,1,
    0,1,0,1,
    0,1,0,1,
    1,1,1,1,
    1,1,1,1
  };
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pts);
  glColorPointer(4, GL_FLOAT, 0, col);
  glDrawArrays(GL_LINES,0,6);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  
  if(!bSmall)
  {
	  glLineWidth(1);
	  glColor4f(0.5,0.5,0.5,1.0f);
	  SetupModelView();
	  Vector<2> v2CamPosXY = se3CfromW.inverse().get_translation().slice<0,2>();
      GLfloat col[4*4];
      for (int i=0; i<16; i++)
    {
      col[i]=1.0f;
    }
      GLfloat pts[] = {
        static_cast<GLfloat>(v2CamPosXY[0] - 0.04), static_cast<GLfloat>(v2CamPosXY[1] + 0.04),
        static_cast<GLfloat>(v2CamPosXY[0] + 0.04), static_cast<GLfloat>(v2CamPosXY[1] - 0.04),
        static_cast<GLfloat>(v2CamPosXY[0] - 0.04), static_cast<GLfloat>(v2CamPosXY[1] - 0.04),
        static_cast<GLfloat>(v2CamPosXY[0] + 0.04), static_cast<GLfloat>(v2CamPosXY[1] + 0.04)
      };
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, pts);
      glColorPointer(4, GL_FLOAT, 0, col);
      glDrawArrays(GL_LINES,0,4);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
  }
  
}


