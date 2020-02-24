## avio_open2()打开输入输出文件

<https://blog.csdn.net/leixiaohua1020/article/details/41199947>

```cpp
int avio_open2(AVIOContext **s, const char *url, int flags,
               const AVIOInterruptCB *int_cb, AVDictionary **options);
//s:函数嗲用成功后创建的AVIOContext
//url:输入输出协议
//flags:打开地址的方式，可以选择只读，只写，或者读写。取值如下。
//AVIO_FLAG_READ：只读。
//AVIO_FLAG_WRITE：只写。
//AVIO_FLAG_READ_WRITE：读写。

```

从avio_open2()的源代码可以看出，它主要调用了2个函数：ffurl_open()和ffio_fdopen()。其中ffurl_open()用于初始化URLContext，ffio_fdopen()用于根据URLContext初始化AVIOContext。URLContext中包含的URLProtocol完成了具体的协议读写等工作。AVIOContext则是在URLContext的读写函数外面加上了一层“包装”（通过retry_transfer_wrapper()函数）。

------------------------------------------------
版权声明：本文为CSDN博主「雷霄骅」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/leixiaohua1020/article/details/41199947