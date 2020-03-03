# ffmpeg的精简

参考博客

<https://blog.csdn.net/seven407/article/details/6170823>

第一种命令

```
./configure --enable-shared --disable-static --enable-memalign-hack --disable-encoders \
--disable-decoders --enable-decoder=h264
make 
make install

```



第二种命令 加上--enable-small

```
./configure --enable-shared --disable-static --enable-memalign-hack --disable-encoders \
--disable-decoders --enable-decoder=h264 --enable-small
```



第三种命令进一步裁减

```
--disable-avdevice --disable-avfilter --disable-network --disable-everything 
--enable-decoder=需要解码的codec格式
--enable-decoder=xxx
...
--enable-demuxer=需要解码的容器格式
--enable-demuxer=xxx

```



最终的命令为

```
./configure --disable-static --enable-shared --enable-memalign-hack --enable-small\
--disable-encoders --disable-decoders --enable-decoder=h264 --enable-decoder=mpeg4 --enable-decoder=mjpeg --prefix=/e/ffmpeg-sdk
```



验证过程中出现的错误

**Q1**：找不到相应的库，将该命令删除

```
Unknown option "--enable-memalign-hack".
See ./configure --help for available options.
```

编译的命令修改为

```
./configure --enable-shared --disable-static  --disable-encoders --disable-decoders --enable-decoder=h264 --enable-small --prefix=/home/xxx/videocode/ffmpeg-sdk
make
make install
```

最后得到的文件大小为

```
0	libavcodec.so
0	libavcodec.so.58
1.9M	libavcodec.so.58.35.100
0	libavdevice.so
0	libavdevice.so.58
64K	libavdevice.so.58.5.100
0	libavfilter.so
0	libavfilter.so.7
2.1M	libavfilter.so.7.40.101
0	libavformat.so
0	libavformat.so.58
1.9M	libavformat.so.58.20.100
0	libavutil.so
0	libavutil.so.56
284K	libavutil.so.56.22.100
0	libswresample.so
0	libswresample.so.3
104K	libswresample.so.3.3.100
0	libswscale.so
0	libswscale.so.5
436K	libswscale.so.5.3.100
32K	pkgconfig
```





## 编译命令速查

在ffmpeg4.1.4源码文件夹下，运行

```
./configure --help
```

可以得到如下的结果

```
Help options:
  --help                   print this message
  --quiet                  Suppress showing informative output
  --list-decoders          show all available decoders
  --list-encoders          show all available encoders
  --list-hwaccels          show all available hardware accelerators
  --list-demuxers          show all available demuxers
  --list-muxers            show all available muxers
  --list-parsers           show all available parsers
  --list-protocols         show all available protocols
  --list-bsfs              show all available bitstream filters
  --list-indevs            show all available input devices
  --list-outdevs           show all available output devices
  --list-filters           show all available filters

Standard options:
  --logfile=FILE           log tests and output to FILE [ffbuild/config.log]
  --disable-logging        do not log configure debug information
  --fatal-warnings         fail if any configure warning is generated
  --prefix=PREFIX          install in PREFIX [/usr/local]
  --bindir=DIR             install binaries in DIR [PREFIX/bin]
  --datadir=DIR            install data files in DIR [PREFIX/share/ffmpeg]
  --docdir=DIR             install documentation in DIR [PREFIX/share/doc/ffmpeg]
  --libdir=DIR             install libs in DIR [PREFIX/lib]
  --shlibdir=DIR           install shared libs in DIR [LIBDIR]
  --incdir=DIR             install includes in DIR [PREFIX/include]
  --mandir=DIR             install man page in DIR [PREFIX/share/man]
  --pkgconfigdir=DIR       install pkg-config files in DIR [LIBDIR/pkgconfig]
  --enable-rpath           use rpath to allow installing libraries in paths
                           not part of the dynamic linker search path
                           use rpath when linking programs (USE WITH CARE)
  --install-name-dir=DIR   Darwin directory name for installed targets

Licensing options:
  --enable-gpl             allow use of GPL code, the resulting libs
                           and binaries will be under GPL [no]
  --enable-version3        upgrade (L)GPL to version 3 [no]
  --enable-nonfree         allow use of nonfree code, the resulting libs
                           and binaries will be unredistributable [no]

Configuration options:
  --disable-static         do not build static libraries [no]
  --enable-shared          build shared libraries [no]
  --enable-small           optimize for size instead of speed
  --disable-runtime-cpudetect disable detecting CPU capabilities at runtime (smaller binary)
  --enable-gray            enable full grayscale support (slower color)
  --disable-swscale-alpha  disable alpha channel support in swscale
  --disable-all            disable building components, libraries and programs
  --disable-autodetect     disable automatically detected external libraries [no]

Program options:
  --disable-programs       do not build command line programs
  --disable-ffmpeg         disable ffmpeg build
  --disable-ffplay         disable ffplay build
  --disable-ffprobe        disable ffprobe build

Documentation options:
  --disable-doc            do not build documentation
  --disable-htmlpages      do not build HTML documentation pages
  --disable-manpages       do not build man documentation pages
  --disable-podpages       do not build POD documentation pages
  --disable-txtpages       do not build text documentation pages

Component options:
  --disable-avdevice       disable libavdevice build
  --disable-avcodec        disable libavcodec build
  --disable-avformat       disable libavformat build
  --disable-swresample     disable libswresample build
  --disable-swscale        disable libswscale build
  --disable-postproc       disable libpostproc build
  --disable-avfilter       disable libavfilter build
  --enable-avresample      enable libavresample build (deprecated) [no]
  --disable-pthreads       disable pthreads [autodetect]
  --disable-w32threads     disable Win32 threads [autodetect]
  --disable-os2threads     disable OS/2 threads [autodetect]
  --disable-network        disable network support [no]
  --disable-dct            disable DCT code
  --disable-dwt            disable DWT code
  --disable-error-resilience disable error resilience code
  --disable-lsp            disable LSP code
  --disable-lzo            disable LZO decoder code
  --disable-mdct           disable MDCT code
  --disable-rdft           disable RDFT code
  --disable-fft            disable FFT code
  --disable-faan           disable floating point AAN (I)DCT code
  --disable-pixelutils     disable pixel utils in libavutil

Individual component options:
  --disable-everything     disable all components listed below
  --disable-encoder=NAME   disable encoder NAME
  --enable-encoder=NAME    enable encoder NAME
  --disable-encoders       disable all encoders
  --disable-decoder=NAME   disable decoder NAME
  --enable-decoder=NAME    enable decoder NAME
  --disable-decoders       disable all decoders
  --disable-hwaccel=NAME   disable hwaccel NAME
  --enable-hwaccel=NAME    enable hwaccel NAME
  --disable-hwaccels       disable all hwaccels
  --disable-muxer=NAME     disable muxer NAME
  --enable-muxer=NAME      enable muxer NAME
  --disable-muxers         disable all muxers
  --disable-demuxer=NAME   disable demuxer NAME
  --enable-demuxer=NAME    enable demuxer NAME
  --disable-demuxers       disable all demuxers
  --enable-parser=NAME     enable parser NAME
  --disable-parser=NAME    disable parser NAME
  --disable-parsers        disable all parsers
  --enable-bsf=NAME        enable bitstream filter NAME
  --disable-bsf=NAME       disable bitstream filter NAME
  --disable-bsfs           disable all bitstream filters
  --enable-protocol=NAME   enable protocol NAME
  --disable-protocol=NAME  disable protocol NAME
  --disable-protocols      disable all protocols
  --enable-indev=NAME      enable input device NAME
  --disable-indev=NAME     disable input device NAME
  --disable-indevs         disable input devices
  --enable-outdev=NAME     enable output device NAME
  --disable-outdev=NAME    disable output device NAME
  --disable-outdevs        disable output devices
  --disable-devices        disable all devices
  --enable-filter=NAME     enable filter NAME
  --disable-filter=NAME    disable filter NAME
  --disable-filters        disable all filters

External library support:

  Using any of the following switches will allow FFmpeg to link to the
  corresponding external library. All the components depending on that library
  will become enabled, if all their other dependencies are met and they are not
  explicitly disabled. E.g. --enable-libwavpack will enable linking to
  libwavpack and allow the libwavpack encoder to be built, unless it is
  specifically disabled with --disable-encoder=libwavpack.

  Note that only the system libraries are auto-detected. All the other external
  libraries must be explicitly enabled.

  Also note that the following help text describes the purpose of the libraries
  themselves, not all their features will necessarily be usable by FFmpeg.

  --disable-alsa           disable ALSA support [autodetect]
  --disable-appkit         disable Apple AppKit framework [autodetect]
  --disable-avfoundation   disable Apple AVFoundation framework [autodetect]
  --enable-avisynth        enable reading of AviSynth script files [no]
  --disable-bzlib          disable bzlib [autodetect]
  --disable-coreimage      disable Apple CoreImage framework [autodetect]
  --enable-chromaprint     enable audio fingerprinting with chromaprint [no]
  --enable-frei0r          enable frei0r video filtering [no]
  --enable-gcrypt          enable gcrypt, needed for rtmp(t)e support
                           if openssl, librtmp or gmp is not used [no]
  --enable-gmp             enable gmp, needed for rtmp(t)e support
                           if openssl or librtmp is not used [no]
  --enable-gnutls          enable gnutls, needed for https support
                           if openssl, libtls or mbedtls is not used [no]
  --disable-iconv          disable iconv [autodetect]
  --enable-jni             enable JNI support [no]
  --enable-ladspa          enable LADSPA audio filtering [no]
  --enable-libaom          enable AV1 video encoding/decoding via libaom [no]
  --enable-libass          enable libass subtitles rendering,
                           needed for subtitles and ass filter [no]
  --enable-libbluray       enable BluRay reading using libbluray [no]
  --enable-libbs2b         enable bs2b DSP library [no]
  --enable-libcaca         enable textual display using libcaca [no]
  --enable-libcelt         enable CELT decoding via libcelt [no]
  --enable-libcdio         enable audio CD grabbing with libcdio [no]
  --enable-libcodec2       enable codec2 en/decoding using libcodec2 [no]
  --enable-libdavs2        enable AVS2 decoding via libdavs2 [no]
  --enable-libdc1394       enable IIDC-1394 grabbing using libdc1394
                           and libraw1394 [no]
  --enable-libfdk-aac      enable AAC de/encoding via libfdk-aac [no]
  --enable-libflite        enable flite (voice synthesis) support via libflite [no]
  --enable-libfontconfig   enable libfontconfig, useful for drawtext filter [no]
  --enable-libfreetype     enable libfreetype, needed for drawtext filter [no]
  --enable-libfribidi      enable libfribidi, improves drawtext filter [no]
  --enable-libgme          enable Game Music Emu via libgme [no]
  --enable-libgsm          enable GSM de/encoding via libgsm [no]
  --enable-libiec61883     enable iec61883 via libiec61883 [no]
  --enable-libilbc         enable iLBC de/encoding via libilbc [no]
  --enable-libjack         enable JACK audio sound server [no]
  --enable-libklvanc       enable Kernel Labs VANC processing [no]
  --enable-libkvazaar      enable HEVC encoding via libkvazaar [no]
  --enable-liblensfun      enable lensfun lens correction [no]
  --enable-libmodplug      enable ModPlug via libmodplug [no]
  --enable-libmp3lame      enable MP3 encoding via libmp3lame [no]
  --enable-libopencore-amrnb enable AMR-NB de/encoding via libopencore-amrnb [no]
  --enable-libopencore-amrwb enable AMR-WB decoding via libopencore-amrwb [no]
  --enable-libopencv       enable video filtering via libopencv [no]
  --enable-libopenh264     enable H.264 encoding via OpenH264 [no]
  --enable-libopenjpeg     enable JPEG 2000 de/encoding via OpenJPEG [no]
  --enable-libopenmpt      enable decoding tracked files via libopenmpt [no]
  --enable-libopus         enable Opus de/encoding via libopus [no]
  --enable-libpulse        enable Pulseaudio input via libpulse [no]
  --enable-librsvg         enable SVG rasterization via librsvg [no]
  --enable-librubberband   enable rubberband needed for rubberband filter [no]
  --enable-librtmp         enable RTMP[E] support via librtmp [no]
  --enable-libshine        enable fixed-point MP3 encoding via libshine [no]
  --enable-libsmbclient    enable Samba protocol via libsmbclient [no]
  --enable-libsnappy       enable Snappy compression, needed for hap encoding [no]
  --enable-libsoxr         enable Include libsoxr resampling [no]
  --enable-libspeex        enable Speex de/encoding via libspeex [no]
  --enable-libsrt          enable Haivision SRT protocol via libsrt [no]
  --enable-libssh          enable SFTP protocol via libssh [no]
  --enable-libtensorflow   enable TensorFlow as a DNN module backend
                           for DNN based filters like sr [no]
  --enable-libtesseract    enable Tesseract, needed for ocr filter [no]
  --enable-libtheora       enable Theora encoding via libtheora [no]
  --enable-libtls          enable LibreSSL (via libtls), needed for https support
                           if openssl, gnutls or mbedtls is not used [no]
  --enable-libtwolame      enable MP2 encoding via libtwolame [no]
  --enable-libv4l2         enable libv4l2/v4l-utils [no]
  --enable-libvidstab      enable video stabilization using vid.stab [no]
  --enable-libvmaf         enable vmaf filter via libvmaf [no]
  --enable-libvo-amrwbenc  enable AMR-WB encoding via libvo-amrwbenc [no]
  --enable-libvorbis       enable Vorbis en/decoding via libvorbis,
                           native implementation exists [no]
  --enable-libvpx          enable VP8 and VP9 de/encoding via libvpx [no]
  --enable-libwavpack      enable wavpack encoding via libwavpack [no]
  --enable-libwebp         enable WebP encoding via libwebp [no]
  --enable-libx264         enable H.264 encoding via x264 [no]
  --enable-libx265         enable HEVC encoding via x265 [no]
  --enable-libxavs         enable AVS encoding via xavs [no]
  --enable-libxavs2        enable AVS2 encoding via xavs2 [no]
  --enable-libxcb          enable X11 grabbing using XCB [autodetect]
  --enable-libxcb-shm      enable X11 grabbing shm communication [autodetect]
  --enable-libxcb-xfixes   enable X11 grabbing mouse rendering [autodetect]
  --enable-libxcb-shape    enable X11 grabbing shape rendering [autodetect]
  --enable-libxvid         enable Xvid encoding via xvidcore,
                           native MPEG-4/Xvid encoder exists [no]
  --enable-libxml2         enable XML parsing using the C library libxml2, needed
                           for dash demuxing support [no]
  --enable-libzimg         enable z.lib, needed for zscale filter [no]
  --enable-libzmq          enable message passing via libzmq [no]
  --enable-libzvbi         enable teletext support via libzvbi [no]
  --enable-lv2             enable LV2 audio filtering [no]
  --disable-lzma           disable lzma [autodetect]
  --enable-decklink        enable Blackmagic DeckLink I/O support [no]
  --enable-libndi_newtek   enable Newteck NDI I/O support [no]
  --enable-mbedtls         enable mbedTLS, needed for https support
                           if openssl, gnutls or libtls is not used [no]
  --enable-mediacodec      enable Android MediaCodec support [no]
  --enable-libmysofa       enable libmysofa, needed for sofalizer filter [no]
  --enable-openal          enable OpenAL 1.1 capture support [no]
  --enable-opencl          enable OpenCL processing [no]
  --enable-opengl          enable OpenGL rendering [no]
  --enable-openssl         enable openssl, needed for https support
                           if gnutls, libtls or mbedtls is not used [no]
  --disable-sndio          disable sndio support [autodetect]
  --disable-schannel       disable SChannel SSP, needed for TLS support on
                           Windows if openssl and gnutls are not used [autodetect]
  --disable-sdl2           disable sdl2 [autodetect]
  --disable-securetransport disable Secure Transport, needed for TLS support
                           on OSX if openssl and gnutls are not used [autodetect]
  --enable-vapoursynth     enable VapourSynth demuxer [no]
  --disable-xlib           disable xlib [autodetect]
  --disable-zlib           disable zlib [autodetect]

  The following libraries provide various hardware acceleration features:
  --disable-amf            disable AMF video encoding code [autodetect]
  --disable-audiotoolbox   disable Apple AudioToolbox code [autodetect]
  --enable-cuda-sdk        enable CUDA features that require the CUDA SDK [no]
  --disable-cuvid          disable Nvidia CUVID support [autodetect]
  --disable-d3d11va        disable Microsoft Direct3D 11 video acceleration code [autodetect]
  --disable-dxva2          disable Microsoft DirectX 9 video acceleration code [autodetect]
  --disable-ffnvcodec      disable dynamically linked Nvidia code [autodetect]
  --enable-libdrm          enable DRM code (Linux) [no]
  --enable-libmfx          enable Intel MediaSDK (AKA Quick Sync Video) code via libmfx [no]
  --enable-libnpp          enable Nvidia Performance Primitives-based code [no]
  --enable-mmal            enable Broadcom Multi-Media Abstraction Layer (Raspberry Pi) via MMAL [no]
  --disable-nvdec          disable Nvidia video decoding acceleration (via hwaccel) [autodetect]
  --disable-nvenc          disable Nvidia video encoding code [autodetect]
  --enable-omx             enable OpenMAX IL code [no]
  --enable-omx-rpi         enable OpenMAX IL code for Raspberry Pi [no]
  --enable-rkmpp           enable Rockchip Media Process Platform code [no]
  --disable-v4l2-m2m       disable V4L2 mem2mem code [autodetect]
  --disable-vaapi          disable Video Acceleration API (mainly Unix/Intel) code [autodetect]
  --disable-vdpau          disable Nvidia Video Decode and Presentation API for Unix code [autodetect]
  --disable-videotoolbox   disable VideoToolbox code [autodetect]

Toolchain options:
  --arch=ARCH              select architecture []
  --cpu=CPU                select the minimum required CPU (affects
                           instruction selection, may crash on older CPUs)
  --cross-prefix=PREFIX    use PREFIX for compilation tools []
  --progs-suffix=SUFFIX    program name suffix []
  --enable-cross-compile   assume a cross-compiler is used
  --sysroot=PATH           root of cross-build tree
  --sysinclude=PATH        location of cross-build system headers
  --target-os=OS           compiler targets OS []
  --target-exec=CMD        command to run executables on target
  --target-path=DIR        path to view of build directory on target
  --target-samples=DIR     path to samples directory on target
  --tempprefix=PATH        force fixed dir/prefix instead of mktemp for checks
  --toolchain=NAME         set tool defaults according to NAME
                           (gcc-asan, clang-asan, gcc-msan, clang-msan,
                           gcc-tsan, clang-tsan, gcc-usan, clang-usan,
                           valgrind-massif, valgrind-memcheck,
                           msvc, icl, gcov, llvm-cov, hardened)
  --nm=NM                  use nm tool NM [nm -g]
  --ar=AR                  use archive tool AR [ar]
  --as=AS                  use assembler AS []
  --ln_s=LN_S              use symbolic link tool LN_S [ln -s -f]
  --strip=STRIP            use strip tool STRIP [strip]
  --windres=WINDRES        use windows resource compiler WINDRES [windres]
  --x86asmexe=EXE          use nasm-compatible assembler EXE [nasm]
  --cc=CC                  use C compiler CC [gcc]
  --cxx=CXX                use C compiler CXX [g++]
  --objcc=OCC              use ObjC compiler OCC [gcc]
  --dep-cc=DEPCC           use dependency generator DEPCC [gcc]
  --nvcc=NVCC              use Nvidia CUDA compiler NVCC [nvcc]
  --ld=LD                  use linker LD []
  --pkg-config=PKGCONFIG   use pkg-config tool PKGCONFIG [pkg-config]
  --pkg-config-flags=FLAGS pass additional flags to pkgconf []
  --ranlib=RANLIB          use ranlib RANLIB [ranlib]
  --doxygen=DOXYGEN        use DOXYGEN to generate API doc [doxygen]
  --host-cc=HOSTCC         use host C compiler HOSTCC
  --host-cflags=HCFLAGS    use HCFLAGS when compiling for host
  --host-cppflags=HCPPFLAGS use HCPPFLAGS when compiling for host
  --host-ld=HOSTLD         use host linker HOSTLD
  --host-ldflags=HLDFLAGS  use HLDFLAGS when linking for host
  --host-libs=HLIBS        use libs HLIBS when linking for host
  --host-os=OS             compiler host OS []
  --extra-cflags=ECFLAGS   add ECFLAGS to CFLAGS []
  --extra-cxxflags=ECFLAGS add ECFLAGS to CXXFLAGS []
  --extra-objcflags=FLAGS  add FLAGS to OBJCFLAGS []
  --extra-ldflags=ELDFLAGS add ELDFLAGS to LDFLAGS []
  --extra-ldexeflags=ELDFLAGS add ELDFLAGS to LDEXEFLAGS []
  --extra-ldsoflags=ELDFLAGS add ELDFLAGS to LDSOFLAGS []
  --extra-libs=ELIBS       add ELIBS []
  --extra-version=STRING   version string suffix []
  --optflags=OPTFLAGS      override optimization-related compiler flags
  --nvccflags=NVCCFLAGS    override nvcc flags [-gencode arch=compute_30,code=sm_30 -O2]
  --build-suffix=SUFFIX    library name suffix []
  --enable-pic             build position-independent code
  --enable-thumb           compile for Thumb instruction set
  --enable-lto             use link-time optimization
  --env="ENV=override"     override the environment variables

Advanced options (experts only):
  --malloc-prefix=PREFIX   prefix malloc and related names with PREFIX
  --custom-allocator=NAME  use a supported custom allocator
  --disable-symver         disable symbol versioning
  --enable-hardcoded-tables use hardcoded tables instead of runtime generation
  --disable-safe-bitstream-reader
                           disable buffer boundary checking in bitreaders
                           (faster, but may crash)
  --sws-max-filter-size=N  the max filter size swscale uses [256]

Optimization options (experts only):
  --disable-asm            disable all assembly optimizations
  --disable-altivec        disable AltiVec optimizations
  --disable-vsx            disable VSX optimizations
  --disable-power8         disable POWER8 optimizations
  --disable-amd3dnow       disable 3DNow! optimizations
  --disable-amd3dnowext    disable 3DNow! extended optimizations
  --disable-mmx            disable MMX optimizations
  --disable-mmxext         disable MMXEXT optimizations
  --disable-sse            disable SSE optimizations
  --disable-sse2           disable SSE2 optimizations
  --disable-sse3           disable SSE3 optimizations
  --disable-ssse3          disable SSSE3 optimizations
  --disable-sse4           disable SSE4 optimizations
  --disable-sse42          disable SSE4.2 optimizations
  --disable-avx            disable AVX optimizations
  --disable-xop            disable XOP optimizations
  --disable-fma3           disable FMA3 optimizations
  --disable-fma4           disable FMA4 optimizations
  --disable-avx2           disable AVX2 optimizations
  --disable-avx512         disable AVX-512 optimizations
  --disable-aesni          disable AESNI optimizations
  --disable-armv5te        disable armv5te optimizations
  --disable-armv6          disable armv6 optimizations
  --disable-armv6t2        disable armv6t2 optimizations
  --disable-vfp            disable VFP optimizations
  --disable-neon           disable NEON optimizations
  --disable-inline-asm     disable use of inline assembly
  --disable-x86asm         disable use of standalone x86 assembly
  --disable-mipsdsp        disable MIPS DSP ASE R1 optimizations
  --disable-mipsdspr2      disable MIPS DSP ASE R2 optimizations
  --disable-msa            disable MSA optimizations
  --disable-mipsfpu        disable floating point MIPS optimizations
  --disable-mmi            disable Loongson SIMD optimizations
  --disable-fast-unaligned consider unaligned accesses slow

Developer options (useful when working on FFmpeg itself):
  --disable-debug          disable debugging symbols
  --enable-debug=LEVEL     set the debug level []
  --disable-optimizations  disable compiler optimizations
  --enable-extra-warnings  enable more compiler warnings
  --disable-stripping      disable stripping of executables and shared libraries
  --assert-level=level     0(default), 1 or 2, amount of assertion testing,
                           2 causes a slowdown at runtime.
  --enable-memory-poisoning fill heap uninitialized allocated space with arbitrary data
  --valgrind=VALGRIND      run "make fate" tests through valgrind to detect memory
                           leaks and errors, using the specified valgrind binary.
                           Cannot be combined with --target-exec
  --enable-ftrapv          Trap arithmetic overflows
  --samples=PATH           location of test samples for FATE, if not set use
                           $FATE_SAMPLES at make invocation time.
  --enable-neon-clobber-test check NEON registers for clobbering (should be
                           used only for debugging purposes)
  --enable-xmm-clobber-test check XMM registers for clobbering (Win64-only;
                           should be used only for debugging purposes)
  --enable-random          randomly enable/disable components
  --disable-random
  --enable-random=LIST     randomly enable/disable specific components or
  --disable-random=LIST    component groups. LIST is a comma-separated list
                           of NAME[:PROB] entries where NAME is a component
                           (group) and PROB the probability associated with
                           NAME (default 0.5).
  --random-seed=VALUE      seed value for --enable/disable-random
  --disable-valgrind-backtrace do not print a backtrace under Valgrind
                           (only applies to --disable-optimizations builds)
  --enable-osfuzz          Enable building fuzzer tool
  --libfuzzer=PATH         path to libfuzzer
  --ignore-tests=TESTS     comma-separated list (without "fate-" prefix
                           in the name) of tests whose result is ignored
  --enable-linux-perf      enable Linux Performance Monitor API

NOTE: Object files are built at the place where configure is launched.
```

## 支持的解码器



```
./configure --list-decoders

#输出


aac			   asv1			      eamad			 indeo2
aac_at			   asv2			      eatgq			 indeo3
aac_fixed		   atrac1		      eatgv			 indeo4
aac_latm		   atrac3		      eatqi			 indeo5
aasc			   atrac3al		      eightbps			 interplay_acm
ac3			   atrac3p		      eightsvx_exp		 interplay_dpcm
ac3_at			   atrac3pal		      eightsvx_fib		 interplay_video
ac3_fixed		   atrac9		      escape124			 jacosub
adpcm_4xm		   aura			      escape130			 jpeg2000
adpcm_adx		   aura2		      evrc				 jpegls
adpcm_afc		   avrn			      exr			 jv
adpcm_aica		   avrp			      ffv1			 kgv1
adpcm_ct		   avs			      ffvhuff			 kmvc
adpcm_dtk		   avui			      ffwavesynth		 lagarith
adpcm_ea		   ayuv			      fic			 libaom_av1
adpcm_ea_maxis_xa	   bethsoftvid		      fits			 libcelt
adpcm_ea_r1		   bfi			      flac			 libcodec2
adpcm_ea_r2		   bink			      flashsv			 libdavs2
adpcm_ea_r3		   binkaudio_dct	      flashsv2			 libfdk_aac
adpcm_ea_xas		   binkaudio_rdft	      flic			 libgsm
adpcm_g722		   bintext		      flv			 libgsm_ms
adpcm_g726		   bitpacked		      fmvc			 libilbc
adpcm_g726le		   bmp			      fourxm			 libopencore_amrnb
adpcm_ima_amv		   bmv_audio		      fraps			 libopencore_amrwb
adpcm_ima_apc		   bmv_video		      frwu			 libopenh264
adpcm_ima_dat4		   brender_pix		      g2m			 libopenjpeg
adpcm_ima_dk3		   c93			      g723_1			 libopus
adpcm_ima_dk4		   cavs			      g729			 librsvg
adpcm_ima_ea_eacs	   ccaption		      gdv			 libspeex
adpcm_ima_ea_sead	   cdgraphics		      gif			 libvorbis
adpcm_ima_iss		   cdxl			      gremlin_dpcm		 libvpx_vp8
adpcm_ima_oki		   cfhd			      gsm			 libvpx_vp9
adpcm_ima_qt		   cinepak		      gsm_ms			 libzvbi_teletext
adpcm_ima_qt_at		   clearvideo		      gsm_ms_at			 loco
adpcm_ima_rad		   cljr			      h261			 m101
adpcm_ima_smjpeg	   cllc			      h263			 mace3
adpcm_ima_wav		   comfortnoise		      h263_v4l2m2m		 mace6
adpcm_ima_ws		   cook			      h263i			 magicyuv
adpcm_ms		   cpia			      h263p			 mdec
adpcm_mtaf		   cscd			      h264			 metasound
adpcm_psx		   cyuv			      h264_crystalhd		 microdvd
adpcm_sbpro_2		   dca			      h264_cuvid		 mimic
adpcm_sbpro_3		   dds			      h264_mediacodec		 mjpeg
adpcm_sbpro_4		   dfa			      h264_mmal			 mjpeg_cuvid
adpcm_swf		   dirac		      h264_qsv			 mjpegb
adpcm_thp		   dnxhd		      h264_rkmpp		 mlp
adpcm_thp_le		   dolby_e		      h264_v4l2m2m		 mmvideo
adpcm_vima		   dpx			      hap			 motionpixels
adpcm_xa		   dsd_lsbf		      hevc			 movtext
adpcm_yamaha		   dsd_lsbf_planar	      hevc_cuvid		 mp1
aic			   dsd_msbf		      hevc_mediacodec		 mp1_at
alac			   dsd_msbf_planar	      hevc_qsv			 mp1float
alac_at			   dsicinaudio		      hevc_rkmpp		 mp2
alias_pix		   dsicinvideo		      hevc_v4l2m2m		 mp2_at
als			   dss_sp		      hnm4_video		 mp2float
amr_nb_at		   dst			      hq_hqa			 mp3
amrnb			   dvaudio		      hqx			 mp3_at
amrwb			   dvbsub		      huffyuv			 mp3adu
amv			   dvdsub		      iac			 mp3adufloat
anm			   dvvideo		      idcin			 mp3float
ansi			   dxa			      idf			 mp3on4
ape			   dxtory		      iff_ilbm			 mp3on4float
apng			   dxv			      ilbc			 mpc7
aptx			   eac3			      ilbc_at			 mpc8
aptx_hd			   eac3_at		      imc			 mpeg1_cuvid
ass			   eacmv		      imm4			 mpeg1_v4l2m2m
mpeg1video		   pcm_s32be		      sgi			 vmdaudio
mpeg2_crystalhd		   pcm_s32le		      sgirle			 vmdvideo
mpeg2_cuvid		   pcm_s32le_planar	      sheervideo		 vmnc
mpeg2_mediacodec	   pcm_s64be		      shorten			 vorbis
mpeg2_mmal		   pcm_s64le		      sipr			 vp3
mpeg2_qsv		   pcm_s8		      smackaud			 vp5
mpeg2_v4l2m2m		   pcm_s8_planar	      smacker			 vp6
mpeg2video		   pcm_u16be		      smc			 vp6a
mpeg4			   pcm_u16le		      smvjpeg			 vp6f
mpeg4_crystalhd		   pcm_u24be		      snow			 vp7
mpeg4_cuvid		   pcm_u24le		      sol_dpcm			 vp8
mpeg4_mediacodec	   pcm_u32be		      sonic			 vp8_cuvid
mpeg4_mmal		   pcm_u32le		      sp5x			 vp8_mediacodec
mpeg4_v4l2m2m		   pcm_u8		      speedhq			 vp8_qsv
mpegvideo		   pcm_vidc		      srgc			 vp8_rkmpp
mpl2			   pcm_zork		      srt			 vp8_v4l2m2m
msa1			   pcx			      ssa			 vp9
mscc			   pgm			      stl			 vp9_cuvid
msmpeg4_crystalhd	   pgmyuv		      subrip			 vp9_mediacodec
msmpeg4v1		   pgssub		      subviewer			 vp9_rkmpp
msmpeg4v2		   pictor		      subviewer1		 vp9_v4l2m2m
msmpeg4v3		   pixlet		      sunrast			 vplayer
msrle			   pjs			      svq1			 vqa
mss1			   png			      svq3			 wavpack
mss2			   ppm			      tak			 wcmv
msvideo1		   prores		      targa			 webp
mszh			   prosumer		      targa_y216		 webvtt
mts2			   psd			      tdsc			 wmalossless
mvc1			   ptx			      text			 wmapro
mvc2			   qcelp		      theora			 wmav1
mwsc			   qdm2			      thp			 wmav2
mxpeg			   qdm2_at		      tiertexseqvideo		 wmavoice
nellymoser		   qdmc			      tiff			 wmv1
nuv			   qdmc_at		      tmv			 wmv2
on2avc			   qdraw		      truehd			 wmv3
opus			   qpeg			      truemotion1		 wmv3_crystalhd
paf_audio		   qtrle		      truemotion2		 wmv3image
paf_video		   r10k			      truemotion2rt		 wnv1
pam			   r210			      truespeech		 wrapped_avframe
pbm			   ra_144		      tscc			 ws_snd1
pcm_alaw		   ra_288		      tscc2			 xan_dpcm
pcm_alaw_at		   ralf			      tta			 xan_wc3
pcm_bluray		   rasc			      twinvq			 xan_wc4
pcm_dvd			   rawvideo		      txd			 xbin
pcm_f16le		   realtext		      ulti			 xbm
pcm_f24le		   rl2			      utvideo			 xface
pcm_f32be		   roq			      v210			 xl
pcm_f32le		   roq_dpcm		      v210x			 xma1
pcm_f64be		   rpza			      v308			 xma2
pcm_f64le		   rscc			      v408			 xpm
pcm_lxf			   rv10			      v410			 xsub
pcm_mulaw		   rv20			      vb			 xwd
pcm_mulaw_at		   rv30			      vble			 y41p
pcm_s16be		   rv40			      vc1			 ylc
pcm_s16be_planar	   s302m		      vc1_crystalhd		 yop
pcm_s16le		   sami			      vc1_cuvid			 yuv4
pcm_s16le_planar	   sanm			      vc1_mmal			 zero12v
pcm_s24be		   sbc			      vc1_qsv			 zerocodec
pcm_s24daud		   scpr			      vc1_v4l2m2m		 zlib
pcm_s24le		   screenpresso		      vc1image			 zmbv
pcm_s24le_planar	   sdx2_dpcm		      vcr1
```



## 支持的编码器

```
./configure --list-encoders


#输出
a64multi		   h264_omx		      mp2fixed			 prores
a64multi5		   h264_qsv		      mpeg1video		 prores_aw
aac			   h264_v4l2m2m		      mpeg2_qsv			 prores_ks
aac_at			   h264_vaapi		      mpeg2_vaapi		 qtrle
ac3			   h264_videotoolbox	      mpeg2video		 r10k
ac3_fixed		   hap			      mpeg4			 r210
adpcm_adx		   hevc_amf		      mpeg4_v4l2m2m		 ra_144
adpcm_g722		   hevc_nvenc		      msmpeg4v2			 rawvideo
adpcm_g726		   hevc_qsv		      msmpeg4v3			 roq
adpcm_g726le		   hevc_v4l2m2m		      msvideo1			 roq_dpcm
adpcm_ima_qt		   hevc_vaapi		      nellymoser		 rv10
adpcm_ima_wav		   hevc_videotoolbox	      nvenc			 rv20
adpcm_ms		   huffyuv		      nvenc_h264		 s302m
adpcm_swf		   ilbc_at		      nvenc_hevc		 sbc
adpcm_yamaha		   jpeg2000		      opus			 sgi
alac			   jpegls		      pam			 snow
alac_at			   libaom_av1		      pbm			 sonic
alias_pix		   libcodec2		      pcm_alaw			 sonic_ls
amv			   libfdk_aac		      pcm_alaw_at		 srt
apng			   libgsm		      pcm_f32be			 ssa
aptx			   libgsm_ms		      pcm_f32le			 subrip
aptx_hd			   libilbc		      pcm_f64be			 sunrast
ass			   libkvazaar		      pcm_f64le			 svq1
asv1			   libmp3lame		      pcm_mulaw			 targa
asv2			   libopencore_amrnb	      pcm_mulaw_at		 text
avrp			   libopenh264		      pcm_s16be			 tiff
avui			   libopenjpeg		      pcm_s16be_planar		 truehd
ayuv			   libopus		      pcm_s16le			 tta
bmp			   libshine		      pcm_s16le_planar		 utvideo
cinepak			   libspeex		      pcm_s24be			 v210
cljr			   libtheora		      pcm_s24daud		 v308
comfortnoise		   libtwolame		      pcm_s24le			 v408
dca			   libvo_amrwbenc	      pcm_s24le_planar		 v410
dnxhd			   libvorbis		      pcm_s32be			 vc2
dpx			   libvpx_vp8		      pcm_s32le			 vorbis
dvbsub			   libvpx_vp9		      pcm_s32le_planar		 vp8_v4l2m2m
dvdsub			   libwavpack		      pcm_s64be			 vp8_vaapi
dvvideo			   libwebp		      pcm_s64le			 vp9_vaapi
eac3			   libwebp_anim		      pcm_s8			 wavpack
ffv1			   libx262		      pcm_s8_planar		 webvtt
ffvhuff			   libx264		      pcm_u16be			 wmav1
fits			   libx264rgb		      pcm_u16le			 wmav2
flac			   libx265		      pcm_u24be			 wmv1
flashsv			   libxavs		      pcm_u24le			 wmv2
flashsv2		   libxavs2		      pcm_u32be			 wrapped_avframe
flv			   libxvid		      pcm_u32le			 xbm
g723_1			   ljpeg		      pcm_u8			 xface
gif			   magicyuv		      pcm_vidc			 xsub
h261			   mjpeg		      pcx			 xwd
h263			   mjpeg_qsv		      pgm			 y41p
h263_v4l2m2m		   mjpeg_vaapi		      pgmyuv			 yuv4
h263p			   mlp			      png			 zlib
h264_amf		   movtext		      ppm			 zmbv
h264_nvenc		   mp2
```





参考博客

<https://www.cnblogs.com/agiletiger/p/4888252.html>



## 精简版ffmpeg编译脚本





```
#!/bin/sh
build_one()
{
  ./configure --target-os=linux\
  --enable-cross-compile \
  --extra-libs="-lgcc"\
  --arch=x86 \
  --disable-shared \
  --disable-ffprobe\
  --disable-ffserver \
  --disable-ffmpeg \
  --enable-debug=2\
  --enable-stripping \
  --enable-avresample \
  --enable-swresample \
  --enable-optimizations\
  --disable-swscale \
  --disable-bsfs \
  --disable-postproc \
  --disable-doc \
  --disable-devices \
  --disable-encoders \
  --disable-muxers \
  --disable-avdevice \
  --disable-parsers \
  --enable-parser=aac \
  --enable-parser=h264 \
  --enable-parser=mpeg4video \
  --enable-parser=mpegvideo \
  --disable-demuxers \
  --enable-demuxer=aac \
  --enable-demuxer=flv \
  --enable-demuxer=h264 \
  --enable-demuxer=mov \
  --enable-demuxer=mp3 \
  --enable-demuxer=hls \
    --enable-demuxer=mpegts \
    --disable-filters \
    --enable-filter=format \
    --disable-protocols \
    --disable-protocol=applehttp \
    --enable-protocol=tcp \
    --enable-protocol=udp \
    --enable-protocol=file \
    --enable-protocol=http \
    --enable-protocol=hls \
    --disable-decoders \
    --enable-decoder=aac \
    --enable-decoder=mp3 \
    --enable-decoder=flv
#make clean
#make  -j4 install
make -j4
  
}
build_one

```







## 简易版验证

参考另外一个文档中的examples文件夹中的代码修改，然后调用简易库，成功，可以得到相关函数的结果























