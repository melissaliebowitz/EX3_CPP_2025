//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/Merchant.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Merchant Constructor Test")
{
    Game game;
    Merchant *merchant = new Merchant(game, "Bill");
    CHECK_EQ(game.get_players().size(), 1);

    CHECK_EQ(merchant->get_name(), "Bill");
    CHECK_EQ(merchant->is_alive(), true);
    CHECK_EQ(merchant->get_coins(), 0);
    CHECK_EQ(merchant->get_extra_turns(), 0);
    CHECK_EQ(merchant->get_arrest_blocked_turn(), -1);
    CHECK_EQ(merchant->get_arrest_blocked(), false);
    CHECK_EQ(merchant->get_pending_bribe(), false);
    CHECK_EQ(merchant->is_sanctioned(), false);
}

TEST_CASE("Merchant Copy Constructor")
{
    Game game;
    Player *base = new Player(game, "Shira");
    base->set_coins(3);
    Merchant *merchant = new Merchant(*base);
    CHECK_EQ(merchant->get_name(), "Shira");
    CHECK_EQ(merchant->is_alive(), true);
    CHECK_EQ(merchant->get_coins(), 3);
    CHECK_EQ(merchant->get_extra_turns(), 0);
    CHECK_EQ(merchant->get_arrest_blocked_turn(), -1);
    CHECK_EQ(merchant->get_arrest_blocked(), false);
    CHECK_EQ(merchant->get_pending_bribe(), false);
    CHECK_EQ(merchant->is_sanctioned(), false);
}

TEST_CASE("Get Job Test")
{
    Game game;
    Merchant *merchant = new Merchant(game, "Merchant");
    CHECK_EQ(merchant->get_job(), "Merchant");
}

TEST_CASE("Add Coin Test")
{
    Game game;
    Merchant *merchant = new Merchant(game, "Merchant");

    merchant->set_coins(3);
    merchant->add_coin();
    CHECK_EQ(merchant->get_coins(), 4);

    merchant->set_coins(1);
    merchant->add_coin();
    CHECK_EQ(merchant->get_coins(), 1);
}

TEST_CASE("Merchant Handled Arrest Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Merchant *merchant = new Merchant(game, "Merchant");

    merchant->set_coins(2);
    merchant->arrested_handle(*player);

    CHECK_EQ(player->get_coins(), 0);
    CHECK_EQ(merchant->get_coins(), 0);
}
