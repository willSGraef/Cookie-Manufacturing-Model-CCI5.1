# Base image
FROM python:3.11-slim AS base
WORKDIR /app
RUN pip install --no-cache-dir pyModbusTCP redis
COPY core /app/core
COPY utils /app/utils

# Factory devices
FROM base AS factory

# Redis server, install redis dependencies
FROM base AS redis
RUN apt-get update && \
    apt-get install -y redis-server && \
    rm -rf /var/lib/apt/lists/*