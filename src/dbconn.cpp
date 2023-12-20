#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

void log_sql(std::string message);

int handle_connection(long int user_id, uint server_id) {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  log_sql("Handling connection, user_id: " + std::to_string(user_id) +
          ", server_id: " + std::to_string(server_id));
  /* Create a connection */
  driver = get_driver_instance();
  con = driver->connect("tcp://localhost:3306", "root", "root");
  con->setSchema("users_time");

  /* Connect to the MySQL test database */
  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT user_id FROM user WHERE user_id =" +
                           std::to_string(user_id));
  if (res->next()) {
    log_sql("User id: " + std::to_string(res->getUInt("user_id")));
    log_sql("Found in the database");

  } else {
    log_sql("User not found, inserting into the db");
    stmt->execute("INSERT INTO user (user_id, time_total) VALUES (" +
                  std::to_string(user_id) + ", 0)");
  }

  res = stmt->executeQuery("SELECT server_id FROM server WHERE server_id =" +
                           std::to_string(server_id));
  if (res->next()) {
    log_sql("Server id: " + std::to_string(res->getUInt("server_id")));
    log_sql("Found in the database");
  } else {
    log_sql("Server not found, inserting into the db");
    stmt->execute("INSERT INTO server (server_id) VALUES (" +
                  std::to_string(server_id) + ")");
  }

  res = stmt->executeQuery(
      "SELECT user_id, server_id FROM user_server WHERE user_id =" +
      std::to_string(user_id) +
      " AND server_id = " + std::to_string(server_id));
  if (res->next()) {
    log_sql("User_id in user_server: " +
            std::to_string(res->getUInt("user_id")));
    log_sql("Found in the database");
  } else {
    log_sql("User server not found, inserting into the db");
    stmt->execute(
        "INSERT INTO user_server (user_id, server_id, time_total) VALUES (" +
        std::to_string(user_id) + ", " + std::to_string(server_id) + ", 0)");
  }

  delete res;
  delete stmt;
  delete con;
  return 0;
}

int handle_disconnection(long int user_id, long int server_id,
                         uint time_spent) {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  /* Create a connection */
  driver = get_driver_instance();
  con = driver->connect("tcp://localhost:3306", "root", "root");
  con->setSchema("users_time");

  log_sql("Handling disconnection, user_id: " + std::to_string(user_id) +
          ", server_id: " + std::to_string(server_id) +
          ", time_spent: " + std::to_string(time_spent));
  /* Connect to the MySQL test database */
  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT user_id FROM user WHERE user_id =" +
                           std::to_string(user_id));
  if (res->next()) {
    log_sql("User id: " + std::to_string(res->getUInt("user_id")));
    stmt->execute("UPDATE user SET time_total = time_total + " +
                  std::to_string(time_spent) +
                  " WHERE user_id = " + std::to_string(user_id));
    stmt->execute("UPDATE user_server SET time_total = time_total + " +
                  std::to_string(time_spent) +
                  " WHERE user_id = " + std::to_string(user_id) +
                  " AND server_id = " + std::to_string(server_id));

    log_sql("Updated time_total");
  } else {
    log_sql("User not found, erroring");
  }

  delete res;
  delete stmt;
  delete con;
  return 0;
}

void log_sql(std::string message) {
  std::cout << "[SQL] " << message << std::endl;
}
