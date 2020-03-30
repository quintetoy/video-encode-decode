## ffmpeg压缩的参考文档

[TOC]

ffmpeg是一套可以用来记录、转换数字音频、视频，并能将其转化为流的开源计算机程序。

其实ffmpeg只是对该框架的通俗称呼，该框架本质上为libav，ffmpeg也只是基于libav开发的一套工具。

libav框架可以编译在其他平台进行使用，在下载的源码中，包含有configure程序来进行编译工作，但是单纯的运行该程序进行编译，编译生成的包体积是非常大的，所以我们需要进行一些配置来减小编译包的大小。

一、增加编译选项
首先我们可以增加一些常规编译选项来减小最终编译包的大小。

我们可以使用configure -h命令来列出configure程序支持的编译选项，每一项编译选项后都有对应的解释，其中能够直接减小编译包大小的命令有如下几个：

–enable-small ：使用编译速度来换取编译包大小。
–disable-runtime-cpudetect ：禁止运行时检测CPU性能，可以编出较小的包。
–disable-doc ：禁止编译文档，可以避免将文档编译入包中。
–disable-htmlpages ：禁止编译html文档，可以避免将文档编译入包中。
–disable-htmlpages ：禁止编译html文档，可以避免将文档编译入包中。
–disable-manpages ：禁止编译man文档，可以避免将文档编译入包中。
–disable-podpages ：禁止编译pod文档，可以避免将文档编译入包中。
–disable-txtpages ：禁止编译txt文档，可以避免将文档编译入包中。
二、减少不必要的工具
在最开始我们介绍了ffmpeg是基于libav开发的一套工具，除了ffmpeg之外，基于libav开发的工具还有：ffplay、ffprobe以及ffserver。

这些不必要的工具我们是可以禁止掉的，相关选项为：

–disable-programs ：禁止编译命令行工具。
–disable-ffmpeg ：禁止编译ffmpeg工具。
–disable-ffplay ：禁止编译ffplay工具。
–disable-ffprobe ：禁止编译ffprobe工具。
–disable-ffserver ：禁止编译ffserver工具。
三、减少不必要的模块
libav包含以下几个模块：

libavdevice
libavcodec
libavformat
libswresample
libswscale
libpostproc
libavfilter
libavresample
1. libavdevice
  该模块主要负责与硬件设备的交互，若无需该模块，可使用--disable-avdevice禁止编译。

2. libavcodec
  该模块主要负责解码与编码，若无需该模块，可使用--disable-avcodec禁止编译，不过该模块为libav核心模块，非特殊情况最好不要禁止。

3. libavformat
  该模块主要负责解封装与封装，若无需该模块，可使用--disable-avformat禁止编译，不过该模块为libav核心模块，非特殊情况最好不要禁止。

4. libswresample
  该模块主要负责对原始音频数据进行格式转换，若无需该模块，可使用--disable-swresample禁止编译。

5. libswscale
  该模块主要负责对原始视频数据进行场景转换、色彩映射以及格式转换，若无需该模块，可使用--disable-swscale禁止编译。

6. libpostproc
  该模块主要负责对音视频进行后期处理，若无需该模块，可使用--disable-postproc禁止编译。

7. libavfilter
  该模块主要负责音视频的过滤，包括裁剪、位置、水印等，若无需该模块，可使用--disable-avfilter禁止编译。

8. libavresample
  该模块主要负责音视频封装编解码格式预设，该模块默认不编译，若要进行编译，使用--enable-avresample。

四、减少不必要设备的编译
libav可以从硬件设备中获取输入，同时也可以输出至硬件设备。

我们可以指定支持的输入输出设备来避免不必要的编译：

–disable-devices ：禁止所有设备的编译。
–disable-indevs ：禁止所有输入设备的编译。
–disable-indev=NAME ：禁止特定输入设备的编译。
–enable-indev=NAME ：允许特定输入设备的编译，搭配–disable-indevs可以实现单纯指定支持的输入设备。
–disable-outdevs ：禁止所有输出设备的编译。
–disable-outdev=NAME ：禁止特定输出设备的编译。
–enable-outdev=NAME ：允许特定输出设备的编译，搭配–disable-outdevs可以实现单纯指定支持的输出设备。
关于libav支持的输入输出设备名称，可以使用configure --list-indevs和configure --list-outdevs命令获取。

五、减少不必要解析器的编译
libav可以对输入的数据进行格式检测，该功能由解析器(parser)负责。

我们可以指定支持的解析器来避免不必要的编译：

–disable-parsers ：禁止所有解析器的编译。
–disable-parser=NAME ：禁止特定解析器的编译。
–enable-parser=NAME ：允许特定解析器的编译，搭配–disable-parsers可以实现单纯指定支持的解析器。
关于libav支持的解析器名称，可以使用configure --list-parsers命令获取。

六、减少不必要的二进制流过滤器的编译
libav可以将输入的数据转为二进制数据，同时可以对二进制数据进行特殊的处理，该功能由二进制流过滤器(bit stream filter)负责。

我们可以指定支持的二进制流过滤器来避免不必要的编译：

–disable-bsfs ：禁止所有二进制流过滤器的编译。
–disable-bsf=NAME ：禁止特定二进制流过滤器的编译。
–enable-bsf=NAME ：允许特定二进制流过滤器的编译，搭配–disable-bsfs可以实现单纯指定支持的二进制流过滤器。
关于libav支持二进制流过滤器名称，可以使用configure --list-bsfs命令获取。

七、减少不必要的协议编译
libav对于如何读入数据及输出数据制定了一套协议，同时libav内置了一些满足协议的方式，这些方式可以通过configure --list-protocols列出。

我们可以指定支持的输入输出方式来避免不必要的编译：

–disable-protocols ：禁止所有输入输出方式的编译。
–disable-protocol=NAME ：禁止特定输入输出方式的编译。
–enable-protocol=NAME ：允许特定输入输出方式的编译，搭配–disable-protocols可以实现单纯指定支持的输入输出方式。
我们必须指定至少一种输入输出方式，通常通过使用--disable-protocols搭配--enable-protocol=NAME来完成。

八、减少不必要组件的编译
接下来先简单介绍一下libav处理音视频的流程，以将 FLV格式(H264视频+AAC音频) 转换为 AVI格式(MPEG2视频+MP3音频) 为例，流程图如下：



在该过程中，负责解封装的是分离器(demuxer)、负责封装的是复用器(muxer)、负责音视频解码的为解码器(decoder)、负责编码的为编码器(encoder)。

我们可以从libav所支持的四个组件的类型来减少不必要的编译。

可以使用以下命令获取组件所支持的类型：

configure --list-demuxers
configure --list-muxers
configure --list-decoders
configure --list-encoders
1. 分离器
  –disable-demuxers ：禁止所有分离器的编译。
  –disable-demuxer=NAME ：禁止特定分离器的编译。
  –enable-demuxer=NAME ：允许特定分离器的编译，搭配–disable-demuxers。
2. 复用器
  –disable-muxers ：禁止所有复用器的编译。
  –disable-muxer=NAME ：禁止特定复用器的编译。
  –enable-muxer=NAME ：允许特定复用器的编译，搭配–disable-muxers。
3. 解码器
  –disable-decoders ：禁止所有解码器的编译。
  –disable-decoder=NAME ：禁止特定解码器的编译。
  –enable-decoder=NAME ：允许特定解码器的编译，搭配–disable-decoders。
4. 编码器
  –disable-encoders ：禁止所有编码器的编译。
  –disable-encoder=NAME ：禁止特定编码器的编译。
  –enable-encoder=NAME ：允许特定编码器的编译，搭配–disable-encoders。
  至此，通过对项目的特殊定制，可以最大化的减小编译包的大小，避免编译包太大造成最终产品体积过大的问题。
------------------------------------------------
参考博客：

https://blog.csdn.net/TuGeLe/article/details/86530968

