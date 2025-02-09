#include "TelnetClient.h"
#include <iostream>
#include <csignal>

std::atomic<bool> running(true);

void signalHandler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
    return 1;
  }

  signal(SIGINT, signalHandler);

  TelnetClient client;
  client.setCallback([](const std::string& response) {
    std::cout << response << std::flush;
  });

  if (!client.connect(argv[1], std::stoi(argv[2]))) {
    return 1;
  }

  std::cout << "Connected! Type request lines (enter 'END' to send):\n";

  std::string input;
  std::string fullRequest;
  while (running) {
    std::cout << "> ";
    std::getline(std::cin, input);

    if (!running) break;

    if (input == "END") {
      if (!fullRequest.empty()) {
        client.sendCommand(fullRequest + "\r\n");
        fullRequest.clear();
      }
    } else {
      fullRequest += input + "\r\n";
    }
  }

  client.disconnect();
  std::cout << "\nDisconnected" << std::endl;
  return 0;
}