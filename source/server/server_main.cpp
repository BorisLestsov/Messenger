#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

#include "Participant.hpp"
#include "Session.hpp"
#include "Chatroom.hpp"
#include "Message.hpp"
#include "Server.hpp"

#define MAX_CMD_LEN 256

using boost::asio::ip::tcp;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

using namespace meow::server;
using namespace meow;

void show_help();

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: ./Server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
            
        // only one server thread is allowed!
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        Server server(io_service, endpoint);
        cout << "Created server at endpoint: " << endpoint << endl;
        
        // run server in a separate thread
        std::thread t([&io_service] () {
			io_service.run();
		});

		cout << "Server succesfully started." << endl;
		cout << "Now you may use command-line interface to manage it." << endl;
		cout << "meow |> ";
		
		char line[MAX_CMD_LEN];
		while (cin.getline(line, MAX_CMD_LEN)) {
			string cmdline(line);
			if (cmdline == "quit" || cmdline == "exit") {
				io_service.stop();
				break;
			}
			else if (cmdline == "stats") {
				cout << "Connections opened: " << server.n_opened() << endl;
			}
			else if (cmdline == "help" || cmdline == "?")
				show_help();
			cout << "meow |> ";
		}

		t.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

void show_help() {
	cout << "Welcome to the Meow Messenger Server command line!" << endl;
	cout << "Available options:" << endl;
	cout << "   help, ?       Print this message" << endl;
	cout << "   stats         Brief statistics about connections" << endl;
	cout << "   quit, exit    Shutdown server and quit" << endl;
	cout << endl;
}
