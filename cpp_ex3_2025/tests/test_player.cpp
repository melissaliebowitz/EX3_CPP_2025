//melissaliebowitz@yahoo.com
#include "doctest.h"
#include "../include/Game.hpp"
#include "../include/Player.hpp"
using namespace coup;


TEST_CASE("Player Constructor Test")
{
    Game game;
    Player *player = new Player(game, "Bob");

    CHECK_EQ(player->get_name(), "Bob");
    CHECK_EQ(player->is_alive(), true);
    CHECK_EQ(player->get_coins(), 0);
    CHECK_EQ(player->get_extra_turns(), 0);
    CHECK_EQ(player->get_arrest_blocked_turn(), -1);
    CHECK_EQ(player->get_arrest_blocked(), false);
    CHECK_EQ(player->get_pending_bribe(), false);
    CHECK_EQ(player->is_sanctioned(), false);
    CHECK_EQ(game.get_players().size(), 1);

}


TEST_CASE("Copy Constructor Test")
{
    Game game;
    Player *base = new Player(game, "Bill");
    base->set_coins(1);

    Player *copy = new Player(*base);
    CHECK_EQ(copy->get_name(), "Bill");
    CHECK_EQ(copy->is_alive(), true);
    CHECK_EQ(copy->get_coins(), 1);
    CHECK_EQ(copy->get_extra_turns(), 0);
    CHECK_EQ(copy->get_arrest_blocked_turn(), -1);
    CHECK_EQ(copy->get_arrest_blocked(), false);
    CHECK_EQ(copy->get_pending_bribe(), false);
    CHECK_EQ(copy->is_sanctioned(), false);
}


TEST_CASE("Set Coins Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    CHECK_EQ(player->get_coins(), 0);

    player->set_coins(3);
    CHECK_EQ(player->get_coins(), 3);

}

TEST_CASE("Set Extra Turns Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    CHECK_EQ(player->get_pending_bribe(), false);
    CHECK_EQ(player->get_extra_turns(), 0);

    player->set_extra_turns(1);
    CHECK_EQ(player->get_pending_bribe(), true);
    CHECK_EQ(player->get_extra_turns(), 1);

    
}

TEST_CASE("Set Pending Bribe Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    CHECK_EQ(player->get_pending_bribe(), false);

    player->set_pending_bribe(true);
    CHECK_EQ(player->get_pending_bribe(), true);

}

TEST_CASE("Decrease Extra Turn Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    CHECK_EQ(player->get_extra_turns(), 0);
    player->set_extra_turns(3);
    CHECK_EQ(player->get_extra_turns(), 3);
    player->decrease_extra_turn();
    CHECK_EQ(player->get_extra_turns(), 2);
    player->decrease_extra_turn();
    CHECK_EQ(player->get_extra_turns(), 1);

}

TEST_CASE("Check Turn")
{
    Game game;
    Player *p1 = new Player(game, "p1");

    p1->eliminate();
    CHECK_THROWS_WITH(p1->check_turn(), "You are out!");

}

TEST_CASE("Handle Arrest Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");

    p1->set_coins(1);

    p1->arrested_handle(*p2);
    CHECK_EQ(p2->get_coins(), 1);

}

TEST_CASE("Gather Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");

    CHECK_EQ(p1->get_coins(), 0);
    CHECK_EQ(p2->get_coins(), 0);

    p1->gather();
    CHECK_EQ(p1->get_coins(), 1);
    p2->gather();
    CHECK_EQ(p2->get_coins(), 1);

}

TEST_CASE("Tax Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");

    CHECK_EQ(p1->get_coins(), 0);
    CHECK_EQ(p2->get_coins(), 0);
    
    p1->tax();
    p2->tax();

    CHECK_EQ(p1->get_coins(), 2);
    CHECK_EQ(p2->get_coins(), 2);

}

TEST_CASE("Arrest Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");

    p3->set_coins(3);
    p1->arrest(*p3);

    CHECK_EQ(p1->get_coins(), 1);
    CHECK_EQ(p3->get_coins(), 2);

    CHECK_THROWS_AS(p2->arrest(*p3), std:: runtime_error);
    
    p2->arrest(*p1);
    CHECK_EQ(p1->get_coins(), 0);
    CHECK_EQ(p2->get_coins(), 1);

    p2->set_coins(0);
    CHECK_THROWS_AS(p3->arrest(*p2), std:: runtime_error);

}

TEST_CASE("Sanction Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");

    CHECK_THROWS_WITH(p1->sanction(*p2), "You do not have enough coins to play sanction on another player.");
    
    p1->set_coins(3);
    p1->sanction(*p2);

    CHECK_EQ(p1->get_coins(), 0);
    CHECK_EQ(p2->is_sanctioned(), true);
    CHECK_EQ(p2->sanction_expired(), false);
    
    p2->tax();
    CHECK_EQ(p2->get_coins(), 0);

}

TEST_CASE("Bribe Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");

    p1->set_coins(2);
    CHECK_THROWS_WITH(p1->bribe(), "Not enough coins to play bribe.");

    p1->set_coins(4);
    p1->bribe();

    CHECK_EQ(p1->get_coins(), 0);
    CHECK_EQ(p1->get_extra_turns(), 1);

    p2->gather();
    p1->gather();
    CHECK_EQ(p1->get_extra_turns(), 0);

    p1->gather();
}


TEST_CASE("Coup Action Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");

    CHECK_THROWS_WITH(p1->coup(*p2), "You don't have enough coins to play coup on another player.");

    p1->set_coins(7);
    p1->coup(*p2);
    CHECK_EQ(game.turn(), p3->get_name());
    p3->gather();

    CHECK_EQ(p2->is_alive(), false);
    CHECK_EQ(game.get_players_name().size(), 2);
}
