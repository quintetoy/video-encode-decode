# FFMPEG最关键的结构体之间的关系

[TOC]



## AVFrame





## AVFormatContext



## AVCodecContext



## AVIOContext



## AVCodec



## AVStream



## AVPacket







## 结构体类型分类

- 解协议

  AVIOContext，URLProtocol，URLContext主要存储视音频使用的协议的类型以及状态。

  URLProtocol存储输入视音频使用的封装格式。每种协议都对应一个URLProtocol结构

  

- 解封装(flv,avi,rmvb,mp4)

  AVFormatContext主要存储视音频封装格式中包含的信息。AVInputFormat存储输入视音频使用的封装格式。每种视音频封装格式都对应一个AVInputFormat结构。

- 解码（h264,mpeg2,aac,mp3）

  每个AVStream存储一个视频/音频流的相关数据，每个AVStream对应一个AVCodecContext，存储该视频/音频流使用解码方式的相关数据；每个AVCodecContext中对应一个AVCodec，包含该视频/音频对应的解码器。每种解码器都对应一个AVCodec结构。

- 存数据

  视频，每个结构一般是存一帧；音频可能有好几帧

  解码前数据：AVPacket

  解码后数据：AVFrame

  

以上参考博客：<https://blog.csdn.net/leixiaohua1020/article/details/11693997>





## 注意

所以的结构体都要调用合适的函数初始化，但是目前需要调用销毁函数的结构体有3个。AVFormatContext,AVFrame，AVPacket























