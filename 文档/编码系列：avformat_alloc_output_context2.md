#编码系列：avformat_alloc_output_context2

<https://blog.csdn.net/leixiaohua1020/article/details/41198929>

通常该函数是第一个调用的函数，可以初始化一个用于输出的AVFormatContext结构体

```cpp

int avformat_alloc_output_context2(AVFormatContext **ctx, AVOutputFormat *oformat,
                                   const char *format_name, const char *filename);

```



```
ctx:函数调用成功之后创建的AVFormatContext结构体
oformat:指定AVFormatContext中的AVOutputFormat，用于确定输出格式，如果指定为NULL，可以设定后两个参数（format_name或者filename）由FFmpeg猜测输出格式
PS：使用该参数需要自己手动获取AVOutputFormat，相对于使用后两个参数来说要麻烦一些
format_name:指定输出格式的名称，根据格式名称。FFmpeg会推测输出格式，输出格式可以是，flv，mkv等等
filename:指定输出文件的名称，根据文件名称，ffmpeg会推测输出格式，可以是xx.flv
函数执行成功的话，其返回值大于等于0

```



```
1) 调用avformat_alloc_context()初始化一个默认的AVFormatContext。
2) 如果指定了输入的AVOutputFormat，则直接将输入的AVOutputFormat赋值给AVOutputFormat的oformat。如果没有指定输入的AVOutputFormat，就需要根据文件格式名称或者文件名推测输出的AVOutputFormat。无论是通过文件格式名称还是文件名推测输出格式，都会调用一个函数av_guess_format()。
```



## avformat_alloc_context()

avformat_alloc_context()首先调用av_malloc（）为AVFormatContext分配一块内存，然后调用了一个函数avformat_get_context_defaults()用于给AVFormatContext设置默认值。

avformat_alloc_context()首先调用memset()将AVFormatContext的内存置0，然后指定它的AVClass（指定了AVClass之后，该结构体就支持和AVOption相关的功能）；

最后调用av_opt_set_defaults()给AVFormatContext的成员变量设置默认值（av_opt_set_defaults()就是和AVOption有关的一个函数，专门用于给指定的结构体设定默认值，此处暂不分析）。



```cpp
AVOutputFormat *av_guess_format(const char *short_name,//格式的名称
                                const char *filename,//文件的名称
                                const char *mime_type);//MIME类型

```



av_guess_format()中使用一个整型变量score记录每种输出格式的匹配程度。函数中包含一个while()循环，该循环利用函数av_oformat_next()遍历ffmpeg中所有的AVOutputFormat，并逐一计算每个输出格式的score。

```
1、如果封装格式名称匹配，score增加100，匹配中使用函数av_match_name()
2、如果mime类型匹配，score增加10，匹配直接使用字符串比较函数strcmp（）
3、如果文件名称的后缀匹配，score增加5，匹配中使用了函数av_match_ext()

```



While（）循环结束后，得到得分最高的格式，就是最匹配的格式。



flv格式的视音频复用器（Muxer）对应的AVOutputFormat格式的变量ff_flv_muxer

```cpp
AVOutputFormat ff_flv_muxer = {
    .name           = "flv",
    .long_name      = NULL_IF_CONFIG_SMALL("FLV (Flash Video)"),
    .mime_type      = "video/x-flv",
    .extensions     = "flv",
    .priv_data_size = sizeof(FLVContext),
    .audio_codec    = CONFIG_LIBMP3LAME ? AV_CODEC_ID_MP3 : AV_CODEC_ID_ADPCM_SWF,
    .video_codec    = AV_CODEC_ID_FLV1,
    .write_header   = flv_write_header,
    .write_packet   = flv_write_packet,
    .write_trailer  = flv_write_trailer,
    .codec_tag      = (const AVCodecTag* const []) {
                          flv_video_codec_ids, flv_audio_codec_ids, 0
                      },
    .flags          = AVFMT_GLOBALHEADER | AVFMT_VARIABLE_FPS |
                      AVFMT_TS_NONSTRICT,
};

```



## av_oformat_next()

```cpp

AVOutputFormat *av_oformat_next(const AVOutputFormat *f);

```



av_oformat_next()参数不为NULL的时候用于获得下一个AVOutputFormat，否则获得第一个AVOutputFormat。

```cpp
AVOutputFormat *av_oformat_next(const AVOutputFormat *f)
{
    if (f)
        return f->next;
    else
        return first_oformat;
}
```



## 调用结构图

```mermaid
graph TB
id1[avformat_alloc_output_context2]-->id2[avformat_alloc_context分配内存,初始化AVFormatContext]
id2-->id4[av_malloc,分配内存]
id2-->id5[avformat_get_context_defaults,去设置一些默认的参数]
id5-->id6[av_opt_set_defaults]
id1-->id3[av_guess_format,根据文件类型,后缀,名字等等来确定其输出格式]
id3-->id7[av_oformat_next,猜测格式]
id3-->id8[av_match_name]
id3-->id9[av_match_ext]

```











































