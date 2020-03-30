# av_read_frame()解析一帧图像的底层调用原理

```
//得到的是一帧完整的压缩数据，完整的

1、av_read_frame()是外部接口
2、首先解封装，得到AVPacket数据
3、调用av_parser_parse2()拿到AVPacket数据，并将AVPacket数据解析组成完整的一帧未解码的压缩数据
注意：该函数只能解“裸流”，如h264，hevc等，不能解封装

4、之前需要av_parser_init()：初始化AVCodecParserContext。其参数是codec_id,所以同时只能解析一种
AVCodecParser用于解析输入的数据流并把它们分成一帧一帧的压缩编码数据。核心函数是av_parser_parse2()：


5、注意下面函数的输入和输出，返回的是输入中被使用的字节数，如果已经全部被解析，则继续读取，否则继续解析，因为函数需要保证的输出一帧完整的图像
* Parse a packet. 
 * 
 * @param s             parser context. 
 * @param avctx         codec context. 
 * @param poutbuf       set to pointer to parsed buffer or NULL if not yet finished. 
 * @param poutbuf_size  set to size of parsed buffer or zero if not yet finished. 
 * @param buf           input buffer. 
 * @param buf_size      input length, to signal EOF, this should be 0 (so that the last frame can be output). 
 * @param pts           input presentation timestamp. 
 * @param dts           input decoding timestamp. 
 * @param pos           input byte position in stream. 
 * @return the number of bytes of the input bitstream used.



int av_parser_parse2(AVCodecParserContext *s,  
                     AVCodecContext *avctx,  
                     uint8_t **poutbuf, int *poutbuf_size,  
                     const uint8_t *buf, int buf_size,  
                     int64_t pts, int64_t dts,  
                     int64_t pos);  
```



**具体示例**



```c++
//解码一个裸流文件,输入以及输出格式
int main(int argc ,char **argv){
  const char *filename,*outfilename;
  const AVCodec *codec;//结构体类型
  AVCodecParserContext *parser;
  AVCodecContext *c=NULL;
  AVPacket *pkt;//
  AVFrame *frame;
  
  //直接输入解码器的名字，如果没有给定的话，则需要调用函数去探测
  const char* codec_name;
  
  
  FILE *f;
  uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
  uint8_t *data;
  size_t   data_size;
  int ret;
  
  if (argc <= 3) {
        fprintf(stderr, "Usage: %s <input file> <output file> <codec name>\n", argv[0]);
        exit(0);
   }
  
  filename=argv[1];
  outfilename=argv[2];
  codec_name=argv[3];
  
  //给pkt初始内存，也可以不，直接后续解码，解析数据包
   pkt = av_packet_alloc();
   if (!pkt)
       exit(1);

  
  
  
  //需要打开文件，或者通过文件路径，找到解码器的名字
  codec=avcodec_find_decoder_by_name(codec_name);
   if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
  
  //解析器
  parser=av_parser_init(codec->id);
      if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }
  
  //编解码上下文
  c=avcodec_alloc_context3(codec);
  if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
   }
  
  
      /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
  
  //然后打开文件，输入一段数据
  f=fopen(filename,"rb");
  f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

  //解析文件的最后的frame，一般定义一个指针类型最好初始化，要么内存初始化，要么赋值初始化
  frame=av_frame_alloc();
  if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
  
  //持续对文件读入并解码
  while(!feof(f)){
    //从文件中读入一段数据，然后开始解析
     data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if (!data_size)
            break;

        /* use the parser to split the data into frames */
        data = inbuf;
        while (data_size > 0) {
          ret=av_parser_parse2(parser,c,&pkt->data,&pkt->size,data,data_size,AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
          
          if(ret<0){
            fprintf(stderr,"error while parsing\n");
            exit(1);
          }
         //注意，上面的解析器返回的是使用的字节数 return the number of bytes of the input bitstream used.
          data+=ret;
          data_size-=ret;
          
          if(pkt->size){
            //开始解码
            ret=avcodec_send_packet(c,pkt);
            if (ret < 0) {
        			fprintf(stderr, "Error sending a packet for decoding\n");
        			exit(1);
    				}
					//拿到一帧数据
            ret = avcodec_receive_frame(c, frame);
            
            
          }
          
  }
  
  //编码输出？或者将解码以后的图像帧转换为具体的格式存储
  av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    return 0;
}
```















