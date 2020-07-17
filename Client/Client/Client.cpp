#include "Client.h"
#include "ui_client.h"
#include <QtWidgets/QFileDialog>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iterator>

Client::Client(const std::string& addr, int port, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

	char decimal_port[16];
	snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
	decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
	if (r != 0 || f_addrinfo == NULL)
	{
		throw client_runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
	}
	f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
	if (f_socket == -1)
	{
		freeaddrinfo(f_addrinfo);
		throw client_runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
	}

	fileName = "";
}

Client::~Client()
{
    delete ui;
	freeaddrinfo(f_addrinfo);
	close(f_socket);
}

int Client::get_socket() const
{
	return f_socket;
}

int Client::get_port() const
{
	return f_port;
}

std::string Client::get_addr() const
{
	return f_addr;
}

int Client::send(const char* msg, size_t size)
{
	return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}

void Client::on_chooseFileButton_clicked()
{
    QString filter = "JPEG File (*.jpeg)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "C://", filter);
	this->fileName = fileName.toStdString();
}

void Client::on_uploadButton_clicked()
{
    //transfer selected JPEG to server

	std::ifstream input(fileName, std::ios::binary);

	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	send(reinterpret_cast<char*>(buffer.data()), buffer.size());
}
