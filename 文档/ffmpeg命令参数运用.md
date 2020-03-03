# ffmpeg命令参数运用

<https://www.jianshu.com/p/a035056cbbf9>

## 分离音视频流

```
ffmpeg -i input_file -vcodec copy -an output_file_video 

ffmpeg -i input_file -acodec copy -vn output_file_audio
```



## 视频解复用



```
ffmpeg -i test.mp4 -vcodec copy -an -f m4v test.264

ffmpeg -i test.avi -vcodec copy -an -f m4v test.264

```



## 视频转码

```
ffmpeg -i test.mp4 -vcodec h264 -s 352*278 -an -f m4v test.264

//转码为码流原始文件

ffmpeg –i test.mp4 –vcodec h264 –bf 0 –g 25 –s 352*278 –an –f m4v test.264 //转码为码流原始文件

ffmpeg –i test.avi -vcodec mpeg4 –vtag xvid –qsame test_xvid.avi //转码为封装文件

说明：-bf B帧数目控制，-g 关键帧间隔控制，-s 分辨率控制


```



## 视频封装

```
ffmpeg -i video_file -i audio_file -vcodec copy -acodec copy output_file
```



## 视频剪切

```
ffmpeg -i test.avi -r 1 -f image2 image-%3d.jpeg //提取图片

ffmpeg -ss 0:1:30 -t 0:0:20 -i input.avi -vcodec copy -acodec copy output.avi //剪切视频//-r 提取图像的频率，-ss 开始时间，-t 持续时间


```



## 视频录制

```
ffmpeg -i rtsp://192.168.3.205:5555/test –vcodec copy out.avi
```



## 利用ffmpeg视频切片

把视频源切成若干个.ts格式的视频片段然后生成一个.m3u8的切片文件索引提供给html5的video做hls直播源

```
ffmpeg -i 视频源地址 -strict -2 -c:v libx264 -c:a aac -f hls m3u8文件输出地址
```





**8、ffmpeg缩放视频**

假设原始视频尺寸是 1080p（即 1920×1080 px，16:9），使用下面命令可以缩小到 480p：

命令如下：

ffmpeg -i 视频源地址 -vf scale=853:480 -acodec aac -vcodec h264 视频输出地址（如：out.mp4）

各个参数的含义：-i a.mov 指定待处理视频的文件名-vf scale=853:480 vf 参数用于指定视频滤镜，其中 scale 表示缩放，后面的数字表示缩放至 853×480 px，其中的 853px 是计算而得，因为原始视频的宽高比为 16:9，所以为了让目标视频的高度为 480px，则宽度 = 480 x 9 / 16 = 853-acodec aac 指定音频使用 aac 编码。注：因为 ffmpeg 的内置 aac 编码目前（写这篇文章时）还是试验阶段，故会提示添加参数 “-strict -2” 才能继续，尽管添加即可。又或者使用外部的 libfaac（需要重新编译 ffmpeg）。-vcodec h264 指定视频使用 h264 编码。注：目前手机一般视频拍摄的格式（封装格式、文件格式）为 mov 或者 mp4，这两者的音频编码都是 aac，视频都是 h264。out.mp4 指定输出文件名上面的参数 scale=853:480 当中的宽度和高度实际应用场景中通常只需指定一个，比如指定高度为 480 或者 720，至于宽度则可以传入 “-1” 表示由原始视频的宽高比自动计算而得。即参数可以写为：scale=-1:480，当然也可以 scale=480:-1

**9、ffmpeg裁剪**

有时可能只需要视频的正中一块，而两头的内容不需要，这时可以对视频进行裁剪（crop），比如有一个竖向的视频 1080 x 1920，如果指向保留中间 1080×1080 部分命令如下：ffmpeg -i 视频源地址 -strict -2 -vf crop=1080:1080:0:420 视频输出地址（如：out.mp4）

其中的 crop=1080:1080:0:420 才裁剪参数，具体含义是 crop=width:height:x:y，其中 width 和 height 表示裁剪后的尺寸，x:y 表示裁剪区域的左上角坐标。比如当前这个示例，我们只需要保留竖向视频的中间部分，所以 x 不用偏移，故传入0，而 y 则需要向下偏移：(1920 – 1080) / 2 = 420

**10. 转视频格式**

ffmpeng -i source.mp4 -c:v libx264 -crf 24 destination.flv

其中 -crf 很重要，是控制转码后视频的质量，质量越高，文件也就越大。

此值的范围是 0 到 51：0 表示高清无损；23 是默认值（如果没有指定此参数）；51 虽然文件最小，但效果是最差的。

值越小，质量越高，但文件也越大，建议的值范围是 18 到 28。而值 18 是视觉上看起来无损或接近无损的，当然不代表是数据（技术上）的转码无损。

第二组

**1.ffmpeg 把文件当做直播推送至服务器 (RTMP + FLV)**

ffmpeg - re -i demo.mp4 -c copy - f flv rtmp://w.gslb.letv/live/streamid

**2.将直播的媒体保存到本地**

ffmpeg -i rtmp://r.glsb.letv/live/streamid -c copy streamfile.flv

**3.将一个直播流，视频改用h264压缩，音频改用faac压缩，送至另一个直播服务器**

ffmpeg -i rtmp://r.glsb.letv/live/streamidA -c:a libfaac -ar 44100 -ab 48k -c:v libx264 -vpre slow -vpre baseline -f flv rtmp://w.glsb.letv/live/streamb

**4.提取视频中的音频,并保存为mp3 然后输出**

ffmpeg -i input.avi -b:a 128k output.mp3

第三组

**1.获取视频的信息**

ffmpeg -i video.avi

**2.将图片序列合成视频**

ffmpeg -f image2 -i image%d.jpg video.mpg

上面的命令会把当前目录下的图片（名字如：image1.jpg. image2.jpg. 等...）合并成video.mpg

**3.将视频分解成图片序列**

ffmpeg -i video.mpg image%d.jpg

上面的命令会生成image1.jpg. image2.jpg. ...

支持的图片格式有：PGM. PPM. PAM. PGMYUV. JPEG. GIF. PNG. TIFF. SGI

**4.为视频重新编码以适合在iPod/iPhone上播放**

ffmpeg -i source_video.avi input -acodec aac -ab 128kb -vcodec mpeg4 -b 1200kb -mbd 2 -flags +4mv+trell -aic 2 -cmp 2 -subcmp 2 -s 320x180 -title X final_video.mp4

**5.为视频重新编码以适合在PSP上播放**

ffmpeg -i source_video.avi -b 300 -s 320x240 -vcodec xvid -ab 32 -ar 24000 -acodec aac final_video.mp4

**6.从视频抽出声音.并存为Mp3**

ffmpeg -i source_video.avi -vn -ar 44100 -ac 2 -ab 192 -f mp3 sound.mp3

**7.将wav文件转成Mp3**

ffmpeg -i son_origine.avi -vn -ar 44100 -ac 2 -ab 192 -f mp3 son_final.mp3

8.将.avi视频转成.mpg

ffmpeg -i video_origine.avi video_finale.mpg

9.将.mpg转成.avi

ffmpeg -i video_origine.mpg video_finale.avi

10.将.avi转成gif动画（未压缩）

ffmpeg -i video_origine.avi gif_anime.gif

11.合成视频和音频

ffmpeg -i son.wav -i video_origine.avi video_finale.mpg

12.将.avi转成.flv

ffmpeg -i video_origine.avi -ab 56 -ar 44100 -b 200 -r 15 -s 320x240 -f flv video_finale.flv

13.将.avi转成dv

ffmpeg -i video_origine.avi -s pal -r pal -aspect 4:3 -ar 48000 -ac 2 video_finale.dv

或者：

ffmpeg -i video_origine.avi -target pal-dv video_finale.dv

14.将.avi压缩成divx

ffmpeg -i video_origine.avi -s 320x240 -vcodec msmpeg4v2 video_finale.avi

15.将Ogg Theora压缩成Mpeg dvd

ffmpeg -i film_sortie_cinelerra.ogm -s 720x576 -vcodec mpeg2video -acodec mp3 film_terminate.mpg

16.将.avi压缩成SVCD mpeg2

NTSC格式：

ffmpeg -i video_origine.avi -target ntsc-svcd video_finale.mpg

PAL格式：

ffmpeg -i video_origine.avi -target pal-dvcd video_finale.mpg

17.将.avi压缩成VCD mpeg2

NTSC格式：

ffmpeg -i video_origine.avi -target ntsc-vcd video_finale.mpg

PAL格式：

ffmpeg -i video_origine.avi -target pal-vcd video_finale.mpg

18.多通道编码

ffmpeg -i fichierentree -pass 2 -passlogfile ffmpeg2pass fichiersortie-2

**19.从flv提取mp3**

ffmpeg -i source.flv -ab 128k dest.mp3

第四组

**1、将文件当做直播送至live**

ffmpeg -re -i localFile.mp4 -c copy -f flv rtmp://server/live/streamName

**2、将直播媒体保存至本地文件**

ffmpeg -i rtmp://server/live/streamName -c copy dump.flv

**3、将其中一个直播流，视频改用h264压缩，音频不变，送至另外一个直播服务流**

ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv rtmp://server/live/h264Stream

**4、将其中一个直播流，视频改用h264压缩，音频改用faac压缩，送至另外一个直播服务流**

ffmpeg -i rtmp://server/live/originalStream -c:a libfaac -ar 44100 -ab 48k -c:v libx264 -vpre slow -vpre baseline -f flv rtmp://server/live/h264Stream

**5、将其中一个直播流，视频不变，音频改用faac压缩，送至另外一个直播服务流**

ffmpeg -i rtmp://server/live/originalStream -acodec libfaac -ar 44100 -ab 48k -vcodec copy -f flv rtmp://server/live/h264_AAC_Stream

**6、将一个高清流，复制为几个不同视频清晰度的流重新发布，其中音频不变**

ffmpeg -re -i rtmp://server/live/high_FMLE_stream -acodec copy -vcodec x264lib -s 640×360 -b 500k -vpre medium -vpre baseline rtmp://server/live/baseline_500k -acodec copy -vcodec x264lib -s 480×272 -b 300k -vpre medium -vpre baseline rtmp://server/live/baseline_300k -acodec copy -vcodec x264lib -s 320×200 -b 150k -vpre medium -vpre baseline rtmp://server/live/baseline_150k -acodec libfaac -vn -ab 48k rtmp://server/live/audio_only_AAC_48k

**7、功能一样，只是采用-x264opts选项**

ffmpeg -re -i rtmp://server/live/high_FMLE_stream -c:a copy -c:v x264lib -s 640×360 -x264opts bitrate=500:profile=baseline:preset=slow rtmp://server/live/baseline_500k -c:a copy -c:v x264lib -s 480×272 -x264opts bitrate=300:profile=baseline:preset=slow rtmp://server/live/baseline_300k -c:a copy -c:v x264lib -s 320×200 -x264opts bitrate=150:profile=baseline:preset=slow rtmp://server/live/baseline_150k -c:a libfaac -vn -b:a 48k rtmp://server/live/audio_only_AAC_48k

**8、将当前摄像头及音频通过DSSHOW采集，视频h264、音频faac压缩后发布**

ffmpeg -r 25 -f dshow -s 640×480 -i video=”video source name”:audio=”audio source name” -vcodec libx264 -b 600k -vpre slow -acodec libfaac -ab 128k -f flv rtmp://server/application/stream_name

**9、将一个JPG图片经过h264压缩循环输出为mp4视频**

ffmpeg.exe -i INPUT.jpg -an -vcodec libx264 -coder 1 -flags +loop -cmp +chroma -subq 10 -qcomp 0.6 -qmin 10 -qmax 51 -qdiff 4 -flags2 +dct8x8 -trellis 2 -partitions +parti8x8+parti4x4 -crf 24 -threads 0 -r 25 -g 25 -y OUTPUT.mp4

**10、将普通流视频改用h264压缩，音频不变，送至高清流服务(新版本FMS live=1)**

ffmpeg -i rtmp://server/live/originalStream -c:a copy -c:v libx264 -vpre slow -f flv “rtmp://server/live/h264Stream live=1〃























