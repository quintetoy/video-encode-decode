# avcodec_encode_video()

<https://blog.csdn.net/leixiaohua1020/article/details/44206485>

```cpp
int avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
                          const AVFrame *frame, int *got_packet_ptr);
```

```
avctx：编码器的AVCodecContext。
avpkt：编码输出的AVPacket。
frame：编码输入的AVFrame。
got_packet_ptr：成功编码一个AVPacket的时候设置为1。
```



## 函数调用关系图

```mermaid
graph TB
id1[avcodec_encode_video]-->id2[av_image_check_size]
id1-->id3[codec->encode2]
id3-->id4[encode2是一个函数指针,指向特定编码器的函数,例如libx264]
```

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

