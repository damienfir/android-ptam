// Copyright 2008 Isis Innovation Limited
#include "System.h"
#include "OpenGL.h"
#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "ARDriver.h"
#include "MapViewer.h"
#include <android/log.h>

using namespace CVD;
using namespace std;
using namespace GVars3;


System* System::_instance = NULL;


System::System(int* size)
    //: mGLWindow(mVideoSource.Size(), "PTAM")
{
    __android_log_print(ANDROID_LOG_INFO, "System", "creating instance...");
    _instance = this;

    GUI.RegisterCommand("exit", GUICommandCallBack, this);
    GUI.RegisterCommand("quit", GUICommandCallBack, this);

    /*mimFrameBW.resize(mVideoSource.Size());
      mimFrameRGB.resize(mVideoSource.Size());*/
    // First, check if the camera is calibrated.
    // If not, we need to run the calibration widget.
    Vector<NUMTRACKERCAMPARAMETERS> vTest;

    vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
    mpCamera = new ATANCamera("Camera");
    Vector<2> v2;
    /* if(v2==v2) ; */
    if(vTest == ATANCamera::mvDefaultParams) {
        cout << endl;
        cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
        cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
        /*exit(1);*/
    }

    set_size(size);

    mpMap = new Map;
    mpMapMaker = new MapMaker(*mpMap, *mpCamera);
    mpTracker = new Tracker(_imsize, *mpCamera, *mpMap, *mpMapMaker);
    mpARDriver = new ARDriver(*mpCamera, _imsize/*, mGLWindow*/);
    mpMapViewer = new MapViewer(*mpMap/*, mGLWindow*/);

    GUI.ParseLine("GLWindow.AddMenu Menu Menu");
    GUI.ParseLine("Menu.ShowMenu Root");
    GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
    GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
    GUI.ParseLine("DrawAR=0");
    GUI.ParseLine("DrawMap=0");
    GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
    GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");

    /* mbDone = false; */
	__android_log_print(ANDROID_LOG_INFO, "System", "loaded");
};


void System::set_size(int* size)
{
    _imsize = CVD::ImageRef(size[0], size[1]);
    mimFrameBW.resize(_imsize);
    __android_log_print(ANDROID_LOG_INFO, "PTAM", "resized");
}


void System::update_frame(unsigned char* frame, int size)
{
    /* __android_log_print(ANDROID_LOG_INFO, "PTAM", "size = %d", size); */
    memcpy(mimFrameBW.data(), frame, _imsize[0]*_imsize[1]);
}


double* System::update()
{
    // Grab new video frame...
    /* mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW); */
    /* static bool bFirstFrame = true; */
    /* if(bFirstFrame) { */
    /*     mpARDriver->Init(); */
    /*     bFirstFrame = false; */
    /* } */

    /* mGLWindow->SetupViewport(); */
    /* mGLWindow->SetupVideoOrtho(); */
    /* mGLWindow->SetupVideoRasterPosAndZoom(); */

    /* if(!mpMap->IsGood()) */
    /*     mpARDriver->Reset(); */

    /* static gvar3<int> gvnDrawMap("DrawMap", 0, HIDDEN|SILENT); */
    /* static gvar3<int> gvnDrawAR("DrawAR", 0, HIDDEN|SILENT); */

    /* bool bDrawMap = mpMap->IsGood(); // && *gvnDrawMap; */
    /* bool bDrawAR = mpMap->IsGood(); // && *gvnDrawAR; */

    mpTracker->TrackFrame(mimFrameBW, true); // !bDrawAR && !bDrawMap);

    /* if(bDrawMap) */
      /* mpMapViewer->DrawMap(mpTracker->GetCurrentPose()); */
      /* else if(bDrawAR) */
      /*          mpARDriver->Render(mimFrameRGB, mpTracker->GetCurrentPose()); */

    //      mGLWindow.GetMousePoseUpdate();
    /* string sCaption; */
    /* if(bDrawMap) */
    /* sCaption = mpMapViewer->GetMessageForUser(); */
    /* else*//* */
             /* sCaption = mpTracker->GetMessageForUser(); */
    //mGLWindow.DrawCaption(sCaption);
    //mGLWindow.DrawMenus();
    //mGLWindow.swap_buffers();
    //mGLWindow.HandlePendingEvents();*/
    
    /* return get_pose(); */
}


double* System::get_pose()
{
    Vector<3> trans = mpTracker->GetCurrentPose().get_translation();;
    double* pose = new double[2]{trans[0], trans[1]};
    return pose;
}


System* System::get_instance()
{
    /* if (!_instance) { */
    /*     _instance = new System(); */
    /* } */
    return _instance;
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams)
{
    if(sCommand=="quit" || sCommand == "exit")
        static_cast<System*>(ptr)->mbDone = true;
}
