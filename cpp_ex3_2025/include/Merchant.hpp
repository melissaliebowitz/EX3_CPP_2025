//melissaliebowitz@yahoo.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include <iostream>
#include "Player.hpp"

namespace coup{
    class Merchant : public Player{

        public:
        Merchant(Game& g, const std:: string& n);
        Merchant(const Player& base);
        ~Merchant() = default;

        std:: string get_job() const override;
        void add_coin() override;
        void arrested_handle(Player& other) override;
    };

}

#endif