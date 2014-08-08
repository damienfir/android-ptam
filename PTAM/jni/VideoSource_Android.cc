#include <jni.h>
#include "VideoSource.h"
#include "cvd/colourspace.h"
#include <android/log.h>

#ifndef JNIEXPORT
#define JNIEXPORT
#endif

#ifndef JNICALL
#define JNICALL
#endif

using namespace CVD;
using namespace std;


JNICALL VideoSource::VideoSource()
{
    int buf[2];
    getSize(buf);
    int width = buf[0];
    int height = buf[1];
    __android_log_print(ANDROID_LOG_INFO, "videosource1", "%d", width);
    __android_log_print(ANDROID_LOG_INFO, "videosource1", "%d", height);
	ImageRef size =
    ImageRef(width, height);
    __android_log_print(ANDROID_LOG_INFO, "videosource2", "%d", size.x);
    __android_log_print(ANDROID_LOG_INFO, "videosource2", "%d", size.y);
	mirSize = size;
    __android_log_print(ANDROID_LOG_INFO, "videosource3", "%d", mirSize.x);
    __android_log_print(ANDROID_LOG_INFO, "videosource3", "%d", mirSize.y);
}


ImageRef VideoSource::Size()
{
  return mirSize;
};

void JNICALL VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW)
{
	__android_log_print(ANDROID_LOG_INFO, "GetAndFillFrameBWandRGB", "1");
	getFrame(&imBW, mirSize.x, mirSize.y);
}
