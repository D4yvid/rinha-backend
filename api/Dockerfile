FROM alpine:3.14
# Setup user
RUN adduser www-api

# Setup compiler and libraries
RUN apk update
RUN apk add gcc

# Compile and install the project
RUN cd /apiroot && make && make install PREFIX=/var/www-api

USER www-api
WORKDIR /var/www-api
ENTRYPOINT [ "./rinha-api", "-c", "./config.cfg" ]
