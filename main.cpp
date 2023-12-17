#include "dpp/dpp.h"
#include <cstdlib>

const std::string token = "token";

int main() {
  dpp::cluster bot(token);
  bot.on_log(dpp::utility::cout_logger());
  bot.on_voice_state_update([](const dpp::voice_state_update_t &event) {
    if (event.state.channel_id == 0) {
      std::cout << "User left a vc" << std::endl;
    } else {
      std::cout << "Voice state update, user_id: " << event.state.user_id
                << std::endl;
      std::cout << "Voice state update, channel_id: " << event.state.channel_id
                << std::endl;
      std::cout << "Voice state update, guild_id (server_id) "
                << event.state.guild_id << std::endl;
      std::cout << "Voice state update, session_id: " << event.state.session_id
                << std::endl;
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
