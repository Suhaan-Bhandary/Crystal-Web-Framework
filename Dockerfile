FROM ubuntu:latest
WORKDIR /app
COPY . .
EXPOSE 8080
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install build-essential
RUN apt-get -y install make
RUN make clean
RUN make all
CMD ["./build/bin/httpScratchServer"]