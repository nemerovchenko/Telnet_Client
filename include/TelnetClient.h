#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>

class TelnetClient {
public:
  TelnetClient();
  ~TelnetClient();

  bool connect(const std::string& host, int port);
  void disconnect();
  void setCallback(const std::function<void(const std::string&)>& cb);
  void sendCommand(const std::string& cmd);
  bool isConnected() const;

private:
  void receiveThread();

  int sockfd = -1;
  std::atomic<bool> running{false};
  std::thread receiver;
  std::function<void(const std::string&)> callback;
};