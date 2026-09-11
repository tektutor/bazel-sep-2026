// Binds an ephemeral port, prints it to stdout, then uppercases
// whatever each client sends.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cctype>
#include <cstdio>

int main() {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) return 1;

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  addr.sin_port = 0;  // let the OS pick a free port

  if (bind(listen_fd, (sockaddr*)&addr, sizeof(addr)) < 0) return 1;
  if (listen(listen_fd, 1) < 0) return 1;

  socklen_t len = sizeof(addr);
  getsockname(listen_fd, (sockaddr*)&addr, &len);
  printf("%d\n", ntohs(addr.sin_port));  // tell the test our port
  fflush(stdout);

  for (;;) {
    int conn = accept(listen_fd, nullptr, nullptr);
    if (conn < 0) continue;
    char buf[1024];
    ssize_t n = read(conn, buf, sizeof(buf));
    for (ssize_t i = 0; i < n; ++i)
      buf[i] = std::toupper((unsigned char)buf[i]);
    write(conn, buf, n);
    close(conn);
  }
}
