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
