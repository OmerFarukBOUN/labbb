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
#ENV CUDD_REVISION=release

# Clone the CUDD repository
#RUN git clone --depth 1 --branch ${CUDD_REVISION} https://github.com/ivmai/cudd.git /cudd

RUN git clone https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd


ARG bruh=bruh

# Clone this repository
RUN git clone --depth 1 --branch main https://github.com/OmerFarukBOUN/labbb.git /test


# Change the working directory
#WORKDIR /cudd

#RUN autoreconf -f -i

# Configure, make, and install CUDD
#RUN ./configure --prefix=/opt/cudd/ && aclocal && autoconf
#RUN ./configure --enable-obj --enable-shared
#RUN make
#RUN make install

WORKDIR /test/tests

RUN make
