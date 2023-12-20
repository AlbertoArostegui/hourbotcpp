#include <iostream>

using namespace std;
int insert_user(long int user_id, uint ts);
int user_is_set(long int user_id);
void log_test(string message);

int main() {
  insert_user(123456789, 123456789);
  log_test("insert user 123456789");
  int set = user_is_set(123456789);
  log_test("is set 123456789: " + to_string(set));
  return 0;
}

void log_test(string message) { cout << "[test] " << message << endl; }
