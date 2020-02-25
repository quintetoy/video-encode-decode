# ffmpeg,libswscale,libavfilter

[TOC]

## libswscale实现YUV转RGB

libswcale里面实现了各种图像像素格式的转换

```
主要的函数有3个：
1、sws_getContext():使用参数初始化SwsContext结构体
2、sws_scale:转换一帧图像
3、sws_freeContext:释放SwsContext
```



## 使用libavfilter为视音频添加特效功能

关键函数如下所示：

```
avfilter_register_all:注册所有AVFilter；
avfilter_graph_alloc:为FilterGraph分配内存
avfilter_graph_create_filter()：创建并向FilterGraph中添加一个Filter。
avfilter_graph_parse_ptr()：将一串通过字符串描述的Graph添加到FilterGraph中。
avfilter_graph_config()：检查FilterGraph的配置。
av_buffersrc_add_frame()：向FilterGraph中加入一个AVFrame。

av_buffersink_get_frame()：从FilterGraph中取出一个AVFrame。

```































