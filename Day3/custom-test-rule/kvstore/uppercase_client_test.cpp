#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static int read_port(int fd) {
  std::string s;
  char c;
  while (read(fd, &c, 1) == 1 && c != '\n') s += c;
  return std::atoi(s.c_str());
}

int main() {
  const char* server_bin = std::getenv("SERVER_BIN");
  if (!server_bin) {
    fprintf(stderr, "SERVER_BIN not set\n");
    return 1;
  }

  int pipefd[2];
  if (pipe(pipefd) != 0) return 1;

  pid_t pid = fork();
  if (pid == 0) {
    // Child: route the server's stdout into our pipe, then run it.
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    execl(server_bin, server_bin, (char*)nullptr);
    _exit(127);  // exec failed
  }

  close(pipefd[1]);
  int port = read_port(pipefd[0]);
  if (port <= 0) {
    fprintf(stderr, "no port from server\n");
    kill(pid, SIGKILL);
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
    fprintf(stderr, "connect failed\n");
    kill(pid, SIGKILL);
    return 1;
  }

  write(sock, "hello", 5);
  char buf[64] = {0};
  ssize_t n = read(sock, buf, sizeof(buf) - 1);
  close(sock);
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);

  if (n == 5 && strcmp(buf, "HELLO") == 0) return 0;
  fprintf(stderr, "expected HELLO, got '%s'\n", buf);
  return 1;
}
