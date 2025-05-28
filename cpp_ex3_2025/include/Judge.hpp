//melissaliebowitz@yahoo.com
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include <iostream>
#include "Player.hpp"

namespace coup{
    class Judge : public Player{

        public:
        Judge(Game& g, const std:: string& n);
        Judge(const Player& base);
        ~Judge() = default;

        std:: string get_job() const override;
        void cancel_bribe(Player &other);
        int sanction_penalty() override;
    };
}

#endif