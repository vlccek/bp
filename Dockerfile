FROM alpine:latest

RUN apk --no-cache add cmake clang clang-dev make gcc g++ libc-dev linux-headers openmp openmp-dev ninja



# Enviroment variables
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

COPY . /app

RUN gcc --version 
RUN cd app/ && cmake -DCMAKE_BUILD_TYPE=Release -GNinja .
RUN cd app/ && ninja bp

ENTRYPOINT ["app/bp"]
