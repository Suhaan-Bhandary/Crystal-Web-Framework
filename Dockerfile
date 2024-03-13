FROM ubuntu:latest as build-stage
WORKDIR /app
EXPOSE 8080
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install build-essential
RUN apt-get -y install make
RUN apt-get -y install cmake
RUN apt-get -y install sqlite3 
RUN apt-get -y install libsqlite3-dev 
COPY . .
RUN rm -rf build/
RUN cmake -S . -B build/
RUN cd build && make

FROM ubuntu:latest as build-release-stage
WORKDIR /app
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install sqlite3 
RUN apt-get -y install libsqlite3-dev 
COPY . .
COPY --from=build-stage /app/build/example/CRYSTAL_HOME ./build/example/CRYSTAL_HOME
ENTRYPOINT ["/app/build/example/CRYSTAL_HOME"]
