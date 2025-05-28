//melissaliebowitz@yahoo.com
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>

namespace coup{
    class Game;
    
    class Player
    {
        protected:
        Game& game;
        std::string name;
        int coins;
        bool alive;
        bool sanctioned;
        int sanction_turn;
        bool bribe_pending;
        int extra_turn;
        bool arrest_blocked;
        int arrest_block_turn;
        bool must_coup;

        virtual int sanction_penalty();


        public:
        Player(Game& g, const std:: string& n);
        Player(const Player& other); 
        virtual ~Player() = default;

        //getters && setters
        int get_coins(){return coins;}
        void set_coins(int c);
        std:: string get_name(){return name;}
        bool is_alive(){return alive;}
        int get_extra_turns(){return extra_turn;}
        void set_extra_turns(int turns);
        bool get_pending_bribe(){return bribe_pending;}
        void set_pending_bribe(bool pending);
        bool is_sanctioned(){return sanctioned;}
        bool get_arrest_blocked(){return arrest_blocked;}
        int get_arrest_blocked_turn(){return arrest_block_turn;}

        //for coup
        void eliminate(){alive = false;}
        void revive(){alive = true;}
        
        //for all actions
        void change_amount(int amount) {coins += amount;}

        //for bribe
        int decrease_extra_turn();

        //for sanction
        bool sanction_expired();
        void reset_sanction();

        //for spy 
        void set_arrest_block(bool arrested){arrest_blocked = arrested;}
        void set_arrest_block_turn(int turns){arrest_block_turn = turns;}
        void reset_arrest_block();
        
        
        //for merchant
        virtual void add_coin(){change_amount(0);}

        //for general & merchant
        virtual void arrested_handle(Player& other);

        //for player who has 10 coins
        void required_coup(bool flag){must_coup = flag;}
        void wait_for_coup();

        
        void check_turn();
        virtual std:: string get_job() const {return "No job";}

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player& other);
        virtual void sanction(Player& other);
        virtual void coup(Player& other);

    };
}

#endif