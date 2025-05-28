//melissaliebowitz@yahoo.com
#ifndef SPY_HPP
#define SPY_HPP

#include <iostream>
#include "Player.hpp"

namespace coup{
    class Spy : public Player{
        private:

        public:
        Spy(Game& g, const std:: string& n);
        Spy(const Player& base);
        ~Spy() = default;

        std:: string get_job() const override;
        void peek(Player& other) const;
        void block_arrest(Player& other);
    };

}

#endif