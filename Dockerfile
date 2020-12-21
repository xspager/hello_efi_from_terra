FROM ubuntu:16.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update -y && apt-get install -y tzdata

RUN apt-get install -y build-essential curl mtools unzip gnu-efi

RUN mkdir /hello
WORKDIR /hello

RUN curl -s -L -O https://github.com/zdevito/terra/releases/download/release-2016-03-25/terra-Linux-x86_64-332a506.zip 
COPY terra-Linux-x86_64-332a506.zip.sha1 /hello
RUN sha1sum --quiet -c terra-Linux-x86_64-332a506.zip.sha1 && unzip -q terra-Linux-x86_64-332a506.zip
RUN rm -f terra-Linux-x86_64-332a506.zip

RUN /bin/bash
