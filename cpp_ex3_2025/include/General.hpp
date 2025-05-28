//melissaliebowitz@yahoo.com
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <iostream>
#include "Player.hpp"

namespace coup{
    class General : public Player{
        private:

        public:
        General(Game& g, const std:: string& n);
        General(const Player& base);
        ~General() = default;
        
        std:: string get_job() const override;
        void cancel_coup(Player& other);
        void arrested_handle(Player& other) override;
    };
}

#endif