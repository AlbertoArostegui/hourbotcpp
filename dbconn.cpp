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

  /* Create a connection */
  driver = get_driver_instance();
  con = driver->connect("tcp://localhost:3306", "root", "example");
  con->setSchema("users_time");

  /* Connect to the MySQL test database */
  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT id FROM user WHERE id =" +
                           std::to_string(user_id));
  if (res->next()) {
    log_sql("User id: " + std::to_string(res->getInt("id")));
    log_sql("Found in the database");

  } else {
    log_sql("User not found, inserting into the db");
    stmt->execute("INSERT INTO user (id, time_total) VALUES (" +
                  std::to_string(user_id) + ", 0)");
  }

  res = stmt->executeQuery("SELECT id FROM server WHERE id =" +
                           std::to_string(server_id));
  if (res->next()) {
    log_sql("Server id: " + std::to_string(res->getInt("id")));
    log_sql("Found in the database");
  } else {
    log_sql("Server not found, inserting into the db");
    stmt->execute("INSERT INTO server (id) VALUES (" +
                  std::to_string(server_id) + ")");
  }

  delete res;
  delete stmt;
  delete con;
  return 0;
}

int handle_disconnection(long int user_id) {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  /* Create a connection */
  driver = get_driver_instance();
  con = driver->connect("tcp://localhost:3306", "root", "example");
  con->setSchema("users_time");

  /* Connect to the MySQL test database */
  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT id FROM user WHERE id =" +
                           std::to_string(user_id));
  if (res->next()) {
    std::cout << "user id: " << res->getInt("id") << std::endl;
  } else {
    std::cout << "User not found in the database" << std::endl;
  }

  delete res;
  delete stmt;
  delete con;
  return 0;
}

void log_sql(std::string message) {
  std::cout << "[SQL] " << message << std::endl;
}
