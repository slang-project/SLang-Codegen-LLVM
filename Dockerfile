FROM ubuntu:focal AS dependencies

ARG DEBIAN_FRONTEND=noninteractive
ARG LLVM_VERSION=13

WORKDIR ~

RUN apt-get update -qq \
    && apt-get upgrade -y \
    && apt-get install --no-install-recommends -y \
    && apt-get install -y \
        cmake \
        curl \
        git \
        lsb-release \
        pkg-config \
        software-properties-common \
        wget \
        zip \
    && rm -rf /var/lib/apt/lists/*

RUN bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" "" ${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*

ENV CC=clang-${LLVM_VERSION}
ENV CXX=clang++-${LLVM_VERSION}

RUN mkdir ~/SLang-LLVM/
WORKDIR ~/SLang-LLVM

COPY .git/ .git/
COPY external/ external/
COPY vcpkg.json vcpkg.json
RUN external/vcpkg/bootstrap-vcpkg.sh
RUN external/vcpkg/vcpkg install


FROM dependencies AS configure

COPY CMakeLists.txt CMakeLists.txt
COPY app/ app/
COPY include/ include/
COPY src/ src/
RUN cmake -S . -B build


FROM configure AS build

RUN cmake --build build --target SLangCompilerLlvmCodegenDriver --config Debug


FROM build AS run

RUN build/app/SLangCompilerLlvmCodegenDriver
