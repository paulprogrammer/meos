ARG PLAT=amd64
FROM --platform=${PLAT} alpine:3.21

RUN mkdir /work
WORKDIR /work
ENTRYPOINT ["make"]

RUN apk add --no-cache nasm gcc make
