## avformat_open_input()，ffmpeg的灵魂

<https://blog.csdn.net/leixiaohua1020/article/details/44064715>

函数位于libavformat/utils.c

```c++

int avformat_open_input(AVFormatContext **ps, const char *filename,
                        AVInputFormat *fmt, AVDictionary **options)
{
    AVFormatContext *s = *ps;
    int ret = 0;
    AVDictionary *tmp = NULL;
    ID3v2ExtraMeta *id3v2_extra_meta = NULL;
 
    if (!s && !(s = avformat_alloc_context()))
        return AVERROR(ENOMEM);
    if (!s->av_class) {
        av_log(NULL, AV_LOG_ERROR, "Input context has not been properly allocated by avformat_alloc_context() and is not NULL either\n");
        return AVERROR(EINVAL);
    }
    if (fmt)
        s->iformat = fmt;
 
    if (options)
        av_dict_copy(&tmp, *options, 0);
 
    if ((ret = av_opt_set_dict(s, &tmp)) < 0)
        goto fail;
 
    if ((ret = init_input(s, filename, &tmp)) < 0)//关键函数1
        goto fail;
    s->probe_score = ret;
 
    if (s->format_whitelist && av_match_list(s->iformat->name, s->format_whitelist, ',') <= 0) {
        av_log(s, AV_LOG_ERROR, "Format not on whitelist\n");
        ret = AVERROR(EINVAL);
        goto fail;
    }
 
    avio_skip(s->pb, s->skip_initial_bytes);
 
    /* Check filename in case an image number is expected. */
    if (s->iformat->flags & AVFMT_NEEDNUMBER) {
        if (!av_filename_number_test(filename)) {
            ret = AVERROR(EINVAL);
            goto fail;
        }
    }
 
    s->duration = s->start_time = AV_NOPTS_VALUE;
    av_strlcpy(s->filename, filename ? filename : "", sizeof(s->filename));
 
    /* Allocate private data. */
    if (s->iformat->priv_data_size > 0) {
        if (!(s->priv_data = av_mallocz(s->iformat->priv_data_size))) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }
        if (s->iformat->priv_class) {
            *(const AVClass **) s->priv_data = s->iformat->priv_class;
            av_opt_set_defaults(s->priv_data);
            if ((ret = av_opt_set_dict(s->priv_data, &tmp)) < 0)
                goto fail;
        }
    }
 
    /* e.g. AVFMT_NOFILE formats will not have a AVIOContext */
    if (s->pb)
        ff_id3v2_read(s, ID3v2_DEFAULT_MAGIC, &id3v2_extra_meta, 0);
 
    if (!(s->flags&AVFMT_FLAG_PRIV_OPT) && s->iformat->read_header)//关键函数2
        if ((ret = s->iformat->read_header(s)) < 0)
            goto fail;
 
    if (id3v2_extra_meta) {
        if (!strcmp(s->iformat->name, "mp3") || !strcmp(s->iformat->name, "aac") ||
            !strcmp(s->iformat->name, "tta")) {
            if ((ret = ff_id3v2_parse_apic(s, &id3v2_extra_meta)) < 0)
                goto fail;
        } else
            av_log(s, AV_LOG_DEBUG, "demuxer does not support additional id3 data, skipping\n");
    }
    ff_id3v2_free_extra_meta(&id3v2_extra_meta);
 
    if ((ret = avformat_queue_attached_pictures(s)) < 0)
        goto fail;
 
    if (!(s->flags&AVFMT_FLAG_PRIV_OPT) && s->pb && !s->data_offset)
        s->data_offset = avio_tell(s->pb);
 
    s->raw_packet_buffer_remaining_size = RAW_PACKET_BUFFER_SIZE;
 
    if (options) {
        av_dict_free(options);
        *options = tmp;
    }
    *ps = s;
    return 0;
 
fail:
    ff_id3v2_free_extra_meta(&id3v2_extra_meta);
    av_dict_free(&tmp);
    if (s->pb && !(s->flags & AVFMT_FLAG_CUSTOM_IO))
        avio_close(s->pb);
    avformat_free_context(s);
    *ps = NULL;
    return ret;
}

```

```
init_input():绝大部分初始化工作在此完成
s->iformat->read_header():读取多媒体数据文件头，根据音视频创建相应的AVStream
```



## init_input(),打开输入的视频数据并且探测视频的格式

```c++
static int init_input(AVFormatContext *s,const char *filename,AVDicitonary **options)
{
  int ret;
  AVProbeData pd={filename,NULL,0};//
  int score=AVPROBE_SCORE_RETRY;//一个判决AVInputFormat的分数的门限值，如果低于待分值，就认为没有找到合适的AVInputFormat
  
  if(s->pb){//如果AVFormatContext的AVIOContext不为NULL
    s->flags |=AVFMT_FLAG_CUNSTOM_IO;
    if(!s->iformat)//不知道格式的时候，则用函数探测
      return av_probe_input_buffer2(s->pb,&s->iformat,filename,s,0,s->format_probesize);
    else if(s->iformat->flags&AVFMT_NOFILE)
      av_log(s, AV_LOG_WARNING, "Custom AVIOContext makes no sense and "
                                      "will be ignored with AVFMT_NOFILE format.\n")
    return 0;
  }
  //根据文件路径判断文件格式
  if((s->iformat&&s->iformat->flags&AVFMT_NOFILE)||(!s->iformat&&(s->ifromat=av_probe_input_format2(&pd,0,&score))))
    return score;
  
  if((ret=avio_open2(&s->pb,filename,AVIO_FLAG_READ|s->avio_flags,&s->interrupt_callback,options))<0)//通过文件路径无法判断，直接打开文件
    return ret;
  
  if(s->iformat)//指定了就直接返回？
    return 0;
  
  return av_probe_input_buffer2(s->pb,&s->iformat,filename,s,0,s->format_probesize);
  //然后查找格式
}
```



```
#define AVPROBE_SCORE_RETRY (AVPROBE_SCORE_MAX/4)
#define AVPROBE_SCORE_MAX       100 ///< maximum score
//score取值是25，即如果推测后得到的最佳AVInputFormat的分值低于25，就认为没有找到合适的AVInputFormat。
```



整体逻辑：

```
1、当使用了自定义的AVIOContext的时候（AVFormatContext中的AVIOContext不为空，即s->pb!=NULL），如果指定了AVInputFormat就直接返回，如果没有指定就调用av_probe_input_buffer2()推测AVInputFormat。这一情况出现的不算很多，但是当我们从内存中读取数据的时候（需要初始化自定义的AVIOContext），就会执行这一步骤。
2、在更一般的情况下，如果已经指定了AVInputFormat，就直接返回；如果没有指定，就调用av_probe_input_format(NULL,…)根据文件路径判断文件格式。这里特意把av_probe_input_format()的第1个参数写成“NULL”，是为了强调这个时候实际上并没有给函数提供输入数据，此时仅仅通过文件路径推测AVInputFormat。
3、如果发现通过文件路径判断不出来文件格式，那么就需要打开文件探测文件格式了，这个时候会首先调用avio_open2()打开文件，然后调用av_probe_input_buffer2()推测AVInputFormat。
```







```
该函数最主要的部分是一个循环，该循环调用av_iformat_next()遍历ffmpeg中所有的AVInputFormat.并根据以下规则确定AVInputFormat和输入媒体数据的匹配分数(score,反应匹配程度)


（1）如果AVInputFormat中包含read_probe()，就调用read_probe()函数获取匹配分数（这一方法如果结果匹配的话，一般会获得AVPROBE_SCORE_MAX的分值，即100分）。如果不包含该函数，就使用av_match_ext()函数比较输入媒体的扩展名和AVInputFormat的扩展名是否匹配，如果匹配的话，设定匹配分数为AVPROBE_SCORE_EXTENSION（AVPROBE_SCORE_EXTENSION取值为50，即50分）。
（2）使用av_match_name()比较输入媒体的mime_type和AVInputFormat的mime_type，如果匹配的话，设定匹配分数为AVPROBE_SCORE_MIME（AVPROBE_SCORE_MIME取值为75，即75分）。
（3）如果该AVInputFormat的匹配分数大于此前的最大匹配分数，则记录当前的匹配分数为最大匹配分数，并且记录当前的AVInputFormat为最佳匹配的AVInputFormat。


```



## AVInputFormat->read_probe()

AVInputFormat中包含read_probe()是用于获得匹配函数的函数指针，不同的封装格式包含不同的实现函数。例如，FLV封装格式的AVInputFormat模块定义（位于libavformat\flvdec.c）如下所示。

```cpp
AVInputFormat ff_flv_demuxer = {
    .name           = "flv",
    .long_name      = NULL_IF_CONFIG_SMALL("FLV (Flash Video)"),
    .priv_data_size = sizeof(FLVContext),
    .read_probe     = flv_probe,
    .read_header    = flv_read_header,
    .read_packet    = flv_read_packet,
    .read_seek      = flv_read_seek,
    .read_close     = flv_read_close,
    .extensions     = "flv",
    .priv_class     = &flv_class,
};

```



## av_probe_input_buffer2()

它根据输入的媒体数据推测该媒体数据的AVInputFormat. 

```cpp
int av_probe_input_buffer2(AVIOContext *pb, AVInputFormat **fmt,
                           const char *filename, void *logctx,
                           unsigned int offset, unsigned int max_probe_size);

//pb：用于读取数据的AVIOContext。
//fmt：输出推测出来的AVInputFormat。
//filename：输入媒体的路径。
//logctx：日志（没有研究过）。
//offset：开始推测AVInputFormat的偏移量。
//max_probe_size：用于推测格式的媒体数据的最大值。

//返回推测后的得到的AVInputFormat的匹配分数

//函数首先需要确定用于推测格式的媒体数据的最大值max_probe_size.它默认为PROBE_BUF_MAX(PROBE_BUF_MAX取值为1<<20)

//在确定了max_probe_size之后，函数就会进入到一个循环中，调用avio_read()读取数据并且使用av_probe_input_format2()推测文件格式。


```



## AVInputFormat->read_header()

在调用完init_input()完成基本初始化并且推测得到相应的AVInputFormat之后，avformat_open_input()会调用AVInputFormat的read_header()方法读取媒体文件的文件头并且完成相关的初始化工作。read_header()是一个函数指针，对于不同的封装格式，会调用不同的read_header()的实现函数。



以flv举例，函数读取了FLV的文件头并且判断其中是否包含视频流和音频流，如果包含，就会调用create_stream()函数。

















