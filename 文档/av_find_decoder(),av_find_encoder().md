# av_find_decoder(),av_find_encoder()

声明位于libavcodec/avcodec.h

```cpp
AVCodec *avcodec_find_encoder(enum AVCodecID id);

AVCodec *avcodec_find_decoder(enum AVCodecID id);
```

函数的参数是一个编/解码器的ID，返回查找到的编/解码器（没有找到就返回NULL）

```cpp
static AVCodec *first_avcodec;

static AVCodec *find_encdec(enum AVCodecID id,int encoder)
{
  AVCodec *p,*experimental=NULL;
  p=first_avcodec;
  id=remap_deprecated_codec_id(id);
  while(p){
    if((encoder?av_codec_is_encoder(p):av_codec_is_decoder(p))&&p->id==id){
      if(p->capabilities&CODEC_CAP_EXPERIMENTAL&&!experimental){
        experimental=p;
      }else{
        return p;
      }      
    }
    p=p->next;
  }
  return experimental;
}
```









find_encdec()中有一个循环，该循环会遍历AVCodec结构的链表，逐一比较输入的ID和每一个编码器的ID，直到找到ID取值相等的编码器。

（1）first_avcodec是一个全局变量，存储AVCodec链表的第一个元素。
（2）remap_deprecated_codec_id()用于将一些过时的编码器ID映射到新的编码器ID。
（3）函数的第二个参数encoder用于确定查找编码器还是解码器。当该值为1的时候，用于查找编码器，此时会调用av_codec_is_encoder()判断AVCodec是否为编码器；当该值为0的时候，用于查找解码器，此时会调用av_codec_is_decoder()判断AVCodec是否为解码器。



### av_codec_is_encoder()

### av_codec_is_decoder()

## avcodec_find_decoder()



https://blog.csdn.net/leixiaohua1020/article/details/44084557