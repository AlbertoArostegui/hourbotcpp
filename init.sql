CREATE TABLE user (
  id INTEGER PRIMARY KEY,
  time_total INTEGER
);

CREATE TABLE server (
  id INTEGER PRIMARY KEY
);

CREATE TABLE user_server (
  user_id INTEGER,
  server_id INTEGER,
  time_total INTEGER,
  PRIMARY KEY (user_id, server_id),
  FOREIGN KEY (user_id) REFERENCES user(id),
  FOREIGN KEY (server_id) REFERENCES server(id)
);


