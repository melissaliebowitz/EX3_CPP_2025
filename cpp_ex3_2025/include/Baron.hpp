//melissaliebowitz@yahoo.com
#include <iostream>
#include "Player.hpp"

namespace coup{
    class Baron : public Player{
        private:

        public:
        Baron(Game& g, const std:: string& n);
        Baron(const Player& base);
        ~Baron() = default;

        std:: string get_job() const override;
        void invest();
        void gather() override;
        void tax() override;

    };
}