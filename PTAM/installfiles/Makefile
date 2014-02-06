# DO NOT DELETE THIS LINE -- make depend depends on it.


# Edit the lines below to point to any needed include and link paths
# Or to change the compiler's optimization flags
CC = g++ -arch i386
COMPILEFLAGS = ${CPPFLAGS} -D_LINUX -D_REENTRANT -Wall  -O3 -march=nocona -msse3 -I/opt/local/include -I/usr/local/include -I/opt/X11/include 
LINKFLAGS = ${LDFLAGS}  -lblas -llapack -lGVars3 -lcvd -L/usr/X11/lib -L/opt/X11/lib -L/usr/local/lib -framework OpenGL

OBJECTS=	main.o\
		VideoSource_OSX.o\
		GLWindow2.o\
		GLWindowMenu.o\
		System.o \
		ATANCamera.o\
		KeyFrame.o\
		MapPoint.o\
		Map.o\
		SmallBlurryImage.o\
		ShiTomasi.o \
		MapMaker.o \
		Bundle.o \
		PatchFinder.o\
		HomographyInit.o\
		Relocaliser.o\
		MiniPatch.o\
		MapViewer.o\
		ARDriver.o\
		EyeGame.o\
		Tracker.o 

CALIB_OBJECTS=	GLWindow2.o\
		GLWindowMenu.o\
		VideoSource_OSX.o \
		CalibImage.o \
		CalibCornerPatch.o\
		ATANCamera.o \
		CameraCalibrator.o 

All: PTAM CameraCalibrator

PTAM:$(OBJECTS)
	$(CC) -o PTAM $(OBJECTS) $(LINKFLAGS)
	/Developer/Tools/Rez OSX_resource_file.r -o PTAM
CameraCalibrator:$(CALIB_OBJECTS)
	$(CC) -o CameraCalibrator $(CALIB_OBJECTS) $(LINKFLAGS)
	/Developer/Tools/Rez OSX_resource_file.r -o CameraCalibrator

%.o: %.cc
	$(CC) $< -o $@ -c $(COMPILEFLAGS)

clean:
	rm *.o


depend:
	rm dependecies; touch dependencies
	makedepend -fdependencies $(INCLUDEFLAGS) $(MOREINCS) *.cc *.h

-include dependencies
