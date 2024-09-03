# Use debian:slim as base image
FROM debian:12 as build

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
    gdb

# Set an environment variable
ENV CUDD_REVISION=release

# Clone the CUDD repository
RUN git clone --depth 1 --branch ${CUDD_REVISION} https://github.com/ivmai/cudd.git /cudd

# Copy test files

RUN mkdir /test

COPY test/* /test/

# Change the working directory
WORKDIR /cudd

RUN autoreconf -f -i

# Configure, make, and install CUDD
RUN ./configure --prefix=/opt/cudd/
RUN ./configure --enable-dddmp --enable-obj --enable-shared --enable-static
RUN make
RUN make install

WORKDIR /test

RUN make
