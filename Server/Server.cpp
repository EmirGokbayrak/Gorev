#include "Server.h"
#include "ui_server.h"
#include "imageform.h"
#include "ui_imageform.h"
#include <QPixmap>

Server::Server(const std::string& addr, int port, QWidget* parent)
    : QMainWindow(parent), 
    f_port(port), 
    f_addr(addr),
    ui(new Ui::Server)
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
		throw udp_client_server_runtime_error(("invalid address or port for UDP socket: \"" + addr + ":" + decimal_port + "\"").c_str());
	}

	f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);

	if (f_socket == -1)
	{
		freeaddrinfo(f_addrinfo);
		throw server_runtime_error(("could not create UDP socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
	}
	r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
	if (r != 0)
	{
		freeaddrinfo(f_addrinfo);
		close(f_socket);
		throw server_runtime_error(("could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
	}
}

Server::~Server()
{
    delete ui;
	freeaddrinfo(f_addrinfo);
	close(f_socket);
}

int Server::get_socket() const
{
	return f_socket;
}

int Server::get_port() const
{
	return f_port;
}

std::string Server::get_addr() const
{
	return f_addr;
}

int Server::recv(char* msg, size_t max_size)
{
	return ::recv(f_socket, msg, max_size, 0);
}

int Server::timed_recv(char* msg, size_t max_size, int max_wait_ms)
{
	fd_set s;
	FD_ZERO(&s);
	FD_SET(f_socket, &s);
	struct timeval timeout;
	timeout.tv_sec = max_wait_ms / 1000;
	timeout.tv_usec = (max_wait_ms % 1000) * 1000;
	int retval = select(f_socket + 1, &s, &s, &s, &timeout);
	if (retval == -1)
	{
		// select() set errno accordingly
		return -1;
	}
	if (retval > 0)
	{
		// our socket has data
		return ::recv(f_socket, msg, max_size, 0);
	}

	// our socket has no data
	errno = EAGAIN;
	return -1;
}


//Eksik kalan adýmlar

//Gelen char* verisini Jpeg formatýna çevirip server da image location altýna kaydetmek

//Kaydedilen Jpeg dosyasýný imageform içine yerleþtirmek 
//QPixmap pix("image location");
//int width = ui->imageArea->width();
//int height = ui->imageArea->height();
//ui->imageArea->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));

//imageform içine koyulan Jpeg dosyasýný her seferinde yeni bir tab altýnda göstermek
//ui->tabWidget->addTab(new Imageform(), QString("Tab %0").arg(ui->tabWidget->count() + 1));
//ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);