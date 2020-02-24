# av_register_all()

ffmpeg注册复用器，编码器等的函数av_register_all()。该函数在所有基于ffmpeg的应用程序中几乎是第一个被调用的，只有调用了该函数，才能使用复用器，编码器等。

```
extern AVInputFormat ff_aac_demuxer;
if(CONFIG_AAC_DEMUXER) av_register_input_format(&ff_aac_demuxer);

从上面这段代码可知，真正注册的函数是av_register_input_format(&&ff_aac_demuxer).

```

```
void av_register_input_format(AVInputFormat *format)
{
  AVInputFormat **p;
  p=&first_iformat;
  while(*p!=NULL)p=&(*p)->next;
  *p=format;
  format->next=NULL;
  
}

ifirst_format是Input Format链表的头部地址，是一个全局静态变量。
static AVInputFormat *first_iformat=NULL;

函数的作用是遍历链表并把当前的Input Format加到链表的尾部
```



整个函数的源码主要完成的功能是，首先确定是不是已经初始化过了，如果没有，就调用av_register_all()注册编解码器，然后注册，。。。直到完成所有的注册。



⚠️

av_register_all()调用了avcodec_register_all().avcodec_register_all()注册了和编解码器有关的组件，硬件加速器，解码器，编码器，parser，bitstream Filter。av_register_all()还注册了复用器，解复用器，协议处理器。













