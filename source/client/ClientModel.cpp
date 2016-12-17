#include <iostream>

#include "client_headers/ClientModel.hpp"

namespace meow {
    namespace client {
		
		ClientModel::ClientModel()
		{
			
		}
		
		void ClientModel::add_observer(Observer* observer)
		{
			observers_.push_front(observer);
		}

		void ClientModel::add_message(const Message& msg)
        {
            dialog_.push_back(msg);
            notify_all();
        }

        deque<Message>* ClientModel::get_dialog()
        {
            return &dialog_;
        }

        ClientModel::~ClientModel()
        {

        }
        
        // private
        void ClientModel::notify_all()
        {
			for (auto obs : observers_)
				obs->update();
		}

    }
} // namespace meow
