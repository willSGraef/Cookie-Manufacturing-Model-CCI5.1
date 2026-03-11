FROM python:3.11-slim

WORKDIR /app

# Install redis-server for containers that need it
RUN apt-get update && \
    apt-get install -y redis-server && \
    rm -rf /var/lib/apt/lists/*

# Install Python dependencies
RUN pip install --no-cache-dir pyModbusTCP redis

# Copy all simulation scripts into the image
COPY core /app/core
COPY utils /app/utils

# Default command (can be overridden per container in Compose)
CMD ["python"]