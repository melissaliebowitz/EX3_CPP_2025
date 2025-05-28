//melissaliebowitz@yahoo.com
#include <iostream>
#include "Baron.hpp"
#include "Game.hpp"
using namespace coup;

Baron::Baron(Game& g, const std:: string& n) : Player(g, n){}

Baron::Baron(const Player &base):Player(base){}

std::string Baron::get_job() const
{
    return "Baron";
}

void Baron::invest()
{   
    if (coins < 3)
    {
        throw std:: runtime_error("You don't have enough coins to play invest.");
    }
    
    change_amount(3);
    std:: cout << get_name() << " invested their money!" <<  std:: endl;
    game.next_turn();
}

void Baron:: gather()
{   
    std:: cout << get_name() << " played gather." << std::endl;
    if (sanctioned && !sanction_expired())
    {
        change_amount(1);
        std:: cout << get_name() << " is sanctioned but still gathers one coin." << std:: endl;
        game.next_turn();
    } else{
        change_amount(1);
        game.next_turn(); 
    }
    
}

void Baron:: tax()
{   
    std:: cout << get_name() << " played tax." << std::endl;
    if(sanctioned && !sanction_expired())
    {
        change_amount(1);
        std:: cout << get_name() << " is sanctioned and collects one coin." << std:: endl;
        game.next_turn();
    } else{
        change_amount(2);
        game.add_blockable_action(action_type::tax, this, nullptr, game.get_turn_counter());
        game.next_turn();
    }

}