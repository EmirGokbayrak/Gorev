#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets/QMainWindow>
#include <sys/types.h>
#include <socket.h>
#include <netdb.h>
#include <stdexcept>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class client_runtime_error : public std::runtime_error
{
public:
    client_runtime_error(const char* w) : std::runtime_error(w) {}
};

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(const std::string& addr, int port, QWidget* parent = nullptr);
    ~Client();

    int get_socket() const;
    int get_port() const;
    std::string get_addr() const;
    int send(const char* msg, size_t size);

private slots:
    void on_chooseFileButton_clicked();
    void on_uploadButton_clicked();

private:
    Ui::Client* ui;

    int f_socket;
    int f_port;
    std::string f_addr;
    struct addrinfo* f_addrinfo;

    std::string fileName;
};
#endif // CLIENT_H
