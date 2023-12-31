CREATE TABLE user (
  user_id BIGINT PRIMARY KEY NOT NULL,
  time_total INTEGER
);

CREATE TABLE server (
  server_id BIGINT PRIMARY KEY
);

CREATE TABLE user_server (
  user_id BIGINT,
  server_id BIGINT,
  time_total INT,
  PRIMARY KEY (user_id, server_id),
  FOREIGN KEY (user_id) REFERENCES user(user_id),
  FOREIGN KEY (server_id) REFERENCES server(server_id)
);


