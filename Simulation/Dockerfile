FROM python:3.11-slim

WORKDIR /app

# Install redis server
RUN apt-get update && \
    apt-get install -y redis-server && \
    rm -rf /var/lib/apt/lists/*

# Install required python libraries
RUN pip install --no-cache-dir pyModbusTCP redis

CMD ["python"]