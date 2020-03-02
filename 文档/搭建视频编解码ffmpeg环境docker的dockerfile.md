# 搭建视频编解码ffmpeg环境docker的dockerfile

```
FROM nvidia/cuda:10.0-devel AS builder-env

#nasm是ffmpeg所需的，其余是基本库
RUN apt-get update&&apt-get install -y \
		autoconf automake build-essential cmake libtool pkg-config nasm
		
WORKDIR /opt

COPY 3rd-party /opt/3rd-party

#这一部分是关于ffmpeg的安装命令
RUN cd 3rd-party &&\
		tar -xvf ffmpeg-4.1.4.tar.gz &&\
		cd ffmpeg-4.1.4 &&\
		./configure --enable-shared &&\
		make -j8 &&\
		make install 
	
#建立镜像
FROM builder-env AS builder
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVDIA_DRIVER_CAPABILITIES=compute,utility,video

WORKDIR /opt

#将源码拷贝到镜像中并编译
COPY CMakeLists.txt project_config.h.in /opt/
COPY include /opt/include
COPY lib /opt/lib
COPY src /opt/src

RUN mkdir build && \
		cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release .. &&\
		make -j4 &&\
		make install
		
		
# RUNTIME IMAGE
FROM nvidia/cuda:10.0-base
LABEL maintainer "dddc -- kkks<239.com.cn>"
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES=compute,utility,video

# Copy binary and libs from build image to runtime image
WORKDIR /app
ENV LD_LIBRARY_PATH /app/lib:${PATH}

COPY --from=builder /usr/local/lib lib
COPY --from=builder /opt/build/main .
COPY --from=builder /opt/sdk sdk
```































