# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y cmake libvolk2-dev librtlsdr-dev libconfig-dev zlib1g-dev check clang

ADD . /sdr-server
WORKDIR /sdr-server

## Build
RUN mkdir -p build
WORKDIR build
RUN cmake .. && make -j$(nproc)

## Package Stage
FROM --platform=linux/amd64 ubuntu:20.04
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y libvolk2.2 librtlsdr0 zlib1g libconfig9 libusb-1.0-0 liborc-0.4-0 libudev1
COPY --from=builder /sdr-server/build/sdr_server /sdr_server
COPY --from=builder /sdr-server/build/configuration.config /configuration.config

CMD ["/sdr_server", "/configuration.config"]
