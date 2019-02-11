How to Compile FFmpeg
======

The way to compile this modified project is the same as the way to compile the original one.

For completely information, see https://trac.ffmpeg.org/wiki/CompilationGuide.

To simply the mass information, if you just want to compile FFmpeg as a static executable binary file, reference the following instructions.

## Compile FFmpeg for Linux on Linux

1. Put this script in the root directory of the source code.

	```bash
	#!/bin/bash
	
	export PREFIX=$(pwd)/build
	export CC=clang
	export CXX=clang++
	
	export LDFLAGS=" -L${PREFIX}/lib -L${PREFIX}/usr/local/lib"
	export CFLAGS="-DNDEBUG -O3 -I${PREFIX}/include"
	
	export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
	
	./configure \
	    --prefix="${PREFIX}" \
	    --enable-static \
	    --disable-doc \
	    --disable-ffplay \
	    --disable-ffprobe \
	    --target-os=linux \
	    --cc=${CC} \
	    --cxx=${CXX} \
	    --extra-ldflags="-fPIE -pie" \
	    --pkg-config="$(which pkg-config)" \
	    --disable-debug \
	    --disable-shared \
	    --enable-hardcoded-tables \
	    --enable-pic \
	    --enable-version3 \
	    --enable-pthreads \
	    || exit 1
	
	make clean
	make -j$(nproc) || exit 1
	make install || exit 1
	```
	
	You may need to install the missing libraries or tools in order to successfully run the script.

2. Compile Libraries

	You may want to enable the third-party library supports. For example, to enable `libopus`, you should follow the instructions of compiling `libopus`. And in order to generate a **static** executable FFmpeg file, you also need to compile the third-party library **statically**, that means you would get the library as a ".a" file (or ".a" files) eventually. Install the library into your FFmpeg's `PREFIX` directory.
	
	An example for compiling a library which uses `GNU make` as its building tool,
	
	```bash
	#!/bin/bash
	
	export PREFIX=/path/to/ffmpeg/prefix
	export CC=clang
	export CXX=clang++
	
	export LDFLAGS=" -L${PREFIX}/lib -L${PREFIX}/usr/local/lib"
	export CFLAGS="-DNDEBUG -O3 -I${PREFIX}/include"
	
	export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
	
	# autoreconf -i  # You may need to run this before configuring.
	
	./configure \
	    --with-pic \
	    --disable-shared \
	    --enable-static \
	    --prefix="${PREFIX}" \
	    || exit 1
	    
	make clean
	make -j$(nproc) || exit 1
	make install || exit 1
	```
	
	An example for `cmake`,
	
	```bash
    #!/bin/bash
    
    export PREFIX=/path/to/ffmpeg/prefix
    export CC=clang
    export CXX=clang++
    
	export LDFLAGS=" -L${PREFIX}/lib -L${PREFIX}/usr/local/lib"
	export CFLAGS="-DNDEBUG -O3 -I${PREFIX}/include"
	
	export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
    
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
        -DCMAKE_POSITION_INDEPENDENT_CODE=on \
        -DBUILD_SHARED_LIBS=off \
        -DBUILD_STATIC_LIBS=on \
        .. \
        || exit 1
	```
	
3. Enable Libraries

	After compiling all libraries that you need, enable them when you **configure** FFmpeg. And you had better add all third-party libraries into `extra-libs`. After doing that, taking `libopus` for example, your script would be,
	
	```bash
	#!/bin/bash
	
	export PREFIX=$(pwd)/build
	export CC=clang
	export CXX=clang++
	
	export LDFLAGS=" -L${PREFIX}/lib -L${PREFIX}/usr/local/lib"
	export CFLAGS="-DNDEBUG -O3 -I${PREFIX}/include"
	
	export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
	
	./configure \
	    --prefix="${PREFIX}" \
	    --enable-static \
	    --disable-doc \
	    --disable-ffplay \
	    --disable-ffprobe \
	    --target-os=linux \
	    --cc=${CC} \
	    --cxx=${CXX} \
	    --extra-ldflags="-fPIE -pie" \
	    --extra-libs="-lopus -lm" \
	    --pkg-config="$(which pkg-config)" \
	    --disable-debug \
	    --disable-shared \
	    --enable-hardcoded-tables \
	    --enable-pic \
	    --enable-version3 \
	    --enable-pthreads \
	    --enable-libopus \
	    || exit 1
	
	make clean
	make -j$(nproc) || exit 1
	make install || exit 1
	```
	
4. Libraries with Different Licenses

	FFmpeg defaults on LGPL. If you want to enable libraries which is under GPL (such as libx264, libx265), you have to add the `--enable-gpl` option for the **configure** script. If you want to enable non-free libraries (such as libfdk-aac), you have to add the `--enable-nonfree` option.
	
## Compile FFmpeg for Android on Linux

You had better compile FFmpeg for Linux on Linux first to quickly make sure your build script can work. Because Android NDK r18+ has removed `gcc` compiler, use `clang` as possible as you can.

Then, follow the steps to convert your build-for-linux script for Linux into a build-for-android script.

(If you want to compile FFmpeg for Android x86, make sure you disable the NASM or YASM for FFmpeg and other libraries or your executable file will not be executable)

1. Look over the [documentation of Android NDK](https://developer.android.com/ndk/guides/).

2. Run the `make-standalone-toolchain.sh` script to build a Android sysroot.

	The command is something like,
	
	```bash
	export ARCH=arm64
	make-standalone-toolchain.sh" --use-llvm --arch=${ARCH} --stl=libc++ --platform=android-21 --install-dir="${PREFIX}"
	```

3. Change environment variables for the toolchain in the Android sysroot.

	For example,
	
	```bash
	export NDK_TOOLCHAIN_ABI="arm-linux-androideabi"
	export LIB_ARCH_NAME="arm64-v8a"
	export CROSS_PREFIX=${PREFIX}/bin/${NDK_TOOLCHAIN_ABI}-
	export SYSROOT=${PREFIX}/sysroot
	
	export CC="${CROSS_PREFIX}clang"
	export CXX="${CROSS_PREFIX}clang++"
	export CPP="${CROSS_PREFIX}cpp"
	export LD="${CROSS_PREFIX}ld"
	export CCLD="${CROSS_PREFIX}ld "
	export CXXLD="${CROSS_PREFIX}ld"
	export RANLIB="${CROSS_PREFIX}ranlib"
	export STRIP="${CROSS_PREFIX}strip"
	export READELF="${CROSS_PREFIX}readelf"
	export OBJDUMP="${CROSS_PREFIX}objdump"
	export ADDR2LINE="${CROSS_PREFIX}addr2line"
	export AR="${CROSS_PREFIX}ar"
	export AS="${CROSS_PREFIX}clang"
	export OBJCOPY="${CROSS_PREFIX}objcopy"
	export ELFEDIT="${CROSS_PREFIX}elfedit"
	export DWP="${CROSS_PREFIX}dwp"
	export GCONV="${CROSS_PREFIX}gconv"
	export GDP="${CROSS_PREFIX}gdb"
	export GPROF="${CROSS_PREFIX}gprof"
	export NM="${CROSS_PREFIX}nm"
	export SIZE="${CROSS_PREFIX}size"
	export STRINGS="${CROSS_PREFIX}strings"
	
	export LDFLAGS="${EXTRA_LD_FLAGS} -L${PREFIX}/lib -L${PREFIX}/sdk/native/libs/${LIB_ARCH_NAME} -L${PREFIX}/sdk/native/staticlibs/${LIB_ARCH_NAME} -L${PREFIX}/sdk/native/3rdparty/libs/${LIB_ARCH_NAME} -L${SYSROOT}/usr/lib "
	export CFLAGS="-DNDEBUG -DANDROID -O3 -I${PREFIX}/include -I${SYSROOT}/usr/include -I${PREFIX}/sdk/native/jni/include"
	```
	
4. Modify the **configure** Script for FFmpeg

	Change `--target-os=linux` to `--target-os=android`. Add `--cross-prefix`, `--arch`, `--enable-cross-compile`, `--sysroot`, `--as` options.

	For example,

	```bash
	./configure \
	    --prefix="${PREFIX}" \
	    --enable-static \
	    --disable-doc \
	    --disable-ffplay \
	    --disable-ffprobe \
	    --cross-prefix="${CROSS_PREFIX}" \
        --target-os=android \
        --arch=${ARCH} \
        --enable-cross-compile \
        --sysroot="${SYSROOT}" \
	    --cc=${CC} \
	    --cxx=${CXX} \
	    --as=${AS} \
	    --extra-ldflags="-fPIE -pie" \
	    --extra-libs="-lopus -lm" \
	    --pkg-config="$(which pkg-config)" \
	    --disable-debug \
	    --disable-shared \
	    --enable-hardcoded-tables \
	    --enable-pic \
	    --enable-version3 \
	    --enable-pthreads \
	    --enable-libopus \
	    || exit 1
	```
	
5. Modify the Build Script for Third-party Libraries

	For GNU make, you can add the `--host` option.
	
	```bash
	./configure \
	    --with-pic \
	    --host="${NDK_TOOLCHAIN_ABI}" \
	    --disable-shared \
	    --enable-static \
	    --prefix="${PREFIX}" \
	    || exit 1
	```
	
	For `cmake`, you need to assign a toolchain file which you could find in your Android NDK folder and set `ANDROID_TOOLCHAIN` to `clang`, `ANDROID_ABI` to ${ARCH_NAME}. You may need to set `ANDROID_STL`.
	
	```bash
	cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
        -DCMAKE_TOOLCHAIN_FILE="android.toolchain.cmake" \
        -DCMAKE_POSITION_INDEPENDENT_CODE=on \
        -DANDROID_TOOLCHAIN="clang" \
        -DANDROID_ABI="${ARCH_NAME}" \
        -DBUILD_SHARED_LIBS=off \
        -DBUILD_STATIC_LIBS=on \
        .. \
        || exit 1
	```
	
## Why can't I compile successfully?

Many people use build scripts shared by other developers but they still don't work. That's mainly because your environment are not totally the same. It would be better to run the build scripts and commands on a virtual machine of which operating system is just installed.