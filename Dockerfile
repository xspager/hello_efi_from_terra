FROM ubuntu:18.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update -y && apt-get install -y tzdata

RUN apt-get install -y build-essential curl mtools unzip gnu-efi ovmf libedit2

RUN groupadd user && useradd -m -g user user

RUN mkdir /hello
WORKDIR /hello
RUN chown user:user /hello

USER user
RUN curl -s -L -O https://github.com/terralang/terra/releases/download/release-1.0.0-beta3/terra-Linux-x86_64-e4ec5d1.zip
COPY terra-Linux-x86_64-e4ec5d1.zip.sha256 /hello
RUN sha256sum --quiet -c terra-Linux-x86_64-e4ec5d1.zip.sha256 && unzip -q terra-Linux-x86_64-e4ec5d1.zip

USER root
RUN mv terra-Linux-x86_64-e4ec5d1 /opt/terra-linux

USER user
RUN rm -f terra-Linux-x86_64-e4ec5d1.zip
RUN echo "PATH=$PATH:/opt/terra-linux/bin" >> ~/.bashrc
RUN /bin/bash
