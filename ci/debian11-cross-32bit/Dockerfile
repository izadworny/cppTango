FROM debian:bullseye

ARG APP_UID=2000

ARG APP_GID=2000

MAINTAINER TANGO Controls team <tango@esrf.fr>

ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386

# apt-get install needs two invocations
# to avoid an unreachable package error
RUN apt-get update && apt-get install -y \
  gcc-i686-linux-gnu                     \
  && apt-get install -y                  \
  apt-utils                              \
  build-essential                        \
  cmake                                  \
  curl                                   \
  g++-multilib                           \
  gcc-multilib                           \
  git                                    \
  libcos4-dev:i386                       \
  libomniorb4-dev:i386                   \
  libomnithread4-dev:i386                \
  libzmq3-dev:i386                       \
  lsb-release                            \
  omniidl                                \
  python3

RUN apt-get install -y                                                                                                        \
    apt-transport-https                                                                                                       \
    ca-certificates                                                                                                           \
    curl                                                                                                                      \
    gnupg2                                                                                                                    \
 && mkdir -p /usr/local/share/keyrings                                                                                        \
 && curl -fsSL https://download.docker.com/linux/debian/gpg | gpg --dearmor > /usr/local/share/keyrings/docker.gpg            \
 && echo                                                                                                                      \
   "deb [arch=amd64 signed-by=/usr/local/share/keyrings/docker.gpg] https://download.docker.com/linux/debian bullseye stable" \
   > /etc/apt/sources.list.d/docker.list                                                                                      \
 && apt-get update                                                                                                            \
 && apt-get install -y docker-ce

RUN groupadd -g "$APP_GID" tango

RUN useradd -u "$APP_UID" -g "$APP_GID" -ms /bin/bash tango

RUN usermod -a -G docker tango

ENV PKG_CONFIG_PATH=/home/tango/lib/pkgconfig

USER tango

WORKDIR /home/tango
