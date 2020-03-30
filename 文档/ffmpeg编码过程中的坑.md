# ffmpeg编码过程中的坑

==描述==

用ffmpeg解码的时候，依据最新版本的使用方法，如下：

```
avcodec_send_packet();
avcodec_receive_frame();
```

但是没有办法解码得到正确的图像，收不到解码以后的frame，总是报错，然而pkt中存在数据，解码得不到的数据原因

==原因==

- 这是由于H264中I帧，P帧，B帧解码的问题，可能出现解码错误的原因，流媒体数据出现了没有接收到I帧便开始解B帧和P帧的情况

<https://blog.csdn.net/z3237109/article/details/97234812>

- 注意，发送给解码器解码的时候，尽量保证是一帧完整的压缩图像数据，使用av_read_frame()可以保证这个条件，内部调用了av_parser_parse2（）



==解决办法==

调用老版本的解码函数

```
ret = avcodec_decode_video2(dec_ctx, frame,&got_frame, &pkt);
```





==描述==

解码mp4文件的时候出现 Error splitting the input into NAL units. 



==原因==

初始化AVCodecContext dec_ctx，不止需要通过解码器AVCodec将其初始化，其余相关的参数也需要赋值



==解决办法==

```
AVCodecID eVideoCodec=ctx->streams[pkt.stream_index]->codecpar->codec_id;
const AVCodec *codec = avcodec_find_decoder(eVideoCodec);
AVCodecContext *dec_ctx = avcodec_alloc_context3(codec);
//这一步是赋值运算，将当前的参数赋值到解码器上下文中
ret = avcodec_parameters_to_context(dec_ctx, stream->codecpar);
```





==描述==

将yuv数据转换成rgb数据，中间过程总是报错

```
[swscaler @ 0x1dea340] deprecated pixel format used, make sure you did set range correctly
[swscaler @ 0x1dea340] bad dst image pointers
段错误
```

<https://blog.csdn.net/chasing_chasing/article/details/78824299>

==原因==

初始化AVFrame *rgb的参数赋值不够，导致变换的时候报错，最后拿不到那段内存地址



==解决办法==

<https://www.cnblogs.com/nanqiang/p/10116279.html>

尽量将已知的参数全部赋值上去

```
 //将原始的pix_fmt和转换后的pix_fmt全部用真实的已知值赋值上去
struct SwsContext *img_convert_ctx = sws_getContext(nwidth, nheight, AV_PIX_FMT_YUV420P, nwidth, nheight, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
　　　　　　　　　　　　　　　　　　　　　　　 
　　　
//进行转码变换的时候，需要将AVFrame* rgb的初始化尽可能详细，详见其余的文章或者将当前的转码直接存在buffer中，开辟一段buffer内存

AVFrame *pFrameRGB;
pFrameRGB= av_frame_alloc();
pFrameRGB->format = AV_PIX_FMT_RGB24;
pFrameRGB->width = dec_ctx->width;
pFrameRGB->height = dec_ctx->height;
if (av_image_alloc(pFrameRGB->data, pFrameRGB->linesize, pFrameRGB->width, pFrameRGB->height, AV_PIX_FMT_RGB24,1) < 0)
{
	break;
}

//开始变换
sws_scale(img_convert_ctx,(uint8_t const * const *)frame->data,frame->linesize, 0, dec_ctx->height, pFrameRGB->data,pFrameRGB->linesize);


if(img_convert_ctx){
	sws_freeContext(img_convert_ctx);
	img_convert_ctx = NULL;
}
```



==描述==

调用hik的摄像头，读取rtsp流数据的时候，刚开始得不到想要的数据

```
UDP timeout,retrying with TCP
method PAUSE failed:551 Option not supported
```



==原因==

没有设置超时参数



==解决方法==

<https://www.cnblogs.com/wainiwann/p/7391229.html>

```
    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();
    AVDictionary *optionsDict = NULL;
    av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);                //采用tcp传输
    av_dict_set(&optionsDict, "stimeout", "2000000", 0);                  //如果没有设置stimeout，那么把ipc网线拔掉，av_read_frame会阻塞（时间单位是微妙）
    
    //将设置的参数赋值上去进行初始化
    int ret=avformat_open_input(&pFormatCtx, streamurl, NULL, &optionsDict);
```







