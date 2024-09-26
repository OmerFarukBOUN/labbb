# Use debian:slim as base image
FROM debian:12 as build

ARG BOOST_VERSION=1.84.0

# Update the system and install necessary packages
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    doxygen \
    graphviz \
    texlive \
    texlive-xetex \
    texlive-luatex \
    automake \
    autoconf \
    libtool \
    flex \
    bison \
    cmake \
    gdb \
    libfl-dev \
    libbison-dev \
    neovim


# Install CUDD
RUN git clone https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd \
    && ldconfig

# Install Boost
RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-${BOOST_VERSION} https://github.com/boostorg/boost.git /tmp/boost \
  && cd /tmp/boost \
  && ./bootstrap.sh --prefix=/usr/local --without-libraries=python \
  && ./b2 -j$(nproc) install \
  && cd / && rm -rf /tmp/boost


# Clone this repository
#RUN git clone --depth 1 --branch main https://github.com/OmerFarukBOUN/labbb.git /test

# Set the working directory

#WORKDIR /test/tests

#RUN make -s
