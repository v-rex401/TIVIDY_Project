FROM gcc:13
RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*
ENV TERM=xterm-256color
WORKDIR /app
COPY . .
RUN make
CMD ["./build/tividy"]
