//melissaliebowitz@yahoo.com
#include <iostream>
#include "Spy.hpp"
#include "Game.hpp"
using namespace coup;

Spy:: Spy(Game& g, const std:: string& n): Player(g,n){}

coup::Spy::Spy(const Player &base): Player(base){}

std:: string Spy:: get_job() const
{
    return "Spy";
}

void Spy:: peek(Player& other) const{
    int c = other.get_coins();
    std:: cout << "Peek: " << other.get_name() << " has " << c << " coins." << std:: endl;
}

void coup::Spy::block_arrest(Player &other)
{
    other.set_arrest_block(true);
    other.set_arrest_block_turn(game.get_turn_counter());
    std:: cout << get_name() << " blocked " << other.get_name() << " from playing arrest." << std:: endl;

}
