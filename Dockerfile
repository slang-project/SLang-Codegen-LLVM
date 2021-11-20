FROM ubuntu:focal AS preinstall

ARG DEBIAN_FRONTEND=noninteractive

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


FROM preinstall AS install-clang

ARG LLVM_PROJECT_VERSION=13

RUN bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" "" ${LLVM_PROJECT_VERSION} \
    && apt-get install -y \
        "clang-format-${LLVM_PROJECT_VERSION}" \
        "clang-tidy-${LLVM_PROJECT_VERSION}" \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s "/usr/bin/clang-format-${LLVM_PROJECT_VERSION}" /usr/local/bin/clang-format \
    && ln -s "/usr/bin/clang-tidy-${LLVM_PROJECT_VERSION}" /usr/local/bin/clang-tidy

ENV CC=clang-${LLVM_PROJECT_VERSION}
ENV CXX=clang++-${LLVM_PROJECT_VERSION}


FROM install-clang AS install-vcpkg

ENV VCPKG_ROOT=/vcpkg

WORKDIR /SLang-Codegen-LLVM/
COPY vcpkg.json vcpkg.json
COPY tools/install_vcpkg.sh tools/install_vcpkg.sh
RUN tools/install_vcpkg.sh ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/vcpkg install --clean-after-build


FROM install-vcpkg AS configure-cmake

WORKDIR /SLang-Codegen-LLVM/
COPY CMakeLists.txt CMakeLists.txt
COPY app/ app/
COPY data/ data/
COPY include/ include/
COPY src/ src/

ENV SLANG_CODEGEN_BUILD_DIR=build
RUN cmake -S . -B ${SLANG_CODEGEN_BUILD_DIR} -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake


FROM configure-cmake AS build

RUN cmake --build ${SLANG_CODEGEN_BUILD_DIR} --target SlangCompilerLlvmCodegenDriver --config Debug


FROM configure-cmake AS run-format

COPY .clang-format .clang-format
RUN cmake --build ${SLANG_CODEGEN_BUILD_DIR} --target Format --config Debug


FROM configure-cmake AS run-tidy

COPY .clang-tidy .clang-tidy
RUN cmake --build ${SLANG_CODEGEN_BUILD_DIR} --target Tidy --config Debug


FROM configure-cmake AS run-unit-tests

RUN cmake --build ${SLANG_CODEGEN_BUILD_DIR} --target SlangCompilerLlvmCodegenUnitTests
RUN cmake --build ${SLANG_CODEGEN_BUILD_DIR} --target test


FROM preinstall AS install

COPY --from=build /SLang-Codegen-LLVM/build/app/SlangCompilerLlvmCodegenDriver /usr/local/bin/SlangCompilerLlvmCodegenDriver
ENTRYPOINT [ "SlangCompilerLlvmCodegenDriver" ]
