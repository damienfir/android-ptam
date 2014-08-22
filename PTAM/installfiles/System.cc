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


static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
	            __android_log_print(ANDROID_LOG_INFO, "PTAM", "after %s() glError (0x%x)\n", op, error);
        }
}


System::System(int* size)
    //: mGLWindow(mVideoSource.Size(), "PTAM")
{
    _instance = this;

    GUI.RegisterCommand("exit", GUICommandCallBack, this);
    GUI.RegisterCommand("quit", GUICommandCallBack, this);
    GUI.RegisterCommand("KeyPress", GUICommandCallBack, this);

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
    /* mpARDriver = new ARDriver(*mpCamera, _imsize); */
    /* mpMapViewer = new MapViewer(*mpMap); */

    GUI.ParseLine("GLWindow.AddMenu Menu Menu");
    GUI.ParseLine("Menu.ShowMenu Root");
    GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
    GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
    GUI.ParseLine("DrawAR=0");
    GUI.ParseLine("DrawMap=0");
    GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
    GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");

    _capture = new Capture();

    /* mbDone = false; */
	__android_log_print(ANDROID_LOG_INFO, "PTAM", "system loaded");
};


void System::set_size(int* size)
{
    _imsize = CVD::ImageRef(size[0], size[1]);
    mimFrameBW.resize(_imsize);
    __android_log_print(ANDROID_LOG_INFO, "PTAM", "resized");
}


void System::update_frame(unsigned char* frame, int size)
{
    memcpy(mimFrameBW.data(), frame, _imsize[0]*_imsize[1]);
}


void System::update()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glOrthof(-0.5,(double)_imsize.x - 0.5, (double) _imsize.y - 0.5, -0.5, -1.0, 1.0);

    mpTracker->TrackFrame(mimFrameBW, true); // !bDrawAR && !bDrawMap);

    glPopMatrix();

    draw_center();
    draw_rectangle();
}


void System::draw_center()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(20);
    glColor4f(1,0,0,1);
    float v[] = {0,0};
    glVertexPointer(2, GL_FLOAT, 0, v);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void System::draw_painted()
{
    
}


void System::draw_rectangle()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMultMatrix(mpCamera->MakeUFBLinearFrustumMatrix(0.005, 100));
    glMultMatrix(mpTracker->GetCurrentPose());
     
    std::vector<Vector<3> > rect = _capture->get_rectangle();
    
    if (rect.size() > 0) {
        if (rect.size() < 3) {
            rect.push_back(mpTracker->GetCurrentPose().get_translation());
        }
        else {
            rect.push_back(rect[0]);
        }

        GLfloat* rectf = new float[rect.size()*3];
        for (size_t i = 0; i < rect.size(); ++i) {
            rectf[i*3] = -(GLfloat)rect[i][0];
            rectf[i*3+1] = (GLfloat)rect[i][1];
            rectf[i*3+2] = 0.f;
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glLineWidth(5);
        glColor4f(0,1,0,1);
        glVertexPointer(3, GL_FLOAT, 0, rectf);
        glDrawArrays(GL_LINE_STRIP, 0, rect.size());
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    glPopMatrix();
}


float* se3_to_float(SE3<> tform) {
    Matrix<3,3,double> rot = tform.get_rotation().get_matrix();
    Vector<3,double> trans = tform.get_translation();

    float* mat = new float[16];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            mat[i*4+j] = (float)rot(j,i); // to column-major matrix

    mat[3] = 0.f;
    mat[7] = 0.f;
    mat[11] = 0.f;
    mat[15] = 1.f;
    
    mat[12] = (float)trans[0];
    mat[13] = (float)trans[1];
    mat[14] = (float)trans[2];

    return mat;
}


float* System::get_viewmodel() {
    SE3<> tform = mpTracker->GetCurrentPose().inverse();
    return se3_to_float(tform);
}


float* System::get_modelview()
{
    SE3<> tform = mpTracker->GetCurrentPose();
    return se3_to_float(tform);
}


void System::store_corner()
{
    bool r = _capture->store_corner(mpTracker->GetCurrentPose());
    __android_log_print(ANDROID_LOG_INFO, "PTAM", "storing corner: %i", (int)r);
}


System* System::get_instance()
{
    return _instance;
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams)
{
    System* s = static_cast<System*>(ptr);
    if(sCommand=="quit" || sCommand == "exit") {
        s->mbDone = true;
    }
    if (sCommand == "KeyPress") {
        if (sParams == "Enter") {
            s->store_corner();
        }
    }
}
