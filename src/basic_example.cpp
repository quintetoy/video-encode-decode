
char *url="http://192.168.1.105/test.flv";
AVPacket pkt;//编解码后的压缩包
int ret=0;

//注册复用器、编码器等
av_register_all();
avformat_network_init();

//打开文件
AVFormatContext *fmtCtx=avformat_alloc_context();
ret=avformat_open_input(&fmtCtx,url,NULL,NULL);
ret=avformat_find_stream_info(fmtCtx,NULL);

//读取音视频数据
while(ret>=0){
    ret=av_read_frame(s,&pkt);
}




