#include <libavcodec/avcodec.h>
#define TEST_H264 1
#define TEST_HEVC 0

//输入：编解码上下文参数 AVCodecContext，
//输出：解出的帧是frame和视频包packet，以及输出的文件
static void decode(AVCodecContext *dec_ctx,AVFrame *frame,AVPacket *pkt,FILE *fp_out){
    //调用系统的函数
    char buf[1024];
    int ret;
    int i=0;
    if(!fp_out)
        return;
    
    //发送编码过的数据包
    ret=avcodec_send_packet(dec_ctx,pkt);
    if(ret<0){
        fprintf(stderr,"Error sending a packet for decoding\n");
        exit(1);
    }

    while(ret>=0){
        ret=avcodec_receive_frame(dec_ctx,frame);
        if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
            return;
        else if(ret<0){
            fprintf(stderr,"Error during decoding\n");
            exit(1);
        }
        fflush(stdout);


        //解码得到的是YUV的数据
        //ffmpeg的YUV数据存储方式是平面的方式
        for(int i=0;i<frame->height;i++){
            fwrite(frame->data[0]+frame->linesize[0]*i,1,frame->width.fp_out);

        }

        for(i=0;i<frame->height/2;i++){
            fwrite(frame->data[1]+frame->linesize[1]*i,1,frame->width/2,fp_out);
        }

        for(i=0;i<frame->height/2;i++)
            fwrite(frame->data[2]+frame->linesize[2]*i,2,frame->width/2,fp_out);

    }

}



int main(int argc,char* argv[]){
    AVCodec *pCodec;//解码器
    AVCodecContext *pCodecCtx=NULL;//解码器上下文
    AVCodecParserContext *pCodecParserCtx=NULL;//？

    FILE *fp_in;
    FILE *fp_out;
    AVFrame *pFrame;

    const int in_buffer_size=4096;
    unsigned char in_buffer[40960]={0};
    unsigned char *cur_ptr;

    int cur_size;
    AVPacket packet;//压缩包
    int ret,got_picture;

    int i=0;

#if TEST_HEVC
    enum AVCodecID codec_id=AV_CODEC_ID_HEVC;
    char filepath_in[]="bihbuckbunny_480*272.hevc";
#elif TEST_H264
    enum AVCodecID codec_id=AV_CODEC_ID_H264;//编码器的ID，可以根据ID找到相应的编码器
    char filepath_in[]="bigbuckbunny_480*272.h264";
#else
    AVCodecID codec_id=AV_CODEC_ID_MPEG2VIDEO;
    char filepath_in[]="bigbuckbunny_480*272.m2v";
#endif

    char filepath_out[]="bigbuckbunny_490*272.yuv";
    int first_time=1;

    //查找解码器
    pCodec =avcodec_find_decoder(codec_id);//根据ID来获得解码器
    if(!pCodec){
        printf("Codec not found\n");
        return -1;
    }

//为AVCodecContext分配内存并初始化
pCodecCtx=avcodec_alloc_context3(pCodec);//初始化相应的上下文
if(!pCodecCtx){
    printf("Could not allocate video codec context\n");
    return -1;
}

//初始化解析器
pCodecParserCtx=av_parser_init(codec_id);
	if (!pCodecParserCtx){
		printf("Could not allocate video parser context\n");
		return -1;
	}

//打开解码器
if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
    printf("Could not open codec\n");
    return -1;
}

//Input file
fp_in=fopen(filepath_in,"rb");
    if (!fp_in) {
        printf("Could not open input stream\n");
        return -1;
    }

fp_out=fopen(filepath_out,"wb");
if(!fp_out){
    printf("Could not open output YUV file\n");
    fclose(fp_in);
    return -1;

}


pFrame=av_frame_alloc();//初始化frame
av_init_packet(&packet);//初始化packet，设置一些参数。

while(1){
    cur_size=fread(in_buffer,1,in_buffer_size,fp_in);//将视频文件读入
    if(cur_size==0)
        break;
    cur_ptr=in_buffer;

    while(cur_size>0)
    {
        int len=av_parser_parse2(pCodecParserCtx,pCodecCtx,&packet.data,&packet.size,cur_ptr,cur_size,AV_NOPTS_VALUE,AV_NOPTS_VALUE,AV_NOPTS_VALUE,AV_NOPTS_VALUE);
        cur_ptr+=len;
        cur_size-=len;

        if(packet.size==0){
            continue;

        }
        printf("[Packet]Size:%6d\t",packet.size);
			switch(pCodecParserCtx->pict_type)
			{
				case AV_PICTURE_TYPE_I: printf("Type:I\t");break;
				case AV_PICTURE_TYPE_P: printf("Type:P\t");break;
				case AV_PICTURE_TYPE_B: printf("Type:B\t");break;
				default: printf("Type:Other\t");break;
			}
			printf("Number:%4d\n",pCodecParserCtx->output_picture_number);

		decode(pCodecCtx, pFrame, &packet, fp_out);

    }
}
//flush decoder
    decode(pCodecCtx,pFrame,NULL,fp_out);
    fclose(fp_in);
    fclose(fp_out);

    av_parser_close(pCodecParserCtx);

    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);

    return 0;



}
