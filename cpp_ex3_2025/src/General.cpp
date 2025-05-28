//melissaliebowitz@yahoo.com
#include <iostream>
#include "General.hpp"
#include "Game.hpp"
using namespace coup;

General:: General(Game& g, const std:: string& n): Player(g,n){}

coup::General::General(const Player &base): Player(base){}

std:: string General:: get_job() const
{
    return "General";
}

void General:: cancel_coup(Player& other)
{   
    if (coins < 5)
    {
        throw std:: runtime_error("Not enough coins to cancel coup.");
    }
    
    auto& actions = game.get_blockable_actions();
    bool found = false;
    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        if (it->action == action_type:: coup && it->played_on == &other)
        {
            other.revive();
            change_amount(-5);
            std:: cout << get_name() << " canceled coup on " << other.get_name() << std:: endl;
            actions.erase(it);
            found = true;

            //think about this
            int revived_index = game.get_player_index(&other);
            std:: cout << "Revived index: " << revived_index << std:: endl;
            std:: cout << "Current turn is index: " << game.get_current_turn() << std:: endl;
            if (game.get_current_turn() == revived_index)
            {
                game.force_next_player_to_be(revived_index);
                std:: cout << game.get_current_turn() << std:: endl;
            }
            break;
        }
    }

    if (!found)
    {
        throw std:: runtime_error("There is no coup to cancel");
    }
}

void General:: arrested_handle(Player& other)
{   
    if (coins < 1)
    {
        throw std:: runtime_error("General doesn't have any coins to be arrested.");
    }

    std:: cout << "General gets back coin from arrest." << std:: endl;
    other.change_amount(-1);
    change_amount(1);
    other.change_amount(1);
    change_amount(-1);
}