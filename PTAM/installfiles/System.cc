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


/*
 * Singleton pattern to recover the unique instance of the System. Used by the JNI interface.
 */
System* System::_instance = NULL;
System* System::get_instance() { return _instance; }


System::System(int* size)
{
    _instance = this;

    GUI.RegisterCommand("exit", GUICommandCallBack, this);
    GUI.RegisterCommand("quit", GUICommandCallBack, this);
    GUI.RegisterCommand("KeyPress", GUICommandCallBack, this);

    Vector<NUMTRACKERCAMPARAMETERS> vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
    mpCamera = new ATANCamera("Camera");

    set_size(size);

    mpMap = new Map;
    mpMapMaker = new MapMaker(*mpMap, *mpCamera);
    mpTracker = new Tracker(_imsize, *mpCamera, *mpMap, *mpMapMaker);

    // manages the camera positions data and the rectangle drawn by the user
    _capture = new Capture();

    started = false;
}


/*
 * Updates the frame size
 */
void System::set_size(int* size)
{
    _imsize = CVD::ImageRef(size[0], size[1]);
    mimFrameBW.resize(_imsize);
}


/*
 * Updates the current frame
 */
void System::update_frame(unsigned char* frame, int size)
{
    memcpy(mimFrameBW.data(), frame, _imsize[0]*_imsize[1]);
}


bool System::map_is_good()
{
    return mpMap->IsGood();
}

bool System::object_is_good()
{
    return _capture->get_rectangle().size() == 4;
}

/*
 * Update step of the PTAM algorithm. It tracks the camera position based on the new frame,
 * and draw some OpenGL content such as image features.
 */
void System::update()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glOrthof(-0.5,(double)_imsize.x - 0.5, (double) _imsize.y - 0.5, -0.5, -1.0, 1.0);

    mpTracker->TrackFrame(mimFrameBW, true); // !bDrawAR && !bDrawMap);

    glPopMatrix();

    // Store position only if the capture process has begun
    if (started) {
        _capture->store_position(mpTracker->GetCurrentPose());
    }

    // Draw user feedback content
    draw_center();
    draw_rectangle();
    draw_painted();
}


/*
 * Projects the point where the light is directed to
 */
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


/*
 * Draw all the points that has been captured, with color information about the distance of the 
 * user to the plane
 */
void System::draw_painted()
{
    std::vector<Vector<3> > prev = _capture->get_positions();
    if (prev.size() == 0) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMultMatrix(mpCamera->MakeUFBLinearFrustumMatrix(0.005, 100));
    glMultMatrix(mpTracker->GetCurrentPose());

    float realdepth = 20.f; // Real depth of the light to the object, given by the user.
    float z0 = prev[0][2]; // Depth at the beginning of the capture process.
    float precision = 1.f; // Error margin. Here we give the user 1cm of error margin.
    float ratio = z0/realdepth;
    float factor; // +1.0 if the user is 1cm to close to the object
                  // -1.0 if the user is 1cm to far from the object
                  // 0.0 if the user is on the sampling plane
    
    GLfloat* v = new float[prev.size()*3];
    GLfloat* c = new float[prev.size()*4];
    for (size_t i = 0; i < prev.size(); ++i) {
        v[i*3] = -(GLfloat)prev[i][0];
        v[i*3+1] = (GLfloat)prev[i][1];
        v[i*3+2] = 0.f;

        factor = max(min(((float)prev[i][2] - z0) / (precision * ratio), 1.f), -1.f);
        c[i*4] = max(-factor, 0.0f);
        c[i*4+1] = 1-abs(factor);
        c[i*4+2] = max(factor, 0.0f);
        c[i*4+3] = 1.f;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glPointSize(10);
    glVertexPointer(3, GL_FLOAT, 0, v);
    glColorPointer(4, GL_FLOAT, 0, c);
    glDrawArrays(GL_POINTS, 0, prev.size());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();

    delete v;
    delete c;
}


/*
 * Draws the rectangle that the user defined, representing the sampling area
 */
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
        } else {
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

        delete rectf;
    }
    
    glPopMatrix();
}


/*
 * Utility function to convert a matrix to a vector of floats (row-major order)
 */
float* se3_to_float(SE3<> tform) {
    Matrix<3,3,double> rot = tform.get_rotation().get_matrix();
    Vector<3,double> trans = tform.get_translation();

    float* mat = new float[16];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            mat[i*4+j] = (float)rot(i,j); // to column-major matrix

    mat[3] = 0.f;
    mat[7] = 0.f;
    mat[11] = 0.f;
    mat[15] = 1.f;
    
    mat[12] = (float)trans[0];
    mat[13] = (float)trans[1];
    mat[14] = (float)trans[2];

    return mat;
}


float* System::get_corners()
{
    std::vector<SE3<> > corners = _capture->get_corners_matrices();
    float* ret = new float[12*4];
    for (size_t i = 0; i < corners.size(); ++i) {
        float* mat = se3_to_float(corners[i]);
        for (int j = 0; j < 12; ++j) {
            ret[i*12+j] = mat[j];
        }
    }
    return ret;
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
