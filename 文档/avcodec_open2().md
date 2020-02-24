# avcodec_open2()

该函数用于初始化一个视音频编解码器的AVCodecContext。位于libavcodec/avcode.h

```cpp

int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);

//avctx：需要初始化的AVCodecContext
//codec：输入的AVCodec
//options：一些选项，例如使用libx264编码的时候，“preset”，“tune”等都可以通过该参数设置
```





略去源码，它的调用关系非常简单，只调用了一个关键的函数，AVCodec的init（）。

avcodec_open2()所做的工作主要如下：

```
1、为各种结构体分配内存，通过各种av_malloc实现
2、将输入的AVDictionary形式的选项设置到AVCodecContext
3、其余的一些检查，检查解码器是否处于“实验”阶段
4、如果是编码器，检查输入参数是否符合编码器的要求（例如检查像素格式，一般编码器都会有该对应支持的像素格式，libx264主要支持的是以YUV为主的像素格式）
5、调用AVCodec的init（）初始化具体的解码器。
```



## AVCodec的init（）

它是一个函数指针，指向具体编码器中的初始化函数。以具体的函数为例，查看对应的定义

```cpp
AVCodec ff_libx264_encoder = {
    .name             = "libx264",
    .long_name        = NULL_IF_CONFIG_SMALL("libx264 H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
    .type             = AVMEDIA_TYPE_VIDEO,
    .id               = AV_CODEC_ID_H264,
    .priv_data_size   = sizeof(X264Context),
    .init             = X264_init,
    .encode2          = X264_frame,
    .close            = X264_close,
    .capabilities     = CODEC_CAP_DELAY | CODEC_CAP_AUTO_THREADS,
    .priv_class       = &x264_class,
    .defaults         = x264_defaults,
    .init_static_data = X264_init_static,
};

```

可以看出其init（）指向X264_init().

功能如下：

```
（1）设置X264Context的参数。X264Context主要完成了libx264和FFmpeg对接的功能。可以看出代码主要在设置一个params结构体变量，该变量的类型即是x264中存储参数的结构体x264_param_t。
（2）调用libx264的API进行编码器的初始化工作。例如调用x264_param_default()设置默认参数，调用x264_param_apply_profile()设置profile，调用x264_encoder_open()打开编码器等等。

```

























































