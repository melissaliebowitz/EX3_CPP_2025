//melissaliebowitz@yahoo.com
#include <sstream>
#include <iostream>
#include "doctest.h"
#include "../include/Spy.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Spy constructor Test")
{
    Game game;
    Spy *spy = new Spy(game, "Charles");
    CHECK_EQ(game.get_players().size(), 1);

    CHECK_EQ(spy->get_name(), "Charles");
    CHECK_EQ(spy->is_alive(), true);
    CHECK_EQ(spy->get_coins(), 0);
    CHECK_EQ(spy->get_extra_turns(), 0);
    CHECK_EQ(spy->get_arrest_blocked_turn(), -1);
    CHECK_EQ(spy->get_arrest_blocked(), false);
    CHECK_EQ(spy->get_pending_bribe(), false);
    CHECK_EQ(spy->is_sanctioned(), false);
}

TEST_CASE("Spy Copy Constructor from Player")
{
    Game game;
    Player *base = new Player(game, "Claire");
    base->set_coins(2);
    base->set_arrest_block_turn(1);
    base->set_extra_turns(1);

    Spy *spy = new Spy(*base);
    CHECK_EQ(spy->get_name(), "Claire");
    CHECK_EQ(spy->is_alive(), true);
    CHECK_EQ(spy->get_coins(), 2);
    CHECK_EQ(spy->get_extra_turns(), 1);
    CHECK_EQ(spy->get_arrest_blocked_turn(), 1);
    CHECK_EQ(spy->get_arrest_blocked(), false);
    CHECK_EQ(spy->get_pending_bribe(), true);
    CHECK_EQ(spy->is_sanctioned(), false);
    
}

TEST_CASE("Get Job Test")
{
    Game game;
    Spy *spy = new Spy(game, "Spy");
    CHECK_EQ(spy->get_job(), "Spy");
}


TEST_CASE("Peek Test (check through cout)")
{
    Game game;
    Spy* spy = new Spy(game, "Spy");
    Player* player = new Player(game, "Player");

    player->set_coins(5);

    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf();       
    std::cout.rdbuf(buffer.rdbuf());  

    spy->peek(*player); 

    std::cout.rdbuf(oldCout);   

    std::string expected = "Peek: Player has 5 coins.\n";
    CHECK_EQ(expected, buffer.str());
}


TEST_CASE("Block Arrest Test")
{
    Game game;
    Spy *spy = new Spy(game, "Spy");
    Player *player = new Player(game, "Player");

    spy->block_arrest(*player);
    spy->gather();
    CHECK_EQ(player->get_arrest_blocked(), true);
    CHECK_EQ(player->get_arrest_blocked_turn(), 0);

    CHECK_THROWS_WITH(player->arrest(*spy), "You are blocked from playing arrest until your next turn.");
}
