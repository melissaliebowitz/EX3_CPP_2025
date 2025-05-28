//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/General.hpp"
#include "../include/Game.hpp"
using namespace coup;

TEST_CASE("Governor Constructor Test")
{
    Game game;
    General *general = new General(game, "John");
    CHECK_EQ(game.get_players().size(), 1);

    CHECK_EQ(general->get_name(), "John");
    CHECK_EQ(general->is_alive(), true);
    CHECK_EQ(general->get_coins(), 0);
    CHECK_EQ(general->get_extra_turns(), 0);
    CHECK_EQ(general->get_arrest_blocked_turn(), -1);
    CHECK_EQ(general->get_arrest_blocked(), false);
    CHECK_EQ(general->get_pending_bribe(), false);
    CHECK_EQ(general->is_sanctioned(), false);
    
}

TEST_CASE("General Copy Constructor")
{
    Game game;
    Player *base = new Player(game, "Jane");
    base->set_coins(6);
    base->set_arrest_block_turn(5);

    General *general = new General(*base);
    CHECK_EQ(general->get_name(), "Jane");
    CHECK_EQ(general->is_alive(), true);
    CHECK_EQ(general->get_coins(), 6);
    CHECK_EQ(general->get_extra_turns(), 0);
    CHECK_EQ(general->get_arrest_blocked_turn(), 5);
    CHECK_EQ(general->get_arrest_blocked(), false);
    CHECK_EQ(general->get_pending_bribe(), false);
    CHECK_EQ(general->is_sanctioned(), false);

}

TEST_CASE("Get Job Test")
{
    Game game;
    General *general = new General(game, "General");

    CHECK_EQ(general->get_job(), "General");

}

TEST_CASE("Cancel Coup Test")
{
    Game game;
    Player *player1 = new Player(game, "P1");
    General *general = new General(game, "General");
    Player *player2 = new Player(game, "P2");

    player1->set_coins(7);
    general->set_coins(4);

    player1->coup(*player2);
    CHECK_EQ(player2->is_alive(), false);
    CHECK_EQ(game.get_blockable_actions().size(), 1);

    CHECK_THROWS_WITH(general->cancel_coup(*player2), "Not enough coins to cancel coup.");

    general->set_coins(5);
    general->cancel_coup(*player2);
    CHECK_EQ(player2->is_alive(), true);
    CHECK_EQ(game.get_blockable_actions().size(), 0);
    CHECK_EQ(player1->get_coins(), 0);

}

TEST_CASE("Arrested Handled By General")
{
    Game game;
    Player *player = new Player(game, "Player");
    General *general = new General(game, "General");
    
    general->set_coins(1);
    player->set_coins(1);
    general->arrested_handle(*player);
    CHECK_EQ(general->get_coins(), 1);
    CHECK_EQ(player->get_coins(), 1);

}
