# avformat_write_header

写文件用到的3个函数。avformat_write_header,写视频文件头，av_write_frame,写视频数据，av_write_trailer写视频文件尾。

wirte_header，通过函数指针，找到不同类型格式对应的函数

