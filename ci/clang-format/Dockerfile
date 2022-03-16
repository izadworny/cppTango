FROM debian:bullseye

ARG LLVM_VERSION

ARG APP_UID=2000

ARG APP_GID=2000

MAINTAINER TANGO Controls team <tango@esrf.fr>

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
  apt-utils                              \
  curl                                   \
  git                                    \
  gnupg

RUN curl https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - &&                                                      \
    echo "deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye-${LLVM_VERSION} main" | tee -a /etc/apt/sources.list && \
    apt-get update -qq &&                                                                                                   \
    apt-get install --no-install-recommends -y                                                                              \
        clang-format-${LLVM_VERSION}

RUN groupadd -g "$APP_GID" tango

RUN useradd -u "$APP_UID" -g "$APP_GID" -ms /bin/bash tango

USER tango

WORKDIR /home/tango
