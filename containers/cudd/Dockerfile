FROM 0x01be/base as build

RUN apk add --no-cache --virtual cudd-build-dependencies \
    git \
    build-base
RUN apk add --no-cache --virtual cudd-doc-dependencies \
    doxygen \
    graphviz \
    texlive \
    texlive-xetex \
    texlive-luatex

ENV CUDD_REVISION=release
RUN git clone --depth 1 --branch ${CUDD_REVISION} https://github.com/ivmai/cudd.git /cudd

WORKDIR /cudd
RUN ./configure --prefix=/opt/cudd/
RUN make
RUN make install
