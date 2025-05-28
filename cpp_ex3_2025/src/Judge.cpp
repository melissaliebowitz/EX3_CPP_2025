//melissaliebowitz@yahoo.com
#include <iostream>
#include "Judge.hpp"
#include "Game.hpp"
#include "Game.hpp"
using namespace coup;

Judge::Judge(Game& g, const std:: string& n): Player(g, n){}

Judge::Judge(const Player &base): Player(base){}

std:: string Judge:: get_job() const
{
    return "Judge";
}

void Judge::cancel_bribe(Player& other)
{
    auto &actions = game.get_blockable_actions();
    bool found = false;

    for (auto it = actions.begin(); it != actions.end(); it++)
    {   

        if (it->action == action_type:: bribe && it->played_by == &other)
        {
            other.set_extra_turns(0);
            other.set_pending_bribe(false);
            std:: cout << get_name() << " canceled " <<other.get_name() << "'s bribe action." << std:: endl;
            actions.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
    {
        throw std:: runtime_error("There is no bribe played by " + other.get_name() + " to cancel.");
    }
}


int Judge::sanction_penalty()
{   
    std:: cout << "Any player who plays sanction on the judge must pay an extra coin." << std:: endl;
    return 1;
}