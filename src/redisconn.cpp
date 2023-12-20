#include <cstdlib>
#include <iostream>

#include <redis-cpp/execute.h>
#include <redis-cpp/stream.h>
#include <string>

void log_redis(std::string message);

int test() {
  try {
    auto stream = rediscpp::make_stream("localhost", "6379");
    auto response = rediscpp::execute(*stream, "ping");
    log_redis(response.as<std::string>());
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int insert_user_redis(long int user_id, uint ts) {
  try {
    auto stream = rediscpp::make_stream("localhost", "6379");
    auto response = rediscpp::execute(*stream, "set", std::to_string(user_id),
                                      std::to_string(ts), "EX", "3600");
    log_redis("Set key '" + std::to_string(user_id) + " = " +
              std::to_string(ts) + "': " + response.as<std::string>());
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int user_is_set(long int user_id) {
  auto stream = rediscpp::make_stream("localhost", "6379");
  log_redis("Checking if key '" + std::to_string(user_id) + "' exists");
  auto response = rediscpp::execute(*stream, "EXISTS", std::to_string(user_id));
  int exists = response.as_integer();
  log_redis("Key exists: " + std::to_string(exists));
  return exists;
}

uint get_user_ts(long int user_id) {
  auto stream = rediscpp::make_stream("localhost", "6379");
  log_redis("Getting value for key '" + std::to_string(user_id) + "'");
  auto response = rediscpp::execute(*stream, "GET", std::to_string(user_id));
  log_redis("Executed response " + response.as<std::string>());
  uint ts = std::stoll(response.as<std::string>());
  auto result = rediscpp::execute(*stream, "DEL", std::to_string(user_id));
  if (result.as_integer() == 1) {
    log_redis("Key deleted.");
  } else {
    log_redis("Key did not exist, not deleted.");
  }
  return ts;
}

void log_redis(std::string message) {
  std::cout << "[Redis] " << message << std::endl;
}
