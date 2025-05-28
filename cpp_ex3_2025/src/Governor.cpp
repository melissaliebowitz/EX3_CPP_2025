//melissaliebowitz@yahoo.com
#include <iostream>
#include <string>
#include "Governor.hpp"
#include "Game.hpp"
#include "General.hpp"
using namespace coup;

Governor:: Governor(Game& g, const std:: string& n): Player(g,n){}

coup::Governor::Governor(const Player &base): Player(base){}

std:: string Governor:: get_job() const
{
    return "Governor";
}

void Governor:: tax()
{   
    if (sanctioned && !sanction_expired())
    {
        std:: cout << "You are sanctioned and cannot play tax this turn." << std:: endl;
        return;
    }
    
    change_amount(3);
    std :: cout << get_name() << " played tax." << std:: endl;
    game.next_turn();
}

void Governor::cancel_tax(Player &other)
{   
    auto& actions = game.get_blockable_actions();
    bool found = false;

    for (auto it = actions.begin(); it != actions.end(); it++)
    {          
        if (it->action == action_type:: tax && it->played_by == &other)
        {
            other.change_amount(-2);
            std:: cout << get_name() << " canceled " << other.get_name() << "'s tax action." << std:: endl;
            actions.erase(it);
            found = true;
            break;
        }
    }
    
    if(!found)
    {
        throw std:: runtime_error("There is no tax played by " + other.get_name() + " to cancel.");
    }
}
