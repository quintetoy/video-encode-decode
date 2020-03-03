# ffmpeg官方文档

<https://ffmpeg.org/doxygen/4.1/modules.html>

官方网站包含Modules，Namespace，Data Structure，Files，Example



ffmpeg官方的翻译文档

<https://www.bookstack.cn/read/other-doc-cn-ffmpeg/ffmpeg-doc-cn-40.md>





## 使用官方的例子编译

```
gcc -o main decode_video.c -fPIC -I/home/xxx/videocode/ffmpeg-sdk-all/include -L/home/xxx/videocode/ffmpeg-sdk-all/lib -lavcodec
```

**Q**：/home/xxx/videocode/ffmpeg-sdk-all/lib/libavutil.so.56: 无法添加符号: DSO missing from command line

collect2: error: ld returned 1 exit status



注意编译的时候加载动态库的时候，需要加上相关的编译参数，最终编译成功的命令为

```
 gcc -o main decode_video.c -fPIC -shared -I/home/xxx/videocode/ffmpeg-sdk-all/include -L/home/xxx/videocode/ffmpeg-sdk-all/lib -lavcodec
 
 #但是编译成功的可执行文件没法用，报错 ❌ 段错误，目前不知道问题是什么
```





或者直接使用编译的exe来进行解码

```
#调用编译成功的例子
ffmpeg –i /home/xxx/videocode/video/IMG_6543.MP4  –r 1 –f image2  image-%3d.jpeg
```



## 修改可执行的代码

**examples的文件夹中metadata.c是测试代码**

将其源码拷贝出来作为main.cpp

代码为

```cpp
#ifdef __cplusplus
extern "C"
{
#endif
    
#include <stdio.h>

#include <libavformat/avformat.h>
#include <libavutil/dict.h>
    
#ifdef __cplusplus
};
#endif
int main (int argc, char **argv)
{
    AVFormatContext *fmt_ctx = NULL;
    AVDictionaryEntry *tag = NULL;
    int ret;
    
    if (argc != 2) {
        printf("usage: %s <input_file>\n"
               "example program to demonstrate the use of the libavformat metadata API.\n"
               "\n", argv[0]);
        return 1;
    }
    
    if ((ret = avformat_open_input(&fmt_ctx, argv[1], NULL, NULL)))
        return ret;
    
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        printf("%s=%s\n", tag->key, tag->value);
    
    avformat_close_input(&fmt_ctx);
    return 0;
}

```

然后编译

```
 g++ -std=c++11  -o main main.cpp -I/home/xxx/videocode/examples/include -L/home/xxx/videocode/examples/lib -lavformat -lavutil

#使用时的命令
 ./main /home/ouyaxxxeocode/video/IMG_6543.MP4
```



对于文件夹中另外一个例子，decode_video.c，同样将其摘出来作为main.cpp，借鉴上述表达，但是实际运行的时候，需要修改编译命令，虽然没有包含libavutil，但是实际编译的时候需要，否则编译不过

```
g++ -std=c++11  -o main main.cpp -I/home/xxx/videocode/examples/include -L/home/xxx/videocode/examples/lib -lavcodec -lavutil

#使用的语法
Usage: ./main <input file> <output file>
```





















