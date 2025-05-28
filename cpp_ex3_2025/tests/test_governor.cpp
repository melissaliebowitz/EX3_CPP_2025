//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/Governor.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Governor Constructor Test")
{
    Game game;
    Governor *governor = new Governor(game, "Jessica");
    CHECK_EQ(game.get_players().size(), 1);

    CHECK_EQ(governor->get_name(), "Jessica");
    CHECK_EQ(governor->is_alive(), true);
    CHECK_EQ(governor->get_coins(), 0);
    CHECK_EQ(governor->get_extra_turns(), 0);
    CHECK_EQ(governor->get_arrest_blocked_turn(), -1);
    CHECK_EQ(governor->get_arrest_blocked(), false);
    CHECK_EQ(governor->get_pending_bribe(), false);
    CHECK_EQ(governor->is_sanctioned(), false);

}

TEST_CASE("Governor Copy Constructor")
{
    Game game;
    Player *base = new Player(game, "Amy");
    base->set_coins(1);
    Governor *governor = new Governor(*base);

    CHECK_EQ(governor->get_name(), "Amy");
    CHECK_EQ(governor->is_alive(), true);
    CHECK_EQ(governor->get_coins(), 1);
    CHECK_EQ(governor->get_extra_turns(), 0);
    CHECK_EQ(governor->get_arrest_blocked_turn(), -1);
    CHECK_EQ(governor->get_arrest_blocked(), false);
    CHECK_EQ(governor->get_pending_bribe(), false);
    CHECK_EQ(governor->is_sanctioned(), false);

}

TEST_CASE("Get Job Test")
{   
    Game game;
    Governor *governor = new Governor(game, "Governor");

    CHECK_EQ(governor->get_job(), "Governor");

}

TEST_CASE("Tax Override Test")
{
    Game game;
    Governor *governor = new Governor(game, "Governor");

    governor->tax();
    CHECK_EQ(governor->get_coins(), 3);

}

TEST_CASE("Cancel Tax Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Governor *governor = new Governor(game, "Governor");

    player->tax();
    CHECK_EQ(game.get_blockable_actions().size(), 1);
    CHECK_EQ(player->get_coins(), 2);

    governor->cancel_tax(*player);
    CHECK_EQ(game.get_blockable_actions().size(), 0);
    CHECK_EQ(player->get_coins(), 0);
    
}
