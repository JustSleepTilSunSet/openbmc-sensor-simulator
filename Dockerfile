FROM alpine:3.21.7

RUN apk add --no-cache \
    openssh-client \
    unison \
    ca-certificates \
    bash

WORKDIR /host-share

CMD ["tail", "-f", "/dev/null"]