# Use the eclipse/cpp_gcc base image
FROM ubuntu:22.04

RUN apt update && apt upgrade && apt-get install -y \
    build-essential \
    gcc \
    make \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /var/webserver

# Expose the necessary ports
EXPOSE 9001
EXPOSE 9002
EXPOSE 9003
EXPOSE 9004

CMD ["bash", "start_server.sh"]