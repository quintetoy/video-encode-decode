## avformat_find_stream_info()

该函数读取一部分视音频数据并且获得相关的信息。声明位于libavformat/avformat.h中。

```cpp
int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
//ic：输入的AVFormatContext。
//options：额外的选项，目前没有深入研究过。
```

函数正常执行后返回值>=0.



具体源码省略。

该函数主要用于给每个媒体流（音频/视频）的AVStream结构体赋值。它实现了解码器的查找，解码器的打开，视音频的读取，视音频的解码等工作。换句话说，该函数实际上已经走通了了解码的整个流程。

```
1.查找解码器：find_decoder()
2.打开解码器：avcodec_open2()
3.读取完整的一帧压缩编码的数据：read_frame_internal()
注：av_read_frame()内部实际上就是调用的read_frame_internal()。
4.解码一些压缩编码数据：try_decode_frame()

```

## find_decode()

用于找到合适的解码器。

如果指定的AVStream已经包含了解码器，则函数什么也不做直接返回。否则调用avcodec_find_decoder()获取解码器。





##read_frame_internal()

读取一帧压缩码流数据，





## try_decode_frame()

尝试解码一些帧

从try_decode_frame()的定义可以看出，该函数首先判断视音频流的解码器是否已经打开，如果没有打开的话，先打开相应的解码器。接下来根据视音频流类型的不同，调用不同的解码函数进行解码：视频流调用avcodec_decode_video2()，音频流调用avcodec_decode_audio4()，字幕流调用avcodec_decode_subtitle2()。解码的循环会一直持续下去直到满足了while()的所有条件。





## has_codec_parameters()

has_codec_parameters()用于检查AVStream中的成员变量是否都已经设置完毕。



## estimate_timings()

用于估算AVFormatContext以及AVStream的时长duration。



估算方法有三种：

1、通过pts（显示时间戳）。该方法调用estimate_timing_from_pts().它的基本思想就是读取视音频流中的结束位置AVPacket的PTS和起始位置的PTS，两者相减得到时长信息。

2、通过已知流的时长。该方法调用fill_all_stream_timings().

3、通过bitrate（码率）。调用estimate_timings_from_bit_rate().它的基本思想就是获得整个文件大小，以及整个文件的bitrate，两者相除之后得到时长信息。



函数：

```cpp
static void estimate_timings_from_bit_rate(AVFormatContext *ic)
{
  int64_t filesize,duration;
  int i,show_warning=0;
  AVStream *st;
  
  if(ic->bit_rate<=0){
    int bit_rate=0;
    for(i=0;i<ic->nb_streams;i++){
      st=ic->streams[i];
      if(st->codec->bit_rate>0){
        if(INT_MAX-st->codec->bit_rate<bit_rate){
          bit_rate=0;
          break;
        }
        bit_rate+=st->codec->bit_rate;
      }
    }
    ic->bit_rate=bit_rate;
  }
  
    /* if duration is already set, we believe it */
  f (ic->duration == AV_NOPTS_VALUE &&
        ic->bit_rate != 0) {
        filesize = ic->pb ? avio_size(ic->pb) : 0;
        if (filesize > ic->data_offset) {
            filesize -= ic->data_offset;
            for (i = 0; i < ic->nb_streams; i++) {
                st      = ic->streams[i];
                if (   st->time_base.num <= INT64_MAX / ic->bit_rate
                    && st->duration == AV_NOPTS_VALUE) {
                    duration = av_rescale(8 * filesize, st->time_base.den,
                                          ic->bit_rate *
                                          (int64_t) st->time_base.num);
                    st->duration = duration;
                    show_warning = 1;
                }
            }
        }
    }
    if (show_warning)
        av_log(ic, AV_LOG_WARNING,
               "Estimating duration from bitrate, this may be inaccurate\n");


  
}
```





主要的实现方式：

1、如果AVFormatContext中没有bit_rate信息，就把所有AVStream的bit_rate加起来作为AVFormatContext的bit_rate信息。

2、使用文件大小filesize除以bitrate得到时长信息。

**AVStream->duration=(filesize\*8/bit_rate)/time_base**

PS：
1）filesize乘以8是因为需要把Byte转换为Bit
2）具体的实现函数是那个av_rescale()函数。x=av_rescale(a,b,c)的含义是x=a*b/c。
3）之所以要除以time_base，是因为AVStream中的duration的单位是time_base，注意这和AVFormatContext中的duration的单位（单位是AV_TIME_BASE，固定取值为1000000）是不一样的。

https://blog.csdn.net/leixiaohua1020/article/details/44084321



























