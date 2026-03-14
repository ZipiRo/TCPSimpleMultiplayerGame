#include <iostream>
#include <random>
#include <conio.h>

#include <Game.h>   

void PlayUI(GameState &game)
{
    system("cls");

    int current_player_index = game.current_player;
    int min_damage = game.player[current_player_index].min_damage;
    int max_damage = game.player[current_player_index].max_damage;

    std::cout << "PLAYER: " << current_player_index + 1 << '\n';
    std::cout << "PLAYER HIT COUNT: " << game.player[current_player_index].hit_count << '\n';
    std::cout << "WIN ENEMY HEALTH TARGET: " << game.player[1 - current_player_index].loose_health << '\n';
    std::cout << "ENEMY HEALTH: " << game.player[1 - current_player_index].health << '\n';
    std::cout << "===============================================================\n";

    std::cout << "INPUT SOME DAMAGE TO GET TO THE TARGET ("<< min_damage << " - " << max_damage << "): ";
    
    int damage = 0;

    while(!(std::cin >> damage))
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "INVALID NUMBER: ";
    }
    
    HitResult hit_result = Engine::Hit(game, {damage});

    std::cout << '\n';

    if(hit_result == HIT_OK)
    {
        system("cls");
        std::cout << "PLAYER: " << current_player_index + 1 << '\n';
        std::cout << "ENEMY TOOK : " << damage << " POINTS OF DAMAGE\n";

        int difference = game.player[1 - current_player_index].health - 
                            game.player[1 - current_player_index].loose_health;

        std::cout << "DISTANCE TO TARGET: " << difference << '\n';

        std::cout << "===============================================================\n";
        std::cout << "PRESS ANY KEY TO CONTINUE...\n";

        getch();
    }
    else if(hit_result == HIT_GAME_OVER)
    {
        system("cls");
        std::cout << "PLAYER: " << current_player_index + 1 << " CAUSED GAME END\n";
        std::cout << "===============================================================\n";
        std::cout << "PRESS ANY KEY TO CONTINUE...\n";

        getch();
    }
    else if(hit_result == HIT_INVALID)
    {
        system("cls");
        std::cout << "HIT ACTION WAS INVALID!!!\n";
        std::cout << "===============================================================\n";
        std::cout << "PRESS ANY KEY TO CONTINUE...\n";

        getch();
    }
}   

void GameOverUI(GameState &game)
{
    if(game.game_phase != GAMEOVER) return;

    system("cls");
    std::cout << "PLAYER: " << game.winner + 1 << " IS THE WINNER WITH " << game.player[game.winner].hit_count << " HIT\\S\n";
    std::cout << "===============================================================\n";
    std::cout << "PRESS ANY KEY TO CONTINUE...\n";

    getch();
}

int main()
{
    srand(time(0));
    
    GameState game;
    Engine::Init(game);

    while (true)
    {
        switch (game.game_phase)
        {
        case WAITING:
            Engine::Start(game);
            break;
        case PLAY:
            PlayUI(game);
            break;
        case GAMEOVER:
            GameOverUI(game);
            return 0;
        default:
            break;
        }
    }

    return 0;
}