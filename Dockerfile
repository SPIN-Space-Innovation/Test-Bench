FROM ubuntu
ENV HOME /root
SHELL ["/bin/bash", "-c"]

# Update repositories
RUN apt-get update

# Install required packages
RUN apt-get install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib git python3
