//melissaliebowitz@yahoo.com
#include <iostream>
#include "Merchant.hpp"
#include "Game.hpp"
#include "Player.hpp"
using namespace coup;

Merchant:: Merchant(Game& g, const std:: string& n): Player(g,n){}

coup::Merchant::Merchant(const Player &base): Player(base){}

std:: string Merchant:: get_job() const
{
    return "Merchant";
}

void Merchant::add_coin()
{
    if (coins >= 3)
    {
        change_amount(1);
        std:: cout << "Merchant has three or more coins therefore gets another one!" << std:: endl;
    }
    
}

void Merchant:: arrested_handle(Player& other)
{   
    std:: cout << "Merchant pays two coins to the bank instead of giving their coin to " << other.get_name() << std:: endl;
    
    if (get_coins() < 2)
    {
        throw std:: runtime_error("Merchant doesn't have enough coins to be arrested.");
    }
    
    other.change_amount(0);
    change_amount(-2);
}

