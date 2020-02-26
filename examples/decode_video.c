/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * video decoding with libavcodec API example
 *
 * @example decode_video.c
 */

/**
1、根据Codec ID查找编码器（Codec）
2、根据编码器 ID查找解析器（Parse）。注：解析器用于解封装，解封装后的数据放到AVPacket中
3、根据Codec创建AVCodecContext。
4、循环读取输入文件，送入解析器解封装出AVPacket，然后再解码出AVFrame数据，再分别保存每一帧图像。
https://blog.csdn.net/lyy901135/article/details/95171545

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>

#define INBUF_SIZE 4096

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;

    f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    //这是原始代码，仅保留Y通道的数据，保存黑白图像
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}



//参考网友版的保存YUV数据，注意输入参数的变化https://blog.csdn.net/lyy901135/article/details/95171545
//输入的是二维数组unsigned char* char[]
//static void pgm_save(unsigned char *buf[],int wrap[],int xsize,int ysize,char *filename)
//{
//    FILE *f;
//    int i;
//
//    f=fopen(filename,"w");
//    //可使得图片直接被预览
//    fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
//
//    //fwrite函数解析size_t fwrite(const void *ptr,size_t size,size_t nmemb,FILE *stream)
////   ptr:指向要被写入的元素数组的指针，size：要被写入的每个元素的大小，以字节为单位，nmemb：元素的个数，以size为字节，stream：指向FILE对象的指针，该FILE对象指定了一个输出流
//
//    /* Save Y data */
//    for (i = 0; (wrap[0]) && (i < ysize); i++)
//        fwrite(buf[0] + i * wrap[0], 1, xsize, f);
//
//    /* Save U(Cb) data */
//    for (i = 0; (wrap[1]) && (i < ysize/2); i++)
//        fwrite(buf[1] + i * wrap[1], 1, xsize / 2, f);
//
//    /* Save V(Cr) data */
//    for (i = 0; (wrap[2]) && (i < ysize/2); i++)
//        fwrite(buf[2] + i * wrap[2], 1, xsize / 2, f);
//
//    fclose(f);
//
//}




static void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                   const char *filename)
{
    char buf[1024];
    int ret;

    //发送一个包给解码器
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    //从解码器中获得图像帧
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);

        /* the picture is allocated by the decoder. no need to
           free it */
        snprintf(buf, sizeof(buf), "%s-%d", filename, dec_ctx->frame_number);
        //原始函数保存图像帧
        //static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
        char *filename)
        pgm_save(frame->data[0], frame->linesize[0],
                 frame->width, frame->height, buf);
        
        //保存YUV420图像
//        pgm_save(frame->data, frame->linesize,
//                 frame->width, frame->height, buf);
    }
}

int main(int argc, char **argv)
{
    const char *filename, *outfilename;
    const AVCodec *codec;
    AVCodecParserContext *parser;//解析器，根据编码器ID查找解析器，解析器用于解封装，解封装后的数据放到AVPacket中
    AVCodecContext *c= NULL;
    FILE *f;
    AVFrame *frame;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];//？
    uint8_t *data;
    size_t   data_size;
    int ret;
    AVPacket *pkt;

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    filename    = argv[1];
    outfilename = argv[2];

    pkt = av_packet_alloc();//初始化
    if (!pkt)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    /* find the MPEG-1 video decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);//指定解码器
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);//找到解析器
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);//初始化解码器
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();//frame初始化
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    while (!feof(f)) {
        /* read raw data from the input file */
        data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if (!data_size)
            break;

        /* use the parser to split the data into frames */
        
        //通过解析器将数据分离成帧与帧的形式，包
        data = inbuf;
        while (data_size > 0) {
            //这一段的data的作用，读取输入的数据
            ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;

            //解码数据包
            if (pkt->size)
                decode(c, frame, pkt, outfilename);
        }
    }

    /* flush the decoder */
    
    //清空解码器
    decode(c, frame, NULL, outfilename);

    fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}
