Hello EFI from Terra
====================

Proof of concept running [Terra](http://terralang.org/) code on the (U)EFI environment

You will need the Terra release for Linux (x86-64) https://github.com/zdevito/terra/releases/download/release-2016-03-25/terra-Linux-x86_64-332a506.zip and the GNU-EFI package, on Debian:

```bash
$ sudo apt-get install gnu-efi
```

There's a Dockerfile that should set things up to build it:

```bash
$ docker build . -t terra
$ docker run -v `realpath ./`:/hello -it terra
```

You will also need to install QEmu and the ovmf package. Please open an Issue if I missed something.

![Screenshot](screenshot.png)
