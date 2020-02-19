FROM ubuntu:18.04

#https://github.com/opennetworkinglab/ODTN-emulator/blob/master/emulator-oc-lumentum/Dockerfile

RUN \
      apt-get update && apt-get install -y \
      # general tools
      git \
      cmake \
      build-essential \
      vim \
      supervisor \
      # libyang
      libpcre3-dev \
      pkg-config \
      # sysrepo
      libavl-dev \
      libev-dev \
      libprotobuf-c-dev \
      protobuf-c-compiler \
      # netopeer2 \
      libssh-dev \
      libssl-dev \
      # bindings
      swig \
      python-dev

# add netconf user
RUN \
    adduser --system netconf && \
    echo "netconf:netconf" | chpasswd

# generate ssh keys for netconf user
RUN \
    mkdir -p /home/netconf/.ssh && \
    ssh-keygen -A && \
    ssh-keygen -t dsa -P '' -f /home/netconf/.ssh/id_dsa && \
    cat /home/netconf/.ssh/id_dsa.pub > /home/netconf/.ssh/authorized_keys

# use /opt/dev as working directory
RUN mkdir /opt/dev
WORKDIR /opt/dev

# libyang
RUN \
      git clone https://github.com/CESNET/libyang.git && \
      cd libyang && git checkout v0.16-r3 && \
      mkdir build && cd build && cmake .. && \
      make -j4 && make install && ldconfig

# libnetconf2
RUN \
      git clone https://github.com/CESNET/libnetconf2.git && \
      cd libnetconf2 && git checkout v0.12-r1 && \
      mkdir build && cd build && cmake -DENABLE_BUILD_TESTS=OFF -DENABLE_SSH=ON .. && \
      make -j4 && make install && ldconfig

# sysrepo
RUN \
      git clone https://github.com/sysrepo/sysrepo.git && \
      cd sysrepo && git checkout v0.7.7 && \
      mkdir build && cd build && \
      cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr .. && \
      make -j4 && make install && ldconfig

# netopeer2
RUN \
      cd /opt/dev && \
      git clone https://github.com/CESNET/Netopeer2.git && \
      cd Netopeer2/server && git checkout v0.7-r1 && \
      mkdir build && cd build && cmake .. && \
      make -j4 && make install

COPY cnc_code /opt/dev/cnc_code
RUN \
      cd /opt/dev && \
      cd cnc_code && mkdir build  && cd build && \
      cmake .. && \
      make -j2 && \
      make install

ENV EDITOR vim
EXPOSE 830

COPY supervisord.conf /etc/supervisord.conf
CMD ["/usr/bin/supervisord", "-c", "/etc/supervisord.conf"]