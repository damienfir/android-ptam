// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited

#ifndef __CAMERACALIBRATOR_H
#define __CAMERACALIBRATOR_H
#include <gvars3/gvars3.h>
#include <vector>

#include "../PTAM/CalibImage.h"
/* #include "../PTAM/GLWindow2.h" */
/* #include "../PTAM/VideoSource.h" */


class CameraCalibrator
{
public:
    static CameraCalibrator* get_instance();
    static CameraCalibrator* _instance;

  CameraCalibrator(int* size);
  void update(unsigned char* frame, int size);
  void set_size(int* size);
  
protected:
  void Reset();
  void HandleFrame(CVD::Image<CVD::byte> imFrame);
  static void MainLoopCallback(void* pvUserData);
  void MainLoopStep();
  /* VideoSource mVideoSource; */
  
  /* GLWindow2 mGLWindow; */
  ATANCamera mCamera;
  bool mbDone;

  CVD::ImageRef _imsize;

    CVD::Image<CVD::byte> imFrameBW;

  std::vector<CalibImage> mvCalibImgs;
  void OptimizeOneStep();
  
  bool mbGrabNextFrame;
  GVars3::gvar3<int> mgvnOptimizing;
  GVars3::gvar3<int> mgvnShowImage;
  GVars3::gvar3<int> mgvnDisableDistortion;
  double mdMeanPixelError;

  void GUICommandHandler(std::string sCommand, std::string sParams);
  static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);
};

#endif
