FROM mysql:5.7

ENV MYSQL_ROOT_PASSWORD=root
ENV MYSQL_DATABASE=users_time

ADD init.sql /docker-entrypoint-initdb.d/

