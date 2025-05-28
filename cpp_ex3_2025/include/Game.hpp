//melissaliebowitz@yahoo.com
#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include "Player.hpp"

namespace coup{

    enum class action_type
    {
        none,
        tax,
        bribe,
        coup
    };

    struct blockable_action{
        action_type action;
        Player *played_by;
        Player *played_on;
        int count_turn;
    
    };

    class Game{
        private:
        int current_turn;
        bool is_game_over;
        int turn_counter;
        Player *last_arrested;
        int last_arrested_turn_count;
        std:: vector<Player*> players;
        std:: vector<blockable_action> blockable_actions;
        
        public:
        Game();
        ~Game();

        void start_game();

        Player* get_current_player(){return players[current_turn];}
        size_t get_player_index(Player *p);

        int get_turn_counter(){return turn_counter;}
        size_t get_current_turn(){return current_turn;}
        
        Player* get_last_arrested(){return last_arrested;}
        void set_last_arrested(Player *target, int current_turn);
        int get_last_arrest_count(){return last_arrested_turn_count;}
        
        void next_turn();
        std:: string turn();
    
        void force_next_player_to_be(size_t index);

        std:: vector<blockable_action>& get_blockable_actions(){return blockable_actions;}
        void add_blockable_action(action_type type, Player *played, Player *target, int turn);
        
        std:: vector<std:: string> get_players_name() const;
        std:: vector<Player*> get_players() const {return players;}
        void add_player(Player *p);
        Player* winner();
    };
}

#endif