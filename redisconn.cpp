#include <cstdlib>
#include <iostream>

#include <redis-cpp/execute.h>
#include <redis-cpp/stream.h>

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

int insert_user(long int user_id, uint ts) {
  try {
    auto stream = rediscpp::make_stream("localhost", "6379");
    auto response = rediscpp::execute(*stream, "set", std::to_string(user_id),
                                      std::to_string(ts), "EX", "3600");
    log_redis("Set key '" + std::to_string(user_id) + "=" + std::to_string(ts) +
              "': " + response.as<std::string>());
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void log_redis(std::string message) {
  std::cout << "[Redis] " << message << std::endl;
}
