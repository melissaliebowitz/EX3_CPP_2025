//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/Baron.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Baron Constructor Test")
{
    Game game;
    Baron *baron = new Baron(game, "Bob");

    CHECK_EQ(baron->get_name(), "Bob");
    CHECK_EQ(baron->is_alive(), true);
    CHECK_EQ(baron->get_coins(), 0);
    CHECK_EQ(baron->get_extra_turns(), 0);
    CHECK_EQ(baron->get_arrest_blocked_turn(), -1);
    CHECK_EQ(baron->get_arrest_blocked(), false);
    CHECK_EQ(baron->get_pending_bribe(), false);
    CHECK_EQ(baron->is_sanctioned(), false);
    CHECK_EQ(game.get_players().size(), 1);

}


TEST_CASE("Baron Copy Constructor form Player")
{
    Game game;
    Player *base = new Player(game, "Jim");
    base->set_coins(4);
    base->set_extra_turns(2);
    Baron *baron = new Baron(*base);
    CHECK_EQ(baron->get_name(), "Jim");
    CHECK_EQ(baron->is_alive(), true);
    CHECK_EQ(baron->get_coins(), 4);
    CHECK_EQ(baron->get_extra_turns(), 2);
    CHECK_EQ(baron->get_arrest_blocked_turn(), -1);
    CHECK_EQ(baron->get_arrest_blocked(), false);
    CHECK_EQ(baron->get_pending_bribe(), true);
    CHECK_EQ(baron->is_sanctioned(), false);
    
}

TEST_CASE("Get Job Test")
{
    Game game;
    Baron *baron = new Baron(game, "Baron");
    CHECK_EQ(baron->get_job(), "Baron");

}

TEST_CASE("Invest Action Test")
{
    Game game;
    Baron *baron = new Baron(game, "Baron");
    
    baron->set_coins(1);
    CHECK_THROWS_WITH(baron->invest(), "You don't have enough coins to play invest.");
    
    baron->set_coins(3);
    CHECK_EQ(baron->get_coins(), 3);
    baron->invest();
    CHECK_EQ(baron->get_coins(), 6);

}

TEST_CASE("Sanction on Tax Override Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Baron *baron = new Baron(game, "Baron");

    player->set_coins(4);
    player->sanction(*baron);
    baron->tax();
    CHECK_EQ(baron->get_coins(), 1);

}

TEST_CASE("Sanction on Gather Override Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Baron *baron = new Baron(game, "Baron");

    player->set_coins(4);
    player->sanction(*baron);
    baron->gather();
    CHECK_EQ(baron->get_coins(), 1);
}
