//melissaliebowitz@yahoo.com
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Player.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
#include "Game.hpp"
using namespace coup;
using namespace std;


sf::Font loadFont() {
    sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        throw std::runtime_error("Failed to load font from assets/arial.ttf");
    }
    return font;
}

sf::Text makeText(const std::string& str, const sf::Font& font, unsigned size, float x, float y) {
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    text.setFillColor(sf::Color::White);
    return text;
}

void addButton(std::vector<sf::RectangleShape>& buttons, std::vector<sf::Text>& labels,
               const std::string& labelText, const sf::Font& font, float x, float y)
{
    sf::RectangleShape btn(sf::Vector2f(120, 40));
    btn.setPosition(x, y);
    btn.setFillColor(sf::Color::Green);
    buttons.push_back(btn);

    sf::Text label = makeText(labelText, font, 18, x + 10, y + 5);
    labels.push_back(label);
}


int main() {
    Game game;
    Player *player1 = new Player(game, "Alice");
    Player *player2 = new Player(game, "Bob");
    Player *player3 = new Player(game, "John");
    Player *player4 = new Player(game, "Kevin");
    Player *player5 = new Player(game, "Peter");
    Player *player6 = new Player(game, "Becky");
    
    game.start_game();

    sf::RenderWindow window(sf::VideoMode(800, 700), "Coup Game GUI");
    sf::Font font = loadFont();

    sf::Text infoText = makeText("Welcome to Coup!", font, 24, 50, 20);
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonLabels;
    std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Coup", "Sanction", "Arrest"};

    //making the action buttons
    for (size_t i = 0; i < actions.size(); ++i) {
        sf::RectangleShape btn(sf::Vector2f(120, 40));
        btn.setPosition(550, 50 + i * 60);
        btn.setFillColor(sf::Color::Blue);
        buttons.push_back(btn);

        sf::Text label = makeText(actions[i], font, 18, 560, 55 + i * 60);
        buttonLabels.push_back(label);
    }

    std:: string pendingTargetAction = "";
    std:: string pendingCancelAction = "";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePos = sf::Mouse::getPosition(window);
                Player* current = game.get_current_player();

                if (!pendingTargetAction.empty())
                {
                    for (size_t i = 0; i < game.get_players().size(); i++)
                    {
                        sf:: FloatRect playerBox(50, 400 + i * 40, 200, 30);
                        if (playerBox.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            Player *target = game.get_players()[i];
                            try
                            {
                                if (pendingTargetAction == "Coup") current->coup(*target);
                                else if (pendingTargetAction == "Arrest") current->arrest(*target);
                                else if (pendingTargetAction == "Sanction") current->sanction(*target);
                                else if(pendingTargetAction == "Peek")
                                {
                                    Spy *spy = dynamic_cast<Spy*>(current);
                                    if(spy)
                                    {
                                        spy->peek(*target);
                                    }
                                } else if (pendingTargetAction == "Block Arrest")
                                {
                                    Spy *spy = dynamic_cast<Spy*>(current);
                                    if (spy)
                                    {
                                        spy->block_arrest(*target);
                                    }
                                }

                                pendingTargetAction = "";

                            }
                            catch(const std::exception& e)
                            {
                                std::cerr << "Actioned failed: " << e.what() << '\n';
                                pendingTargetAction = "";
                            }
                            
                        }
                        
                    } 

                }
                else if (!pendingCancelAction.empty())
                {
                    for (size_t i = 0; i < game.get_players().size(); i++)
                    {
                        sf:: FloatRect playerBox(50, 400 + i * 40, 200, 30);
                        if (playerBox.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            Player *selected = game.get_players()[i];

                            if (pendingCancelAction == "Cancel Bribe")
                            {
                                for (Player *p : game.get_players())
                                {
                                    if (!p->is_alive()) continue;

                                    Judge *judge = dynamic_cast<Judge*>(p);
                                    if (judge)
                                    {
                                        for (auto it = game.get_blockable_actions().begin(); it != game.get_blockable_actions().end(); it++)
                                        {
                                            if (it->action == action_type:: bribe && it->played_by == selected)
                                            {
                                                try
                                                {
                                                    judge->cancel_bribe(*selected);
                                                }
                                                catch(const std::exception& e)
                                                {
                                                    std::cerr << e.what() << '\n';
                                                }
                                                break;
                                            }
                                        } 
                                    }
                                }
                            } else if (pendingCancelAction == "Cancel Coup")
                            {
                                for (Player *p : game.get_players())
                                {
                                    General *general = dynamic_cast<General*>(p);
                                    if (!general) continue;
                                    
                                    for (auto it = game.get_blockable_actions().begin(); it != game.get_blockable_actions().end(); it++)
                                    {
                                        if (it->action == action_type::coup && it->played_on == selected)
                                        {
                                            try
                                            {
                                                general->cancel_coup(*selected);
                                            }
                                            catch(const std::exception& e)
                                            {
                                                std::cerr << e.what() << '\n';
                                            }
                                            break; 
                                        } 
                                    }
                                    
                                }
                            } else if (pendingCancelAction == "Cancel Tax")
                            {
                                for (Player *p : game.get_players())
                                {
                                    if(!p->is_alive()) continue;
                                    
                                    Governor *governor = dynamic_cast<Governor*>(p);
                                    if (governor)
                                    {
                                        for (auto it = game.get_blockable_actions().begin(); it != game.get_blockable_actions().end(); it++)
                                        {
                                            if (it->action == action_type:: tax && it->played_by == selected)
                                            {
                                                try
                                                {
                                                    governor->cancel_tax(*selected);
                                                }
                                                catch(const std::exception& e)
                                                {
                                                    std::cerr << e.what() << '\n';
                                                }
                                                break;
                                            }
                                            
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                            pendingCancelAction = "";
                        }
                        
                    }
                    
                }
                else {
                
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        try {
                            if (actions[i] == "Gather") current->gather();
                            else if (actions[i] == "Tax") current->tax();
                            else if (actions[i] == "Bribe") current->bribe();
                            else if (actions[i] == "Coup" || actions[i] == "Sanction" || actions[i] == "Arrest")
                            {
                                pendingTargetAction = actions[i];
                                std:: cout << "Select a player to " << actions[i] << "." << std:: endl;
                                break; 
                            }
                            
                        } catch (const std::exception& e) {
                            std::cerr << "Action failed: " << e.what() << std::endl;
                        }
                    }
                }

                for (size_t i = actions.size(); i < buttons.size(); i++)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        try
                        {
                            if (buttonLabels[i].getString() == "Peek")
                            {
                                Spy *spy = dynamic_cast<Spy*>(current);
                                if (spy)
                                {
                                    std:: cout << "Select a player to peek." << std:: endl;
                                    pendingTargetAction = "Peek";
                                }
                            }else if(buttonLabels[i].getString() == "Block Arrest")
                            {
                                Spy *spy = dynamic_cast<Spy*>(current);
                                if (spy)
                                {
                                    std:: cout << "Select a player to block from playing arrest." << std:: endl;
                                    pendingTargetAction = "Block Arrest";
                                }
                            }else if (buttonLabels[i].getString() == "Invest")
                            {
                                Baron *baron = dynamic_cast<Baron*>(current);
                                if (baron)
                                {
                                    baron->invest();
                                } 
                            } else if (buttonLabels[i].getString() == "Cancel Bribe")
                            {
                                for (Player *p : game.get_players())
                                {   
                                    if (!p->is_alive()) continue;
                                    Judge *judge = dynamic_cast<Judge*>(p);
                                    if (judge)
                                    {   
                                        std:: cout << "Select a player to cancel their bribe." << std:: endl;
                                        pendingCancelAction = "Cancel Bribe";
                                    }
                                }
                            } else if (buttonLabels[i].getString() == "Cancel Coup")
                            {
                                for (Player *p : game.get_players())
                                {
                                    General *general = dynamic_cast<General*>(p);
                                    if (general)
                                    {
                                        std:: cout << "Select a player to cancel coup played on them."  << std:: endl;
                                        pendingCancelAction = "Cancel Coup";
                                    }
                                }
                            } else if (buttonLabels[i].getString() == "Cancel Tax")
                            {
                                for (Player *p : game.get_players())
                                {
                                    if(!p->is_alive()) continue;
                                    Governor *governor = dynamic_cast<Governor*>(p);
                                    if (governor)
                                    {
                                        std:: cout << "Select a player to cancel tax." << std:: endl;
                                        pendingCancelAction = "Cancel Tax";
                                    }
                                    
                                }
                                
                            }
                            
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << "Special Action failed: " << e.what() << '\n';
                        }
                    }
                }
            }
        }
        
    }

        // Updating player info
        Player *current = game.get_current_player();
        std::string display = "Current player's turn: " + current->get_name() + " | Coins: " + std:: to_string(game.get_current_player()->get_coins()) + "\n";
        for (Player* p : game.get_players()) {
            display += p->get_name() + " | Role: " + p->get_job();
            display += p->is_alive() ? "\n" : " [OUT]\n";
        }
        infoText.setString(display);
  
        window.clear();
        window.draw(infoText);
       
        buttons.resize(actions.size());
        buttonLabels.resize(actions.size());
        
        float specialBtnX = 550;
        float specialBtnY = 50 + actions.size() * 60;
        float specialBtnOffset = 0;

        if (dynamic_cast<Spy*>(current))
        {
            addButton(buttons, buttonLabels, "Peek", font, specialBtnX, specialBtnY + specialBtnOffset);
            specialBtnOffset += 60;
            addButton(buttons, buttonLabels, "Block Arrest", font, specialBtnX, specialBtnY + specialBtnOffset);
            specialBtnOffset += 60;
        }
        if (dynamic_cast<Baron*>(current))
        {
            addButton(buttons, buttonLabels, "Invest", font, specialBtnX, specialBtnY + specialBtnOffset);
            specialBtnOffset += 60;
        }

        auto& ca = game.get_blockable_actions();
        bool hasCancelBribe = false;
        bool hasCancelCoup = false;
        bool hasCancelTax = false;
        for (auto it = ca.begin(); it != ca.end(); it++)
        {   
            for (Player *p : game.get_players())
            {
                if (dynamic_cast<General*>(p) && it->action == action_type:: coup && it->count_turn < game.get_turn_counter() + game.get_players_name().size() && !hasCancelCoup)
                {
                    addButton(buttons, buttonLabels, "Cancel Coup", font, specialBtnX, specialBtnY + specialBtnOffset);
                    specialBtnOffset += 60;
                    hasCancelCoup = true;
                }
                
                if (!p->is_alive()) continue;

                if (dynamic_cast<Judge*>(p) && it->action == action_type:: bribe && it->count_turn < game.get_turn_counter() + game.get_players_name().size() && !hasCancelBribe)
                {
                    addButton(buttons, buttonLabels, "Cancel Bribe", font, specialBtnX , specialBtnY + specialBtnOffset);
                    specialBtnOffset += 60;
                    hasCancelBribe = true;
                }

                if (dynamic_cast<Governor*>(p) && it->action == action_type:: tax && it->count_turn < game.get_turn_counter() + game.get_players_name().size() && !hasCancelTax)
                {
                    addButton(buttons, buttonLabels, "Cancel Tax", font, specialBtnX, specialBtnY + specialBtnOffset);
                    specialBtnOffset += 60;
                    hasCancelTax = true;
                }
            }
            
        }
        

        //draw the action buttons
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonLabels[i]);
        }

        //Draw player selection box
        if (!pendingTargetAction.empty())
        {
            for (size_t i = 0; i < game.get_players().size(); i++)
            {
                Player *p = game.get_players()[i];
                sf:: RectangleShape box(sf:: Vector2f(200, 30));
                box.setPosition(50, 400 + i* 40);
                box.setFillColor(p->is_alive() ? sf:: Color(255,0,255) : sf:: Color(100,100,100));
                window.draw(box);

                sf:: Text nameText = makeText(p->get_name(), font, 16, 60, 405 +  i * 40);
                window.draw(nameText);
            }
            
        }

        if (!pendingCancelAction.empty())
        {
            for (size_t i = 0; i < game.get_players().size(); i++)
            {
                Player *p = game.get_players()[i];
                bool canCancel = false;

                for (const auto& ca : game.get_blockable_actions())
                {
                    if (pendingCancelAction == "Cancel Bribe" && ca.action == action_type:: bribe 
                        && ca.played_by == p)
                    {
                        canCancel = true;
                        break;
                    } else if(pendingCancelAction == "Cancel Coup" && ca.action == action_type:: coup 
                        && ca.played_on == p){
                        canCancel = true;
                        break;
                    } else if (pendingCancelAction == "Cancel Tax" && ca.action == action_type:: tax 
                        && ca.played_by == p)
                    {
                        canCancel = true;
                        break;
                    } 
                }
                
                if (!canCancel) continue;
                sf:: RectangleShape box(sf:: Vector2f(200, 30));
                box.setPosition(50, 400 + i * 40);
                box.setFillColor(sf:: Color(0,200,0));
                window.draw(box);

                sf:: Text nameText = makeText(p->get_name(), font, 16, 60, 405 + i * 40);
                window.draw(nameText);

            }
            
        }

        
        int counter = 0;
        for (Player *p : game.get_players())
        {
            if (p->is_alive())
            {
                counter++;
            } 
        }
            
        if (counter == 1)
        {
            
            window.clear();
            
            std:: string message = game.winner()->get_name() + " wins the game!";
            sf:: Text winText = makeText(message, font, 30, 100, 300);
            winText.setFillColor(sf:: Color:: Yellow);
            window.draw(winText);

            sf:: Text instructionText = makeText("Press ESC to exit the game.", font, 10, 100, 350);
            window.draw(instructionText);

            window.display();
            
            sf:: Event event;
            while (window.waitEvent(event))
            {
                if (event.type == sf:: Event::Closed || 
                    (event.type == sf:: Event:: KeyPressed && event.key.code == sf:: Keyboard:: Escape))
                {
                    window.close();
                    break;
                }
                
            }
            
            break;
        }

        window.display();
    }

    return 0;
}


    