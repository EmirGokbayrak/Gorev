#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets/QMainWindow>
#include "ui_Server.h"
#include <netdb.h>
#include <stdexcept>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class server_runtime_error : public std::runtime_error
{
public:
    server_runtime_error(const char* w) : std::runtime_error(w) {}
};

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(const std::string& addr, int port, QWidget* parent = nullptr);
    ~Server();

    int get_socket() const;
    int get_port() const;
    std::string get_addr() const;

    int recv(char* msg, size_t max_size);
    int timed_recv(char* msg, size_t max_size, int max_wait_ms);

private:
    Ui::Server* ui;
    int f_socket;
    int f_port;
    std::string f_addr;
    struct addrinfo* f_addrinfo;
};
#endif // SERVER_H
