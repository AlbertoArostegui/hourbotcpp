#include "dpp/dpp.h"
#include <cstdlib>

const std::string token = "";
int handle_connection(long int user_id, uint ts);
int handle_disconnection(long int user_id);
int insert_user(long int user_id, uint ts);
void log_main(std::string message);

int main() {
  dpp::cluster bot(token);
  bot.on_log(dpp::utility::cout_logger());
  bot.on_voice_state_update([](const dpp::voice_state_update_t &event) {
    if (event.state.channel_id == 0) {
      std::cout << "User left a vc" << std::endl;
    } else {
      long int user_id = event.state.user_id;
      uint ts = std::time(nullptr);
      log_main("User " + std::to_string(user_id) + " joined a vc");
      log_main("Timestamp: " + std::to_string(ts));
      log_main("Server id: " + std::to_string(event.state.guild_id));
      log_main("Channel id: " + std::to_string(event.state.channel_id));
      log_main("Session id: " + event.state.session_id);
      log_main("Deafened: " + std::to_string(event.state.is_deaf()));
      log_main("Muted: " + std::to_string(event.state.is_mute()));
      log_main("Self deafened: " + std::to_string(event.state.is_self_deaf()));
      log_main("Self muted: " + std::to_string(event.state.is_self_mute()));
      log_main("Self video: " + std::to_string(event.state.self_video()));
      log_main("Streaming: " + std::to_string(event.state.self_stream()));
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
