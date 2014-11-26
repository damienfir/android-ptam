#include <string.h>
#include <jni.h>
#include <sstream>
#include <stdlib.h>
#include <gvars3/instances.h>
#include <android/log.h>

#include "PTAM/System.h"

using namespace std;
using namespace GVars3;

#ifndef JNIEXPORT
#define JNIEXPORT
#endif

#ifndef JNICALL
#define JNICALL
#endif



extern "C"{

JavaVM* jvm;


/*
 * Initialize the PTAM environment with the size of the frame
 */
JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_init( JNIEnv* env, jobject thiz, jintArray size)
{
    env->GetJavaVM(&jvm);
    jvm->AttachCurrentThread(&env, NULL);

    jint* imsize = env->GetIntArrayElements(size, 0);
    new System(imsize);
    delete imsize;
}


/*
 * Sends keypress events to PTAM to simulate user interaction
 */
JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_send( JNIEnv* env, jobject thiz, jstring command )
{
    const char* c = env->GetStringUTFChars(command, 0);
    GUI.CallCallbacks("KeyPress", string(c));
    env->ReleaseStringUTFChars(command, c);
}

/*
 * Asks if the map created by PTAM is good and stable
 */
JNIEXPORT jboolean JNICALL Java_com_ecn_ptam_PTAM_mapIsGood( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    jboolean ret = (jboolean)s->map_is_good();
}

/*
 * Asks if the object size has already been defined by the user
 */
JNIEXPORT jboolean JNICALL Java_com_ecn_ptam_PTAM_objectIsGood( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    jboolean ret = (jboolean)s->object_is_good();
}

/*
 * Tells PTAM that the user has begun the capture process
 */
JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_start( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    s->started = true;
}

/*
 * Tells PTAM that the user has stopped the capture process
 */
JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_stop( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    s->started = false;
}

/*
 * Returns the model-view transformation which contains camera position relative to the plane
 * and its rotation.
 */
JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getModelView( JNIEnv* env, jobject thiz)
{
    System* s = System::get_instance();
    float* mat = s->get_modelview();
    jfloatArray ret = env->NewFloatArray(16);
    env->SetFloatArrayRegion(ret, 0, 16, mat);
    delete mat;
    return ret;
}


JNIEXPORT jlong JNICALL Java_com_ecn_ptam_PTAM_getTime( JNIEnv* env, jobject thiz)
{
    System* s = System::get_instance();
    return (jlong)s->time;
}


/*
 * Returns the three object corners that the user defined
 */
JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getCorners( JNIEnv* env, jobject thiz)
{
    System* s = System::get_instance();
    float* array = s->get_corners();
    jfloatArray ret = env->NewFloatArray(4*12);
    env->SetFloatArrayRegion(ret, 0, 4*12, array);
    delete array;
    return ret;
}


/*
 * Updates PTAM with the camera frame that needs to be included in the tracking process.
 *
 * Possible improvement:
 * Instead of sending the frame as an array, the native code can access the texture that contains the video frame. In this manner
 * we won't have to copy the frame in memory, only from GPU memory. Not sure if it's faster though.
 */
JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_update( JNIEnv* env, jobject thiz, jbyteArray array )
{
    System* s = System::get_instance();

    int len = env->GetArrayLength(array);
    unsigned char * y = (unsigned char *) env->GetByteArrayElements(array, 0);
    if (y != NULL) {
        s->update_frame(y, len);
        env->ReleaseByteArrayElements(array, (jbyte*)y, JNI_ABORT);
        /* delete y; */
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "PTAM", "cannot get image");
    }
    s->update();
}

}
