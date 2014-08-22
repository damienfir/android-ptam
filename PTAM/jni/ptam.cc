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

    /* GUI.LoadFile("settings.cfg"); */

    /* GUI.StartParserThread(); // Start parsing of the console input */
    /* atexit(GUI.StopParserThread); */

    /* try { */
    /*     s = System::get_instance(); */
    /*     __android_log_print(ANDROID_LOG_INFO, "System", "running"); */
    /*     s->Run(); */
    /* } */
    /* catch(CVD::Exceptions::All e) { */
    /*     __android_log_print(ANDROID_LOG_INFO, "System", "[exception] can't run"); */
    /*     /1* ss << endl; *1/ */
    /*     /1* ss << "!! Failed to run system; got exception. " << std::endl; *1/ */
    /*     /1* ss << "   Exception was: " << std::endl; *1/ */
    /*     /1* ss << e.what << std::endl; *1/ */
    /* } */
    /* __android_log_print(ANDROID_LOG_INFO, "System", "end"); */

}


JNIEXPORT void JNICALL Java_com_ecn_ptam_PTAM_send( JNIEnv* env, jobject thiz, jstring command )
{
    const char* c = env->GetStringUTFChars(command, 0);
    GUI.CallCallbacks("KeyPress", string(c));
    /* __android_log_print(ANDROID_LOG_INFO, "PTAM", "key pressed"); */
    env->ReleaseStringUTFChars(command, c);
}

/* JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getModelView( JNIEnv* env, jobject thiz) */
/* { */
/*     System* s = System::get_instance(); */
/*     float* mat = s->get_modelview(); */
/*     jfloatArray ret = env->NewFloatArray(16); */
/*     env->SetFloatArrayRegion(ret, 0, 16, mat); */
/*     delete mat; */
/*     return ret; */
/* } */

/* JNIEXPORT jfloatArray JNICALL Java_com_ecn_ptam_PTAM_getViewModel( JNIEnv* env, jobject thiz) */
/* { */
/*     System* s = System::get_instance(); */
/*     float* mat = s->get_viewmodel(); */
/*     jfloatArray ret = env->NewFloatArray(16); */
/*     env->SetFloatArrayRegion(ret, 0, 16, mat); */
/*     delete mat; */
/*     return ret; */
/* } */

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
