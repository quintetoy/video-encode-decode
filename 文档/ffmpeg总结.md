# ffmpeg总结

[TOC]

<https://www.cnblogs.com/renhui/p/6922971.html>

## ffmpeg介绍

ffmpeg是一套可以用来记录，转换数字音频，视频，并能将其转化为流的开源计算机程序。采用LGPL或GPL许可证。它提供了录制，转换，以及流化音视频的完整解决方案。

## ffmpeg组成

- libavformat：用于各种音视频封装格式的生成和解析，包括获取解码所需信息以生成解码上下文结构和读取音视频帧等功能，各种流媒体协议代码以及音视频格式的（解）复用代码；
- libavcodec：用于各种类型声音/图像编解码
- libavutil：包含一些公共的工具函数，包括算数运算，字符操作等
- libswscale：用于视频场景比例缩放，色彩映射转换
- libswresample:提供音频重采样，采样格式转换和混合等功能。
- libavfilter：各种音视频滤波器
- libpostproc：用于后期效果处理，如图像的去块效应等
- libavdevice:用于硬件的视音频采集。加速和显示
- ffmpeg：该项目提供的一个工具，可用于格式转换，解码或电视卡即时编码
- ffserver：一个http多媒体即时广播串流服务器
- ffplay：一个简单的播放器，使用ffmpeg库解析和解码，通过SDL显示



⚠️

libavformat、libavcodec、libavutil，它们提供了音视频开发的最基本功能，应用范围最广。



## 常见结构说明

### 1、封装格式

- AVFormatContext 描述了媒体文件的构成及基本信息，是统领全局的基本结构体，
- AVInputFormat 解复用器对象，每种作为输入的封装格式（flv，mp4,TS等）对应一个该结构体，具体的格式不同 ，如libavformat/flvdec.c的ff_flv_demuxer
- AVOutputFormat 复用器对象，每种作为输出的封装格式（flv，mp4，ts等）对应一个该结构体，如libavformat/flvenc.c的ff_flv_muxer
- AVStream 用于描述一个视频/音频流的相关数据信息



### 2、编解码



- AVCodecContext 描述编解码器上下文的数据结构，包含众多编解码器的参数信息
- AVCodec 编解码器对象，每种编解码格式(H264,AAC等)对应一个结构体，如libavcodec/aacdec.c的ff_aac_decoder.每个AVCodecContext中含有一个AVCodec。得到packet
- AVCodecParameters 编解码参数。每个AVStream中都含有一个AVCodecParameters，用来存放当前流的编解码参数



### 3、网络协议



- AVIOContext 管理输入输出数据的结构体
- URLProtocol 描述了音视频数据传输所使用的协议，每种传输协议（http，rtmp）等，都会对应一个URLProtocol结构。如libavformat/http.c中的ff_http_protocol;
- URLContext 封装了协议对象及协议操作对象



### 4、数据存放



- AVPacket 存放编码后，解码前的数据，即ES数据
- AVFrame 存放编码前，解码后的原始数据，如YUV或PCM格式数据













