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
JNIEXPORT jstring JNICALL
Java_com_ecn_ptam_HelloPTAM_stringFromJNI( JNIEnv* env,
		jobject thiz )
{
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
	return env->NewStringUTF(ss.str().c_str());
}
}
