#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

int handle_connection() {
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
  res = stmt->executeQuery("SELECT * FROM user");
  while (res->next()) {
    std::cout << "user id: " << res->getInt("id") << std::endl;
    std::cout << "user time" << res->getInt("time_total") << std::endl;
  }
  delete res;
  delete stmt;
  delete con;
  return 0;
}
