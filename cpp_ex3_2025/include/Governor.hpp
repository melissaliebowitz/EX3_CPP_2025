//melissaliebowitz@yahoo.com
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include <iostream>
#include "Player.hpp"

namespace coup{
    class Governor : public Player{

        private:

        public:
        Governor(Game& g, const std:: string& n);
        Governor(const Player& base);
        ~Governor() = default;

        std:: string get_job() const override;
        void tax() override;
        void cancel_tax(Player& other);

    };
}

#endif