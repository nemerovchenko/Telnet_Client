#include "TelnetClient.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

TelnetClient::TelnetClient() = default;

TelnetClient::~TelnetClient() {
  disconnect();
}

bool TelnetClient::connect(const std::string& host, int port) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    return false;
  }

  struct hostent *server_host = gethostbyname(host.c_str());
  if (!server_host) {
    std::cerr << "DNS resolution failed for: " << host << std::endl;
    return false;
  }

  sockaddr_in server{};
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  memcpy(&server.sin_addr, server_host->h_addr, server_host->h_length);

  if (::connect(sockfd, (sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect");
    return false;
  }

  running = true;
  receiver = std::thread(&TelnetClient::receiveThread, this);
  return true;
}

void TelnetClient::disconnect() {
  running = false;
  if (receiver.joinable()) receiver.join();
  if (sockfd != -1) close(sockfd);
  sockfd = -1;
}

void TelnetClient::setCallback(const std::function<void(const std::string&)>& cb) {
  callback = cb;
}

void TelnetClient::sendCommand(const std::string& cmd) {
  if (!isConnected()) return;

  std::string fullCmd = cmd + "\r\n";

  if (send(sockfd, fullCmd.c_str(), fullCmd.size(), 0) < 0) {
    perror("send");
    disconnect();
  }
}

bool TelnetClient::isConnected() const {
  return sockfd != -1;
}

void TelnetClient::receiveThread() {
  char buffer[4096];
  while (running) {
    int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
      if (running) {
        std::cerr << "\nConnection closed by server" << std::endl;
      }
      disconnect();
      return;
    }
    
    if (callback) {
      callback(std::string(buffer, bytes));
    }
  }
}