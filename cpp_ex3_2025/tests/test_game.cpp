//melissaliebowitz@yahoo.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/Baron.hpp"
#include "../include/General.hpp"
#include "../include/Governor.hpp"
#include "../include/Judge.hpp"
#include "../include/Merchant.hpp"
#include "../include/Spy.hpp"
using namespace coup;

TEST_CASE("Game constructor Test")
{
    Game game;
    CHECK_EQ(game.get_current_turn(), 0);
    CHECK_EQ(game.get_last_arrest_count(),-2);
    CHECK_EQ(game.get_turn_counter(), 0);
    CHECK_EQ(game.get_last_arrested(), nullptr);

}

TEST_CASE("Start Game Error Throwing Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");
    Player *p4 = new Player(game, "p4");
    Player *p5 = new Player(game, "p5");
    Player *p6 = new Player(game, "p6");
    
    CHECK_THROWS_WITH((new Player(game, "p7")), "Game can have up to 6 players.");
}

TEST_CASE("Start Game Test")
{   
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");
    Player *p4 = new Player(game, "p4");
    Player *p5 = new Player(game, "p5");
    Player *p6 = new Player(game, "p6");

    game.start_game();

    CHECK_EQ(game.get_turn_counter() ,1);

}

TEST_CASE("Add Player Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");
    Player *p4 = new Player(game, "p4");
    Player *p5 = new Player(game, "p5");
    Player *p6 = new Player(game, "p6");

    CHECK_EQ(game.get_players().size(), 6);

    CHECK_THROWS_WITH((new Player(game, "p7")), "Game can have up to 6 players.");

}

TEST_CASE("Player Index Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");

    CHECK_EQ(game.get_player_index(p1), 0);
    CHECK_EQ(game.get_player_index(p3), 2);
    CHECK_EQ(game.get_player_index(p2), 1);

}

TEST_CASE("Get Name of Alive Players Test")
{
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");

    CHECK_EQ(p1->get_name(), game.get_players_name()[0]);
    CHECK_EQ(p2->get_name(), game.get_players_name()[1]);
    CHECK_EQ(p3->get_name(), game.get_players_name()[2]);
    
    p1->eliminate();

    CHECK_EQ(p2->get_name(), game.get_players_name()[0]);
    CHECK_EQ(p3->get_name(), game.get_players_name()[1]);
}

TEST_CASE("Blockable Action Vector Test")
{
    Game game;
    Player *player = new Player(game, "Player");
    game.add_blockable_action(action_type:: tax, player, nullptr, 2);

    CHECK_EQ(game.get_blockable_actions().size(), 1);
    CHECK_EQ(game.get_blockable_actions()[0].action, action_type:: tax);
    CHECK_EQ(game.get_blockable_actions()[0].count_turn, 2);

}

TEST_CASE("Set Last Arrested Test")
{
    Game game;
    Player *player1 = new Player(game, "Player1");

    game.set_last_arrested(player1, 3);
    CHECK_EQ(game.get_last_arrested(), player1);
    CHECK_EQ(game.get_last_arrest_count(), 3);

}

TEST_CASE("Turn and Next Turn Test")
{
    Game game;
    CHECK_THROWS_WITH(game.turn(), "No players in the game");

    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");
    Player *p4 = new Player(game, "p4");
    Player *p5 = new Player(game, "p5");

    CHECK_EQ(game.turn(), p1->get_name());
    game.next_turn();
    CHECK_EQ(game.turn(), p2->get_name());
    p3->eliminate();
    game.next_turn();
    CHECK_EQ(game.turn(), p4->get_name());
    game.next_turn();
    CHECK_EQ(game.turn(), p5->get_name());

}

TEST_CASE("Winner Test")
{   
    Game game;
    Player *p1 = new Player(game, "p1");
    Player *p2 = new Player(game, "p2");
    Player *p3 = new Player(game, "p3");

    CHECK_EQ(game.get_players_name().size(), 3);
    p2->eliminate();
    p3->eliminate();

    CHECK_EQ(game.winner(), p1);
}