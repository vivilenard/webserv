FROM alpine:latest

RUN apk update

RUN apk add git make valgrind gcc vim nano g++

COPY ./entrypoint.sh /

ENTRYPOINT [ "/entrypoint.sh" ]