//melissaliebowitz@yahoo.com
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"
using namespace coup;

Player::Player(Game& g, const std::string& n): game(g), name(n), coins(0), alive(true), sanctioned(false),
sanction_turn(-1), bribe_pending(false) , extra_turn(0), arrest_blocked(false), arrest_block_turn(-1), must_coup(false){
    game.add_player(this);
}

Player::Player(const Player &other): 
    game(other.game), 
    name(other.name), 
    coins(other.coins),
    alive(other.alive),
    sanctioned(other.sanctioned),
    sanction_turn(other.sanction_turn),
    bribe_pending(other.bribe_pending),
    extra_turn(other.extra_turn),
    arrest_blocked(other.arrest_blocked),
    arrest_block_turn(other.arrest_block_turn),
    must_coup(other.must_coup){}

void Player::set_coins(int c)
{
    coins = c;
}

void Player::set_extra_turns(int turns)
{
    bribe_pending = true;
    extra_turn = turns; 
}

void Player::set_pending_bribe(bool pending)
{
    bribe_pending = pending;
}

int coup::Player::decrease_extra_turn()
{
    if (extra_turn > 0)
    {
        extra_turn--;
    }
    
    return extra_turn;
}

void Player:: check_turn()
{
    if (!is_alive())
    {
        throw std:: runtime_error("You are out!");
    }
    
    if (game.turn() != this->get_name())
    {
        throw std:: runtime_error("Not your turn!");
    }
}

bool Player::sanction_expired()
{
    return game.get_turn_counter() > sanction_turn + game.get_players_name().size() -1;
}

void Player::reset_sanction()
{   
    if (sanctioned && sanction_expired())
    {
        sanctioned = false;
        sanction_turn = -1;
    }
}

void Player::reset_arrest_block()
{
    set_arrest_block(false);
    set_arrest_block_turn(0);
}

void Player::arrested_handle(Player& other)
{   

    if (get_coins() < 1)
    {
        throw std:: runtime_error(get_name() + " doesn't have enough coins to be arrested.");
    }

    other.change_amount(1);
    change_amount(-1);
}

int Player::sanction_penalty() 
{
    return 0;
}

void Player::wait_for_coup()
{
    if (must_coup)
    {
        throw std:: runtime_error("You must play coup before playing any other action.");
    }
}

//main actions
void Player::gather()
{   
    check_turn();
    wait_for_coup();
    
    if (sanctioned && !sanction_expired())
    {
        std:: cout << "You are sanctioned and cannot play gather this turn." << std:: endl;
        return;
    }
    
   change_amount(1);
   std:: cout << get_name() << " played gather." << std:: endl;
   game.next_turn();
}

void Player:: tax()
{   
    check_turn();
    wait_for_coup();
    if (sanctioned && !sanction_expired())
    {
        std:: cout << "You are sanctioned and cannot play tax this turn." << std:: endl;
        return;
    }

    change_amount(2);
    std:: cout << get_name() << " played tax." << std:: endl;
    game.add_blockable_action(action_type:: tax, this, nullptr, game.get_turn_counter());
    game.next_turn();

}

void Player::arrest(Player &other)
{   
    check_turn();
    wait_for_coup();

    //checks if you are blocked by the spy.
    if (arrest_blocked && game.get_turn_counter() < arrest_block_turn + game.get_players_name().size())
    {
        throw std:: runtime_error("You are blocked from playing arrest until your next turn.");
    }
    
    if(game.get_last_arrested() == &other && (game.get_turn_counter() - game.get_last_arrest_count() == 1))
    {
        throw std:: runtime_error(other.get_name() + " was arrested last turn and cannot be arrested two turns in a row.");
    }

    if(!other.is_alive())
    {
        throw std:: runtime_error("Cannot arrest a players who is out.");
    }

    std:: cout << get_name() << " arrested " << other.get_name() << "." << std:: endl;
    other.arrested_handle(*this);
    game.set_last_arrested(&other, game.get_turn_counter());
    game.next_turn();
}

void Player::sanction(Player &other)
{   
    check_turn();
    wait_for_coup();
    
    if(!other.is_alive())
    {
        throw std:: runtime_error("Cannot sanction a player who is out of the game.");
    }
    
    std:: cout << get_name() << " sanctioned " << other.get_name() << "." << std:: endl;
    int amount = 3;
    int cost = amount + other.sanction_penalty();

    if (coins < cost)
    {
        throw std:: runtime_error("You do not have enough coins to play sanction on another player.");
    }
    
    change_amount(-cost);
    other.sanctioned = true;
    other.sanction_turn = game.get_turn_counter();
    game.next_turn();
}

void Player::bribe()
{   
    check_turn();
    wait_for_coup();
    if (coins < 4)
    {
        throw std:: runtime_error("Not enough coins to play bribe.");
    }

    change_amount(-4);
    set_extra_turns(1);
    game.add_blockable_action(action_type:: bribe, this, nullptr, game.get_turn_counter());
    std:: cout << get_name() << " played bribe and gets an extra turns next round." << std:: endl;
    game.next_turn();    
}

void Player::coup(Player &other)
{   
    check_turn();
    if (coins < 7)
    {
        throw std:: runtime_error("You don't have enough coins to play coup on another player.");
    }
    
    if (!other.is_alive())
    {
        throw std:: runtime_error("This players is already out.");
    }
        
        change_amount(-7);
        other.eliminate();
        must_coup = false;
        std:: cout << get_name() << " couped " << other.get_name() << std:: endl;
        game.add_blockable_action(action_type:: coup, this, &other, game.get_turn_counter());
        game.next_turn();
}



