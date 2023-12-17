#include "dpp/dpp.h"
#include <cstdlib>

const std::string token =
    "";

int main() {
  dpp::cluster bot(token);
  bot.on_log(dpp::utility::cout_logger());
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
