#include "dpp/dpp.h"
#include <cstdlib>

const std::string token = "";
int handle_connection(long int user_id, uint ts);
int handle_disconnection(long int user_id, long int server_id, uint ts);
int insert_user_redis(long int user_id, uint ts);
int user_is_set(long int user_id);
uint get_user_ts(long int user_id);
void log_main(std::string message);

int main() {
  dpp::cluster bot(token);
  bot.on_log(dpp::utility::cout_logger());
  bot.on_voice_state_update([](const dpp::voice_state_update_t &event) {
    if (event.state.channel_id == 0) {
      log_main("################### USER LEFT A VC ####################");
      long int user_id = event.state.user_id;
      log_main("User id: \t" + std::to_string(user_id));
      uint ts_now = std::time(nullptr);
      uint ts_last = get_user_ts(user_id);
      uint ts_diff = ts_now - ts_last;
      log_main("User \t" + std::to_string(user_id) + " left a vc");
      log_main("Spent in a vc \t" + std::to_string(ts_diff));
      handle_disconnection(user_id, event.state.guild_id, ts_diff);

    } else {
      log_main("################### USER ENTERED A VC ####################");
      long int user_id = event.state.user_id;
      uint ts = std::time(nullptr);
      log_main("User \t" + std::to_string(user_id) + " joined a vc");
      log_main("Timestamp: \t" + std::to_string(ts));
      log_main("Server id: \t" + std::to_string(event.state.guild_id));
      log_main("Channel id: \t" + std::to_string(event.state.channel_id));
      log_main("Session id: \t" + event.state.session_id);
      log_main("Deafened: \t" + std::to_string(event.state.is_deaf()));
      log_main("Muted: \t\t" + std::to_string(event.state.is_mute()));
      log_main("Self deafened: \t" +
               std::to_string(event.state.is_self_deaf()));
      log_main("Self muted: \t" + std::to_string(event.state.is_self_mute()));
      log_main("Self video: \t" + std::to_string(event.state.self_video()));
      log_main("Streaming: \t" + std::to_string(event.state.self_stream()));

      if (user_is_set(user_id)) {
        log_main("User is already set");
      } else {
        log_main("User is not set");
        insert_user_redis(user_id, ts);
        handle_connection(user_id, event.state.guild_id);
      }
    }
  });
  bot.on_slashcommand([](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "ping") {
      event.reply("Pong!");
    }
  });
  bot.on_ready([&bot](auto event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
          dpp::slashcommand("ping", "Ping pong!", bot.me.id));
    }
  });

  bot.start(dpp::st_wait);

  return 0;
}

void log_main(std::string message) {
  std::cout << "[main] " << message << std::endl;
}
