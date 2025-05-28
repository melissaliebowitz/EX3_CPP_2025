//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/Judge.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Judge Constructor Test")
{
    Game game;
    Judge *judge = new Judge(game, "Steve");
    CHECK_EQ(game.get_players().size(), 1);

    CHECK_EQ(judge->get_name(), "Steve");
    CHECK_EQ(judge->is_alive(), true);
    CHECK_EQ(judge->get_coins(), 0);
    CHECK_EQ(judge->get_extra_turns(), 0);
    CHECK_EQ(judge->get_arrest_blocked_turn(), -1);
    CHECK_EQ(judge->get_arrest_blocked(), false);
    CHECK_EQ(judge->get_pending_bribe(), false);
    CHECK_EQ(judge->is_sanctioned(), false);

}

TEST_CASE("Judge Copy Constructor")
{   
    Game game;
    Player *base = new Player(game, "Lily");
    base->set_coins(9);
    base->set_extra_turns(1);

    Judge *judge = new Judge(*base);
    CHECK_EQ(judge->get_name(), "Lily");
    CHECK_EQ(judge->is_alive(), true);
    CHECK_EQ(judge->get_coins(), 9);
    CHECK_EQ(judge->get_extra_turns(), 1);
    CHECK_EQ(judge->get_arrest_blocked_turn(), -1);
    CHECK_EQ(judge->get_arrest_blocked(), false);
    CHECK_EQ(judge->get_pending_bribe(), true);
    CHECK_EQ(judge->is_sanctioned(), false);

}

TEST_CASE("Get Job Test")
{
    Game game;
    Judge *judge = new Judge(game, "Judge");

    CHECK_EQ(judge->get_job(), "Judge");

}

TEST_CASE("Cancel Bribe Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Judge *judge = new Judge(game, "Judge");

    player->set_coins(4);
    player->bribe();
    CHECK_EQ(game.get_blockable_actions().size(), 1);
    CHECK_EQ(player->get_extra_turns(), 1);

    judge->cancel_bribe(*player);
    CHECK_EQ(game.get_blockable_actions().size(), 0);
    CHECK_EQ(player->get_extra_turns(), 0);
    CHECK_EQ(player->get_coins(), 0);

}

TEST_CASE("Sanction Action On Judge Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    Judge *judge = new Judge(game, "Judge");

    CHECK_EQ(judge->sanction_penalty() , 1);
    player->set_coins(4);
    player->sanction(*judge);

    CHECK_EQ(player->get_coins(), 0);

}
