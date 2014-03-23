# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.ccrg/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../installfiles
LOCAL_MODULE    := PTAM

LOCAL_SRC_FILES += ../installfiles/ARDriver.cc                      \
../installfiles/ATANCamera.cc                       \
../installfiles/Bundle.cc                       \
../installfiles/CalibCornerPatch.cc                      \
../installfiles/CalibImage.cc                      \
../installfiles/CameraCalibrator.cc                      \
../installfiles/EyeGame.cc                      \
../installfiles/GLWindow2.cc                      \
../installfiles/GLWindowMenu.cc                      \
../installfiles/HomographyInit.cc                      \
../installfiles/KeyFrame.cc                      \
../installfiles/Map.cc                      \
../installfiles/MapMaker.cc                      \
../installfiles/MapPoint.cc                      \
../installfiles/MapViewer.cc                      \
../installfiles/MiniPatch.cc                      \
../installfiles/PatchFinder.cc                      \
../installfiles/Relocaliser.cc                      \
../installfiles/ShiTomasi.cc                      \
../installfiles/SmallBlurryImage.cc                      \
../installfiles/System.cc                      \
../installfiles/Tracker.cc                      \

LOCAL_STATIC_LIBRARIES += TooN
LOCAL_STATIC_LIBRARIES += cpufeatures
LOCAL_STATIC_LIBRARIES += cvd
LOCAL_STATIC_LIBRARIES += gvars3
LOCAL_LDLIBS    += -landroid

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES) #export includes
LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS) #export linker cmds
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS) #export c flgs
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS) #export cpp flgs
LOCAL_EXPORT_CXXFLAGS := $(LOCAL_CXXFLAGS) #export cpp flgs

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc
LOCAL_MODULE    := testptam

LOCAL_LDLIBS +=  -llog -ldl
LOCAL_LDLIBS    += -landroid -lGLESv1_CM
LOCAL_SRC_FILES := hello-ptam.cc
LOCAL_SRC_FILES += VideoSource_Android.cc

LOCAL_STATIC_LIBRARIES += PTAM
LOCAL_STATIC_LIBRARIES += cpufeatures

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/cpufeatures)
$(call import-add-path,../ndk-modules)
$(call import-module,cvd)
$(call import-module,gvars3)
$(call import-module,TooN)
