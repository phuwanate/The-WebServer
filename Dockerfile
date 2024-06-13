# Use the eclipse/cpp_gcc base image
FROM eclipse/cpp_gcc

# Set the working directory inside the container
WORKDIR /var/webserver

# Copy the web server files from the host to the container
COPY ./webserver /var/webserver

# RUN chmod 777 start_server.sh

# Expose the necessary ports
EXPOSE 9000
EXPOSE 9001
EXPOSE 9002
EXPOSE 9003
EXPOSE 9004

CMD ["bash", "start_server.sh"]