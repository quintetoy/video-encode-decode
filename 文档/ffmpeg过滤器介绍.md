# ffmpeg过滤器介绍

<https://www.cnblogs.com/zxqstrong/p/4566495.html>

基本的过滤器使用流程是：

解码后的画面—>buffer过滤器—其它过滤器—buffersink过滤器—处理完的画面

所有的过滤器形成了过滤链，一定要的两个过滤器，buffer过滤器和buffersink过滤器。前者的作用是将解码后的画面加载到过滤器链中。后者的作用是将处理好的画面从过滤器链中读取出来。



## AVFilterGraph:管理所有的过滤器图像





## AVFilterContext：过滤器上下文



## AVFilter：过滤器



## 创建过滤器链

```cpp
//管理所有的过滤器图像
AVFilterGraph *filter_graph=avfilter_graph_alloc();


AVFilter *filter_buffer=avfilter_get_by_name("buffer");
AVFilter *filter_yadif=avfilter_get_by_name("yadif");
AVFilter *filter_buffersink=avfilter_get_by_name("buffersink");

//int avfilter_graph_create_filter(AVFilterContext **filt_ctx,const AVFilter *filt,const char *name,const char *args,void *opaque,AVFilterGraph *graph_ctx)


AVFilterContext *filter_buffer_ctx,*filter_yadif_ctx,*filter_buffersink_ctx;

        //创建buffer过滤器

        snprintf(args, sizeof(args),
            "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
            dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
            dec_ctx->time_base.num, dec_ctx->time_base.den,
            dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
        avfilter_graph_create_filter(&filter_buffer_ctx, avfilter_get_by_name("buffer"), "in",
                                       args, NULL, filter_graph);



 //创建yadif过滤器

    avfilter_graph_create_filter(&filter_yadif_ctx, avfilter_get_by_name("yadif"), "yadif", "mode=send_frame:parity=auto:deint=interlaced", NULL, filter_graph);

 //创建buffersink过滤器
		enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };

    	   		avfilter_graph_create_filter(&filter_buffersink_ctx,avfilter_get_by_name("buffersink"), "out",NULL, NULL,filter_graph);

av_opt_set_int_list(filter_buffersink_ctx, "pix_fmts", pix_fmts,  AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);



   // 第四步,连接过滤器

    avfilter_link(filter_buffer_ctx, 0, filter_yadif_ctx, 0);

    avfilter_link(filter_yadif_ctx, 0, filter_buffersink_ctx, 0);

   // 第五步,检查所有配置是否正确:

    if ((ret = avfilter_graph_config(player->filter_graph, NULL)) < 0){
            LOGE(0,"avfilter_graph_config:%d\n",ret);
            goto end;
     }


//将解码后的frame推送给过滤器链
int av_buffersrc_add_frame_flags(AVFilterContext *buffer_src,AVFrame *frame,int flags);

//将处理完的frame拉取出来
int av_buffersink_get_frame(AVFilterContext *ctx,AVFrame *frame);

//例如
av_buffersrc_add_frame_flags(filter_buffer_ctx, orgin_frame, AV_BUFFERSRC_FLAG_KEEP_REF);

    while(1){

        ret = av_buffersink_get_frame(filter_buffersink_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            break;
        }

        display(frame);

    };
```



<https://blog.csdn.net/newchenxf/article/details/51364105>



## 函数流程图



























