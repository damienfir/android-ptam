// Copyright 2008 Isis Innovation Limited
#define GL_GLEXT_PROTOTYPES 1
#include "ARDriver.h"

using namespace GVars3;
using namespace CVD;
using namespace std;

static bool CheckFramebufferStatus();

ARDriver::ARDriver(const ATANCamera &cam, ImageRef irFrameSize, GLWindow2 &glw)
  :mCamera(cam), mGLWindow(glw)
{
  mirFrameSize = irFrameSize;
  mCamera.SetImageSize(mirFrameSize);
  mbInitialised = false;
}

void ARDriver::Init()
{
  mbInitialised = true;
  mirFBSize = GV3::get<ImageRef>("ARDriver.FrameBufferSize", ImageRef(1200,900), SILENT);
  glGenTextures(1, &mnFrameTex);
  glBindTexture(GL_TEXTURE_CROP_RECT_OES,mnFrameTex);
  glTexImage2D(GL_TEXTURE_CROP_RECT_OES, 0, 
	       GL_RGBA, mirFrameSize.x, mirFrameSize.y, 0, 
	       GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
  MakeFrameBuffer();
  mGame.Init();
};

void ARDriver::Reset()
{
  mGame.Reset();
  mnCounter = 0;
}

void ARDriver::Render(Image<Rgb<byte> > &imFrame, SE3<> se3CfromW)
{
  if(!mbInitialised)
    {
      Init();
      Reset();
    };
  
  mnCounter++;
  
  // Upload the image to our frame texture
  glBindTexture(GL_TEXTURE_CROP_RECT_OES, mnFrameTex);
  glTexSubImage2D(GL_TEXTURE_CROP_RECT_OES,
		  0, 0, 0,
		  mirFrameSize.x, mirFrameSize.y,
		  GL_RGB,
		  GL_UNSIGNED_BYTE,
		  imFrame.data());
  
  // Set up rendering to go the FBO, draw undistorted video frame into BG
  glBindFramebufferOES(GL_FRAMEBUFFER_OES,mnFrameBuffer);
  CheckFramebufferStatus();
  glViewport(0,0,mirFBSize.x,mirFBSize.y);
  DrawFBBackGround();
  glClearDepthf(1);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  // Set up 3D projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrix(mCamera.MakeUFBLinearFrustumMatrix(0.005, 100));
  glMultMatrix(se3CfromW);
  
  DrawFadingGrid();
  
  mGame.DrawStuff(se3CfromW.inverse().get_translation());
  
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Set up for drawing 2D stuff:
  glBindFramebufferOES(GL_FRAMEBUFFER_OES,0);
  
  DrawDistortedFB();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  mGLWindow.SetupViewport();
  mGLWindow.SetupVideoOrtho();
  mGLWindow.SetupVideoRasterPosAndZoom();
}



void ARDriver::MakeFrameBuffer()
{
  // Needs nvidia drivers >= 97.46
  cout << "  ARDriver: Creating FBO... ";
  
  glGenTextures(1, &mnFrameBufferTex);
  glBindTexture(GL_TEXTURE_CROP_RECT_OES,mnFrameBufferTex);
  glTexImage2D(GL_TEXTURE_CROP_RECT_OES, 0, 
	       GL_RGBA, mirFBSize.x, mirFBSize.y, 0, 
	       GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  GLuint DepthBuffer;
  glGenRenderbuffersOES(1, &DepthBuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, DepthBuffer);
  glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, mirFBSize.x, mirFBSize.y);

  glGenFramebuffersOES(1, &mnFrameBuffer);
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, mnFrameBuffer); 
  glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, 
			    GL_TEXTURE_CROP_RECT_OES, mnFrameBufferTex, 0);
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, 
  			       GL_RENDERBUFFER_OES, DepthBuffer);
  
  CheckFramebufferStatus();
  cout << " .. created FBO." << endl;
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
}

static bool CheckFramebufferStatus()         
{                                            
  
  GLenum n;                                            
  n = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
  if(n == GL_FRAMEBUFFER_COMPLETE_OES)
    return true; // All good
  
  cout << "glCheckFrameBufferStatusOes returned an error." << endl;
  return false;
};

void ARDriver::DrawFBBackGround()
{
  static GLuint nList;
  mGLWindow.SetupUnitOrtho();
  
  //glDisable(GL_POLYGON_SMOOTH);
  glEnable(GL_TEXTURE_CROP_RECT_OES);
  glBindTexture(GL_TEXTURE_CROP_RECT_OES, mnFrameTex);  
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glDisable(GL_BLEND);

  // How many grid divisions in the x and y directions to use?
  int nStepsX = 24; // Pretty arbitrary..
  int nStepsY = (int) (nStepsX * ((double) mirFrameSize.x / mirFrameSize.y)); // Scaled by aspect ratio
  if(nStepsY < 2)
  nStepsY = 2;
  for(int ystep = 0; ystep< nStepsY; ystep++)
{  
  GLfloat col[16];
  GLfloat vtx1[8];
  GLfloat vtx2[8];
  GLfloat tex1[8];
  GLfloat tex2[8];
  int count = 0;
  for (int i = 0; i<16; i++)
    col[i] = 1.0f;
  for(int xstep = 0; xstep <= nStepsX; xstep++)
  {
	for(int yystep = ystep; yystep<=ystep+1; yystep++) // Two y-coords in one go - magic.
	  {
	Vector<2> v2Iter;
	v2Iter[0] = (double) xstep / nStepsX;
	v2Iter[1] = (double) yystep / nStepsY;
	// If this is a border quad, draw a little beyond the
	// outside of the frame, this avoids strange jaggies
	// at the edge of the reconstructed frame later:
	if(xstep == 0 || yystep == 0 || xstep == nStepsX || yystep == nStepsY)
	  for(int i=0; i<2; i++)
		v2Iter[i] = v2Iter[i] * 1.02 - 0.01; 
	Vector<2> v2UFBDistorted = v2Iter; 
	Vector<2> v2UFBUnDistorted = mCamera.UFBLinearProject(mCamera.UFBUnProject(v2UFBDistorted));
	tex1[4*(xstep % 2)+2*(yystep-ystep)] = v2UFBDistorted[0] * mirFrameSize.x;
	tex1[4*(xstep % 2)+2*(yystep-ystep)+1] = v2UFBDistorted[1] * mirFrameSize.y;
	tex2[(4*(xstep % 2)+2*(yystep-ystep)+4) % 8] = v2UFBDistorted[0] * mirFrameSize.x;
	tex2[(4*(xstep % 2)+2*(yystep-ystep)+5) % 8] = v2UFBDistorted[1] * mirFrameSize.y;
	vtx1[4*(xstep % 2)+2*(yystep-ystep)] = v2UFBUnDistorted[0];
	vtx1[4*(xstep % 2)+2*(yystep-ystep)+1] = v2UFBUnDistorted[1];
	vtx2[(4*(xstep % 2)+2*(yystep-ystep)+4) % 8] = v2UFBUnDistorted[0];
	vtx2[(4*(xstep % 2)+2*(yystep-ystep)+5) % 8] = v2UFBUnDistorted[1];
	  }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, col);
    if (xstep % 2 == 1)
      {
    glVertexPointer(2, GL_FLOAT, 0, vtx1);
    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
      } else if (xstep != 0) {
    glVertexPointer(2, GL_FLOAT, 0, vtx2);
    glTexCoordPointer(2, GL_FLOAT, 0, vtx2);
      }
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
}
  glDisable(GL_TEXTURE_CROP_RECT_OES);
}


void ARDriver::DrawDistortedFB()
{
  static bool bFirstRun = true;
  static GLuint nList;
  mGLWindow.SetupViewport();
  mGLWindow.SetupUnitOrtho();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glDisable(GL_POLYGON_SMOOTH);
  glEnable(GL_TEXTURE_CROP_RECT_OES);
  glBindTexture(GL_TEXTURE_CROP_RECT_OES, mnFrameBufferTex);  
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CROP_RECT_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glDisable(GL_BLEND);

  // How many grid divisions in the x and y directions to use?
  int nStepsX = 24; // Pretty arbitrary..
  int nStepsY = (int) (nStepsX * ((double) mirFrameSize.x / mirFrameSize.y)); // Scaled by aspect ratio
  if(nStepsY < 2)
nStepsY = 2;
  for(int ystep = 0; ystep<nStepsY; ystep++)
{  
  GLfloat col[16];
  GLfloat vtx1[8];
  GLfloat vtx2[8];
  GLfloat tex1[8];
  GLfloat tex2[8];
  int count = 0;
  for (int i = 0; i<16; i++)
    col[i] = 1.0f;
  for(int xstep = 0; xstep<=nStepsX; xstep++)
  {
	for(int yystep = ystep; yystep<=ystep + 1; yystep++) // Two y-coords in one go - magic.
	  {
	Vector<2> v2Iter;
	v2Iter[0] = (double) xstep / nStepsX;
	v2Iter[1] = (double) yystep / nStepsY;
	Vector<2> v2UFBDistorted = v2Iter; 
	Vector<2> v2UFBUnDistorted = mCamera.UFBLinearProject(mCamera.UFBUnProject(v2UFBDistorted));
	tex1[4*(xstep % 2)+2*(yystep-ystep)] = v2UFBUnDistorted[0] * mirFBSize.x;
	tex1[4*(xstep % 2)+2*(yystep-ystep)+1] = (1.0 - v2UFBUnDistorted[1]) * mirFBSize.y;
	tex2[(4*(xstep % 2)+2*(yystep-ystep)+4) % 8] = v2UFBUnDistorted[0] * mirFBSize.x;
	tex2[(4*(xstep % 2)+2*(yystep-ystep)+5) % 8] = (1.0 - v2UFBUnDistorted[1]) * mirFBSize.y;
	vtx1[4*(xstep % 2)+2*(yystep-ystep)] = v2UFBDistorted[0];
	vtx1[4*(xstep % 2)+2*(yystep-ystep)+1] = v2UFBDistorted[1];
	vtx2[(4*(xstep % 2)+2*(yystep-ystep)+4) % 8] = v2UFBDistorted[0];
	vtx2[(4*(xstep % 2)+2*(yystep-ystep)+5) % 8] = v2UFBDistorted[1];
	  }	 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, col);
    if (xstep % 2 == 1)
      {
    glVertexPointer(2, GL_FLOAT, 0, vtx1);
    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
      } else if (xstep != 0) {
    glVertexPointer(2, GL_FLOAT, 0, vtx2);
    glTexCoordPointer(2, GL_FLOAT, 0, vtx2);
      }
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
}
  glDisable(GL_TEXTURE_CROP_RECT_OES);
}

void ARDriver::DrawFadingGrid()
{
  double dStrength;
  if(mnCounter >= 60)
    return;
  if(mnCounter < 30)
    dStrength = 1.0;
  dStrength = (60 - mnCounter) / 30.0;
  
  glColor4f(1,1,1,dStrength);
  int nHalfCells = 8;
  if(mnCounter < 8)
    nHalfCells = mnCounter + 1;
  int nTot = nHalfCells * 2 + 1;
  Vector<3>  aaVertex[17][17];
  for(int i=0; i<nTot; i++)
    for(int j=0; j<nTot; j++)
      {
	Vector<3> v3;
	v3[0] = (i - nHalfCells) * 0.1;
	v3[1] = (j - nHalfCells) * 0.1;
	v3[2] = 0.0;
	aaVertex[i][j] = v3;
      }

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(2);
  for(int i=0; i<nTot; i++)
    {
      GLfloat vtx1[3*nTot];
      for(int j=0; j<nTot; j++)
	    for(int k=0; k<3; k++)
	      vtx1[3*j+k] = aaVertex[i][j][k];
	  glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, vtx1);
      glDrawArrays(GL_LINE_STRIP,0,nTot);
      glDisableClientState(GL_VERTEX_ARRAY);
      
      GLfloat vtx2[3*nTot];
      for(int j=0; j<nTot; j++)
	    for(int k=0; k<3; k++)
	      vtx2[3*j+k] = aaVertex[j][i][k];
	  glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(2, GL_FLOAT, 0, vtx2);
      glDrawArrays(GL_LINE_STRIP,0,nTot);
      glDisableClientState(GL_VERTEX_ARRAY);
    };
};







