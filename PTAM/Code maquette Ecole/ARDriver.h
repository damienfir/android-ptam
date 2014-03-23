// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
// ARDriver.h
// This file declares the ARDriver class
//
// ARDriver provides basic graphics services for drawing augmented
// graphics. It manages the OpenGL setup and the camera's radial
// distortion so that real and distorted virtual graphics can be
// properly blended.
//
#ifndef __AR_Driver_H
#define __AR_Driver_H
#include <TooN/se3.h>
#include "ATANCamera.h"
#include "GLWindow2.h"
#include "OpenGL.h"
#include <cvd/image.h>
#include <cvd/rgb.h>
#include <cvd/byte.h>
#include "EyeGame.h"
///Utilisation du header batiment.h
#include "batiment.h"

using namespace std;
using namespace CVD;

class ARDriver
{
 public:
  ARDriver(const ATANCamera &cam, ImageRef irFrameSize, GLWindow2 &glw);
  void Render(Image<Rgb<byte> > &imFrame, SE3<> se3CamFromWorld);
  void Reset();
  void Init();
  ///Fonctions de zoom
  void ZoomUp();
  void ZoomDown();
  void Rotate();

 protected:
  ATANCamera mCamera;
  GLWindow2 &mGLWindow;
  void DrawFadingGrid();
  void MakeFrameBuffer();
  void DrawFBBackGround();
  void DrawDistortedFB();
  void SetFrustum();


  // Texture stuff:
  GLuint mnFrameBuffer;
  GLuint mnFrameBufferTex;
  GLuint mnFrameTex;

  int mnCounter;
  ImageRef mirFBSize;
  ImageRef mirFrameSize;
  SE3<> mse3;
  bool mbInitialised;

  ///La classe ARDriver contient les structures
  Batiment mBatiment;
};
#endif
