#include <string.h>
#include <jni.h>
#include <sstream>
#include <stdlib.h>
#include <gvars3/instances.h>
#include <System.h>

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

JNIEXPORT void JNICALL
Java_com_ecn_ptam_PTAMActivity_stringFromJNI( JNIEnv* env,
		jobject thiz )
{
	env->GetJavaVM(&jvm);
	std::stringstream ss;

	ss<<"Testing PTAM: "<<std::endl;

	ss << "  Welcome to PTAM " << std::endl;
	ss << "  --------------- " << std::endl;
	ss << "  Parallel tracking and mapping for Small AR workspaces" << std::endl;
	ss << "  Copyright (C) Isis Innovation Limited 2008 " << std::endl;
	ss << std::endl;
	ss << "  Parsing settings.cfg ...." << std::endl;
	GUI.LoadFile("settings.cfg");

	GUI.StartParserThread(); // Start parsing of the console input
	atexit(GUI.StopParserThread);
/*
	try
		{
			System s;
			s.Run();
		}
	catch(CVD::Exceptions::All e)
		{
			ss << endl;
			ss << "!! Failed to run system; got exception. " << std::endl;
			ss << "   Exception was: " << std::endl;
			ss << e.what << std::endl;
		}
*/
}

JNICALL void VideoSource::getSize(int * sizeBuffer) {
	JNIEnv* env;
	jvm->AttachCurrentThread(&env, NULL);
	jclass main = env->FindClass("com/ecn/ptam/PTAMActivity");
	jmethodID getVideoSource = env->GetStaticMethodID(main, "getVideoSource",
			"()Lcom/ecn/ptam/VideoSource;");
	jobject vs = env->CallStaticObjectMethod(main, getVideoSource);
	jclass videosource = env->FindClass("com/ecn/ptam/VideoSource");
	jmethodID getSize = env->GetMethodID(videosource, "getSize", "()[I");
	jintArray result = (jintArray) env->CallObjectMethod(vs, getSize);
	int len = env->GetArrayLength(result);
	env->GetIntArrayRegion(result, 0, len, sizeBuffer);
}

JNICALL void VideoSource::getFrame(CVD::Image<CVD::byte> * imBW,
		CVD::Image<CVD::Rgb<CVD::byte> > * imRGB, int width, int height) {
	JNIEnv* env;
	jvm->AttachCurrentThread(&env, NULL);
	jclass main = env->FindClass("com/ecn/ptam/PTAMActivity");
	jmethodID getVideoSource = env->GetStaticMethodID(main, "getVideoSource",
			"()Lcom/ecn/ptam/VideoSource;");
	jobject vs = env->CallStaticObjectMethod(main, getVideoSource);
	jclass videosource = env->FindClass("com/ecn/ptam/VideoSource");
	jmethodID getFrame = env->GetMethodID(videosource, "getFrame", "()[B");
	jbyteArray array = (jbyteArray) env->CallObjectMethod(vs, getFrame);
	int len = env->GetArrayLength(array);
	jboolean frame_copy;
	unsigned char * y = (unsigned char *) env->GetByteArrayElements(array,
			&frame_copy);
	imBW->resize(mirSize);
	memcpy(imBW->data(), y, width * height);
	imRGB->resize(mirSize);
	unsigned int rowpairs = height / 2;
	unsigned char* rgb = reinterpret_cast<unsigned char*>(imRGB->data());
	int halfwidth = width >> 1;
	int yy, uu, vv, ug_plus_vg, ub, vr;
	int r, g, b;
	unsigned int i, j;
	unsigned char * u = &y[height * width];
	unsigned char * v = &y[width * height + width * rowpairs / 2];
	unsigned char test = ((y[0] << 8) + (v[0] - 128) * 359) >> 8;
	test = ((y[0] << 8) - (u[0] - 128) * 88 - (v[0] - 128) * 183) >> 8;
	test = ((y[0] << 8) + (u[0] - 128) * 454) >> 8;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < halfwidth; ++j) {
			yy = y[0] << 8;
			uu = *(u++) - 128;
			vv = *(v++) - 128;
			ug_plus_vg = uu * 88 + vv * 183;
			ub = uu * 454;
			vr = vv * 359;
			r = (yy + vr) >> 8;
			g = (yy - ug_plus_vg) >> 8;
			b = (yy + ub) >> 8;
			rgb[0] = r < 0 ? 0 : (r > 255 ? 255 : (unsigned char) r);
			rgb[1] = g < 0 ? 0 : (g > 255 ? 255 : (unsigned char) g);
			rgb[2] = b < 0 ? 0 : (b > 255 ? 255 : (unsigned char) b);
			yy = y[1] << 8;
			r = (yy + vr) >> 8;
			g = (yy - ug_plus_vg) >> 8;
			b = (yy + ub) >> 8;
			rgb[3] = r < 0 ? 0 : (r > 255 ? 255 : (unsigned char) r);
			rgb[4] = g < 0 ? 0 : (g > 255 ? 255 : (unsigned char) g);
			rgb[5] = b < 0 ? 0 : (b > 255 ? 255 : (unsigned char) b);
			y += 2;
			rgb += 6;
		}
		if ((i & 1) == 0) {
			u -= halfwidth;
			v -= halfwidth;
		}
	}
}
}
