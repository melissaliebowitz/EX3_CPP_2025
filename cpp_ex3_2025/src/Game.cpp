//melissaliebowitz@yahoo.com
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "Game.hpp"
#include "Player.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
using namespace coup;
 
Game::Game(): current_turn(0), is_game_over(false), turn_counter(0), last_arrested(nullptr), last_arrested_turn_count(-2){}

Game::~Game()
{
    for (Player *p : players)
    {
        delete p;
    }
    players.clear();
}

void Game::start_game()
{
    if (players.size() < 2 || players.size() > 6)
    {
        throw std:: runtime_error("Game can only have 2-6 players.");
    }

    std:: vector<std:: string> roles = {
        "General" , "Governor", "Judge", "Spy", "Merchant", "Baron"
    };

    unsigned seed = std:: chrono:: system_clock:: now().time_since_epoch().count();
    std:: shuffle(roles.begin(), roles.end(), std:: default_random_engine(seed));

    for (int i = 0; i < players.size(); i++)
    {
        const std:: string& role = roles[i];

        Player *old_player = players[i];
        Player *new_player = nullptr;

        if (role == "General")
        {
            new_player = new General(*old_player);
        }
        else if(role == "Baron"){
            new_player = new Baron(*old_player);
        }
        else if (role == "Governor")
        {
            new_player = new Governor(*old_player);
        }
        else if (role == "Judge")
        {
            new_player = new Judge(*old_player);
        }
        else if (role == "Merchant")
        {
            new_player = new Merchant(*old_player);
        }else{
            new_player = new Spy(*old_player);
        }
        
        delete old_player;
        players[i] = new_player;

        std:: cout << new_player->get_name() << " has been assigned " << role << "." << std:: endl;
        
    }
    
    std:: cout << "Game has started! First turn is " << players[current_turn]->get_name() << "." << std::endl;
    turn_counter++;
    
}

void Game::add_player(Player *p)
{   
    if (players.size() >= 6)
    {
        throw std:: runtime_error("Game can have up to 6 players.");
    }
    players.push_back(p);
    
   
}

size_t Game::get_player_index(Player *p)
{   

    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i] == p)
        {
            return i;
        }
        
    }
    throw std:: runtime_error("Player not found in game");
}

std::vector<std::string> Game::get_players_name() const
{
    std:: vector<std:: string> active_names;
    for (const auto& player : players)
    {   
        if (player->is_alive())
        {
            active_names.push_back(player->get_name());
        }
    }

    return active_names;
}

void Game::force_next_player_to_be(size_t index)
{
    current_turn = (index + players.size()) % players.size();
}

void Game::add_blockable_action(action_type type, Player *played, Player *target, int turn)
{
    blockable_action new_blockable_action = {
        type,
        played,
        target,
        turn
    };

    blockable_actions.push_back(new_blockable_action);
}
 
void Game::set_last_arrested(Player *target, int current_turn)
{
    last_arrested = target;
    last_arrested_turn_count = current_turn;
}

std:: string Game::turn()
{
    if (players.empty())
    {
        throw std:: runtime_error("No players in the game");
    }
    
    int index = current_turn % players.size();

    while (!players[index]->is_alive())
    {
        index = (index + 1) % players.size();
        if (index == current_turn)
        {
            throw std:: runtime_error("No more players alive.");
        }
    }
    
    return players[index]->get_name();
}

void Game::next_turn()
{   
    Player *current = players[current_turn];

    //resets the block arrest played by spy
    if (current->get_arrest_blocked() && get_turn_counter() >= current->get_arrest_blocked_turn() + get_players().size())
    {
        current->reset_arrest_block();
    }
    
    //handling extra turns when bribe is played
    if (current->get_pending_bribe())
    {
        current->set_pending_bribe(false);  
    } 
    else if (current->get_extra_turns() > 0)
    {
        std:: cout << current->get_name() << " is taking an extra turn." << std:: endl;
        current->decrease_extra_turn();
        current->add_coin();
        if (current->get_coins() >= 10)
        {
            current->required_coup(true);
            std:: cout << current->get_name() << " has 10 coins therefore must play coup." << std:: endl; 
        }

        return;
    }
    
    //check for winners before moving to next turn
    size_t alive_players = 0;
    for (Player *player : players)
    {
        if (player->is_alive())
        {
            alive_players++;
        }
    }

    if (alive_players == 1)
    {
        try
        {
            Player *win = winner();
            std:: cout << win->get_name() << " is the winner!" << std:: endl;
            
        }
        catch(const std::exception& e)
        {
            std::cerr <<"Error: " << e.what() << '\n';
        }
        return;
    }

    do
    { 
        current_turn = (current_turn + 1) % players.size();
        current = players[current_turn];

    } while (!current->is_alive());

    turn_counter++;
    std:: cout << "turn #" << turn_counter << std:: endl;
    
    current->reset_sanction();
    current->add_coin();
   
    if (current->get_coins() < 3 && current->is_sanctioned() && current->get_arrest_blocked())
    {   
        std:: cout << current->get_name() << " doesn't have any actions they can play ans will be skipped this turn." << std:: endl;
        current_turn = (current_turn + 1) % players.size();
        current = players[current_turn];
        std:: cout << current->get_name() << "'s turn." << std:: endl;
        
    }

    for(auto it = get_blockable_actions().begin(); it != get_blockable_actions().end(); it++)
    {   
        if(get_turn_counter() > it->count_turn + get_players_name().size() -1)
        {
            get_blockable_actions().erase(it);
            break;
        }
    }
    
    if (current->get_coins() >= 10)
    {
        current->required_coup(true);
        std:: cout << current->get_name() << " has 10 coins therefore must play coup." << std:: endl; 
    }
}

Player* Game::winner()
{
    int counter = 0;
    Player *alive_player = nullptr;
    for (Player *player : players)
    {
        if (player->is_alive())
        {
            counter++;
            alive_player = player;
        }
    }

    if (counter > 1)
    {
        throw std:: runtime_error("There is still more than one player alive, no winner yet.");
    }

    if (counter == 1)
    {
        is_game_over = true;
        return alive_player;
    }

    throw std:: runtime_error("No players are alive, this is problematic.");
    
}
