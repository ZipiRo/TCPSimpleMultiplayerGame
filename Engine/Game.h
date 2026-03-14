#pragma once

#include <random>

enum GamePhase
{
    WAITING,
    PLAY,
    GAMEOVER
};

struct HitAction {
    int damage;
};

enum HitResult
{
    HIT_OK,
    HIT_GAME_OVER,
    HIT_INVALID
};

struct PlayerState
{
    int health;
    int loose_health;
    int min_damage;
    int max_damage;
    int hit_count;
};

struct GameState
{
    GamePhase game_phase;
    PlayerState player[2];
    int current_player;
    int winner;
};

namespace Engine
{
    void Init(GameState &game_state)
    {
        game_state.game_phase = WAITING;

        for(int i = 0; i < 2; i++)
        {
            game_state.player[i].health = 100;
            game_state.player[i].loose_health = rand() % 100 + 1;
            game_state.player[i].min_damage = rand() % 10 + 1;
            game_state.player[i].max_damage =
                game_state.player[i].min_damage + rand() % 15 + 5;
            game_state.player[i].hit_count = 0;
        }

        game_state.current_player = -1;
        game_state.winner = -1;
    }

    void Start(GameState &game_state)
    {
        game_state.game_phase = PLAY;
        game_state.current_player = 0;
    }

    HitResult Hit(GameState &game_state, const HitAction &action)
    {
        if(game_state.game_phase != PLAY) return HIT_INVALID;
        
        int current_player_index = game_state.current_player;

        if(action.damage < game_state.player[current_player_index].min_damage) return HIT_INVALID;
        if(action.damage > game_state.player[current_player_index].max_damage) return HIT_INVALID;

        PlayerState &current_player = game_state.player[current_player_index];
        PlayerState &other_player = game_state.player[1 - current_player_index];
        
        other_player.health -= action.damage;
        current_player.hit_count++;

        if(other_player.health < other_player.loose_health) 
        {
            game_state.winner = 1 - current_player_index;
            game_state.game_phase = GAMEOVER;
            return HIT_GAME_OVER;
        }
        else if(other_player.health == other_player.loose_health)
        {
            game_state.winner = current_player_index;
            game_state.game_phase = GAMEOVER;
            return HIT_GAME_OVER;
        }

        game_state.current_player = 1 - current_player_index;

        return HIT_OK;
    }
}