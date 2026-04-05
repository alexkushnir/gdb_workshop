FROM ubuntu:latest AS build-env

RUN apt-get update && \
    apt-get install --no-install-recommends -y wget ca-certificates gnupg2 software-properties-common \
    build-essential vim tree git file gdb python3-pip pipx tree\
    g++ make ninja-build cmake linux-tools-common linux-tools-generic\
    libc6-dev libc6-dbg python3-gi libglib2.0-dev python3-ply linux-tools-common \
    valgrind tcpdump clang-format-18 clang-tidy-18 \
    && rm -rf /var/lib/apt/lists/*

RUN add-apt-repository universe && apt-get update && apt-get install --no-install-recommends -y gcc-14 g++-14

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 10
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 10

RUN chsh -s /bin/bash

FROM build-env AS devcontainer
RUN apt-get update && apt-get install -y openssh-server sudo && rm -rf /var/lib/apt/lists/*
RUN useradd -m devuser && echo 'devuser:devuser' | chpasswd && adduser devuser sudo
RUN mkdir /var/run/sshd
RUN echo 'PermitRootLogin no' >> /etc/ssh/sshd_config && echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config
RUN echo 'PermitUserEnvironment yes' >> /etc/ssh/sshd_config
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

RUN git config --global http.sslVerify false
RUN git clone https://github.com/alexkushnir/gdb_workshop.git /devuser/gdb_workshop
RUN git clone https://github.com/tomasz-pawelec-pl/crashfixer.git /devuser/crashfixer

ENV GLIBC_TUNABLES=glibc.cpu.hwcaps=-AVX,-AVX2,-AVX_Fast_Unaligned_Load

FROM devcontainer AS examples
WORKDIR /devuser
CMD ["/bin/bash"]