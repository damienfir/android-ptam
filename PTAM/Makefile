all: libptam_built

libptam_built: 
	mkdir -p build
	cp -rup installfiles/* build
	cd build && export CPATH=../../TooN/include && export CPATH=../../cvd/installfiles && export CPATH=../../gvars3/installfiles && ./configure --without-ffmpeg --without-dc1394v1 --without-dc1394v2 --with-lapack
	cd build && export CPATH=../../TooN/include && export CPATH=../../cvd/installfiles && export CPATH=../../gvars3/installfiles && make -j16
	mkdir -p include
	mkdir -p lib
	cp build/*.so* lib
	cp -r build/cvd include
	touch libcvd_built

clean:
	rm -rf include lib
	rm -rf build
	rm -f libcvd_ptam

