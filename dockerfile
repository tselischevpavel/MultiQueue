FROM gcc:latest
WORKDIR ./build
COPY . .
RUN apt-get update && apt-get install -y cmake
RUN cmake .
RUN cmake --build . 
ENTRYPOINT ["./multi_queue"]