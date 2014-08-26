#include <string.h>
#include <jni.h>
#include <sstream>
#include <stdlib.h>
#include <gvars3/instances.h>
#include "System.h"
#include <android/log.h>

using namespace std;
using namespace GVars3;

#ifndef JNIEXPORT
#define JNIEXPORT
#endif

#ifndef JNICALL
#define JNICALL
#endif


/* return current time in milliseconds */
static double
now_ms(void)
{
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

extern "C"{

JavaVM* jvm;


JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_init( JNIEnv* env, jobject thiz, jintArray size)
{
    env->GetJavaVM(&jvm);
    jvm->AttachCurrentThread(&env, NULL);

    jint* imsize = env->GetIntArrayElements(size, 0);
    new System(imsize);
    delete imsize;
}


JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_send( JNIEnv* env, jobject thiz, jstring command )
{
    const char* c = env->GetStringUTFChars(command, 0);
    GUI.CallCallbacks("KeyPress", string(c));
    env->ReleaseStringUTFChars(command, c);
}

JNIEXPORT jboolean JNICALL Java_com_ecn_ptam_PTAM_mapIsGood( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    jboolean ret = (jboolean)s->map_is_good();
}

JNIEXPORT jboolean JNICALL Java_com_ecn_ptam_PTAM_objectIsGood( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    jboolean ret = (jboolean)s->object_is_good();
}

JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_start( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    s->started = true;
}

JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_stop( JNIEnv* env, jobject thiz )
{
    System* s = System::get_instance();
    s->started = false;
}

JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getModelView( JNIEnv* env, jobject thiz)
{
    System* s = System::get_instance();
    float* mat = s->get_modelview();
    jfloatArray ret = env->NewFloatArray(16);
    env->SetFloatArrayRegion(ret, 0, 16, mat);
    delete mat;
    return ret;
}

JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getCorners( JNIEnv* env, jobject thiz)
{
    System* s = System::get_instance();
    float* array = s->get_corners();
    jfloatArray ret = env->NewFloatArray(4*12);
    env->SetFloatArrayRegion(ret, 0, 4*12, array);
    delete array;
    return ret;
}

JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_update( JNIEnv* env, jobject thiz, jbyteArray array )
{
    System* s = System::get_instance();

    int len = env->GetArrayLength(array);
    unsigned char * y = (unsigned char *) env->GetByteArrayElements(array, 0);
    if (y != NULL) {
        s->update_frame(y, len);
        env->ReleaseByteArrayElements(array, (jbyte*)y, JNI_ABORT);
        delete y;
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "PTAM", "cannot get image");
    }
    s->update();
}

}
