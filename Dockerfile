FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ cmake make libpq-dev libpqxx-dev git wget libasio-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.1.2/Crow-1.2.1-Linux.deb && \
    dpkg -i Crow*

COPY . .

RUN make install

CMD ["./task_tracker"]