# 解码pkt到frame初始化的过程实现

[TOC]



## 背景

在图像yuv2rgb转换时，将dst, AVFrame *rgb初始化

常用的pix_fmt有

```
enum AVPixelFormat {
    AV_PIX_FMT_NONE = -1,
    AV_PIX_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    AV_PIX_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    AV_PIX_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    AV_PIX_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    AV_PIX_FMT_YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    AV_PIX_FMT_YUV444P,   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    AV_PIX_FMT_YUV410P,   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
    AV_PIX_FMT_YUV411P,   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
    AV_PIX_FMT_GRAY8,     ///<        Y        ,  8bpp
    AV_PIX_FMT_MONOWHITE, ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
    AV_PIX_FMT_MONOBLACK, ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
    AV_PIX_FMT_PAL8,      ///< 8 bits with AV_PIX_FMT_RGB32 palette
    AV_PIX_FMT_YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV420P and setting color_range
    AV_PIX_FMT_YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV422P and setting color_range
    AV_PIX_FMT_YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV444P and setting color_range
#if FF_API_XVMC
    AV_PIX_FMT_XVMC_MPEG2_MC,///< XVideo Motion Acceleration via common packet passing
....
}
```



## frame的初始化方法

参考博客
原文链接：https://blog.csdn.net/xionglifei2014/java/article/details/90693048

1.第一种（目前已经验证过可行）

       
       m_pFrameVideoOut = av_frame_alloc();
        m_pFrameVideoOut->format = AV_PIX_FMT_BGR24;
        m_pFrameVideoOut->width = m_VideoCodecCtx->width;
        m_pFrameVideoOut->height = m_VideoCodecCtx->height;
        if (av_image_alloc(m_pFrameVideoOut->data, m_pFrameVideoOut->linesize, m_pFrameVideoOut->width, m_pFrameVideoOut->height, AV_PIX_FMT_BGR24, 16) < 0)//16代表几帧？
        {
            return ;
        }

2.第二种

        m_pFrameVideoOut = av_frame_alloc();
        m_pFrameVideoOut->format = AV_PIX_FMT_BGR24;
        m_pFrameVideoOut->width = m_VideoCodecCtx->width;
        m_pFrameVideoOut->height = m_VideoCodecCtx->height;
        av_frame_get_buffer(m_pFrameVideoOut, 16);

 


3.第三种

    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, nWidth, nHeight, 1);;
    m_InputFrame = av_frame_alloc();
    m_InputFrame->width = nWidth;
    m_InputFrame->height = nHeight;
    m_InputFrame->format = AV_PIX_FMT_YUV420P;
    m_Buffer = (unsigned char *)av_malloc(bufferSize);
    av_image_fill_arrays(m_InputFrame->data, m_InputFrame->linesize, m_Buffer, AV_PIX_FMT_YUV420P, nWidth, nHeight, 1);
------------------------------------------------








## 压缩并且存储

