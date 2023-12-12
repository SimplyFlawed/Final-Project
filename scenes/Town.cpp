/**
* Author: Raymond Lin
* Assignment: Knight's Descent
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "town.h"
#include "../Utility.h"

#define LEVEL_WIDTH 32
#define LEVEL_HEIGHT 16
#define ENEMY_COUNT 2

unsigned int TOWN_DATA[] =
{
    19,19,19,32,80,81,45,81,45,81,45,81,45,81,45,81,45,81,45,81,45,81,45,81,45,81,82,21,22,22,22,22,
    19,19,32,0,0,0,0,5,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,27,0,0,0,33,22,22,22,
    19,32,0,52,53,54,0,0,0,0,0,28,0,0,0,0,0,0,0,0,48,49,50,0,0,2,1,0,21,22,22,22,
    19,20,2,64,65,66,52,53,54,2,0,0,0,0,0,4,0,0,1,0,60,63,62,0,0,0,3,0,0,33,22,22,
    19,20,1,76,88,79,64,67,66,0,2,0,92,0,1,16,0,0,0,0,72,85,75,81,82,0,15,0,1,0,33,22,
    19,20,0,76,77,77,76,89,79,0,0,0,104,0,0,0,0,0,0,1,0,43,0,0,0,0,0,0,1,0,21,22,
    19,20,0,0,0,0,0,43,1,0,0,0,43,0,0,0,27,1,0,0,0,43,0,0,0,0,0,0,0,0,21,22,
    19,20,0,0,0,2,0,43,0,1,17,0,43,0,0,0,0,0,0,43,43,43,0,0,102,0,0,102,0,0,21,22,
    19,20,1,0,0,0,0,43,43,43,43,43,43,43,43,43,43,43,43,43,0,2,0,0,190,103,97,190,0,0,21,22,
    19,20,0,1,0,0,0,0,17,0,0,0,0,0,43,0,0,0,0,43,0,0,0,96,190,121,121,190,98,0,21,22,
    19,20,1,44,45,45,45,48,51,49,50,0,0,0,43,0,1,0,1,43,0,0,0,120,189,111,112,191,122,0,21,22,
    19,20,0,56,94,43,94,60,61,63,62,1,1,0,43,0,0,0,0,43,0,27,0,189,189,123,124,191,191,0,21,22,
    19,20,0,68,82,43,80,72,84,85,75,0,0,0,43,0,2,4,0,43,0,0,1,0,0,43,43,0,1,0,21,22,
    19,8,0,2,0,0,0,0,0,43,43,43,43,43,43,0,0,16,0,43,29,0,0,1,0,43,43,0,0,1,9,22,
    19,19,8,0,0,0,1,0,0,1,0,0,17,0,0,0,0,0,0,43,43,43,43,43,43,43,43,0,1,9,22,22,
    19,19,19,20,80,81,81,81,45,81,81,81,45,81,81,81,45,81,81,81,45,81,81,81,45,81,81,82,9,22,22,22
};

Town::~Town()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.sfx[SWORD_SFX]);
    Mix_FreeChunk(m_state.sfx[KILL_SFX]);
    Mix_FreeChunk(m_state.sfx[DEATH_SFX]);
    Mix_FreeChunk(m_state.sfx[GAMEOVER_SFX]);
    Mix_FreeChunk(m_state.sfx[WIN_SFX]);
    Mix_FreeChunk(m_state.sfx[ENTER_SFX]);
    
    for (int i = 0; i < m_number_of_sfx; i++)
    {
        delete m_state.sfx[i];
    }

    delete [] m_state.sfx;
}

void Town::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, TOWN_DATA, map_texture_id, 1.0f, 12, 22);
    
    GLuint shadow_texture_id = Utility::load_texture("assets/images/shadow.png");

    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(7.0f, -6.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(7.0f, -6.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(3.0f);
    m_state.player->m_texture_id = Utility::load_texture("assets/images/knight_spritesheet.png");
    
    // Animation
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 1, 5, 9, 13 };
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };

    m_state.player->m_attack[m_state.player->DOWN] = new int[4] { 16, 16, 16, 16 };
    m_state.player->m_attack[m_state.player->UP] = new int[4] { 17, 17, 17, 17 };
    m_state.player->m_attack[m_state.player->LEFT] = new int[4] { 18, 18, 18, 18 };
    m_state.player->m_attack[m_state.player->RIGHT] = new int[4] { 19, 19, 19, 19 };
    
    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->DOWN];
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 7;
    m_state.player->m_facing = m_state.player->DOWN;
    m_state.player->set_height(0.7f);
    m_state.player->set_width(0.7f);

    // Shadow
    m_state.player->shadow = new Entity();
    m_state.player->shadow->set_entity_type(SHADOW);
    m_state.player->shadow->set_position(glm::vec3(m_state.player->get_position().x, m_state.player->get_position().y - 0.5f, 0.0f));
    m_state.player->shadow->set_movement(glm::vec3(0.0f));
    m_state.player->shadow->set_scale(glm::vec3(0.8f, 0.3f, 0.0f));
    m_state.player->shadow->m_texture_id = shadow_texture_id;

    // ————— PLAYER'S WEAPON SET-UP ————— //
    m_state.player->weapon = new Entity();
    m_state.player->weapon->set_entity_type(WEAPON);
    m_state.player->weapon->set_position(m_state.player->get_position());
    m_state.player->weapon->set_init_pos(m_state.player->get_position());
    m_state.player->weapon->set_movement(glm::vec3(0.0f));
    m_state.player->weapon->set_speed(3.0f);
    m_state.player->weapon->m_texture_id = Utility::load_texture("assets/images/sword_spritesheet.png");

    m_state.player->weapon->m_attack[m_state.player->weapon->DOWN] = new int[1] { 0 };
    m_state.player->weapon->m_attack[m_state.player->weapon->UP] = new int[1] { 1 };
    m_state.player->weapon->m_attack[m_state.player->weapon->LEFT] = new int[1] { 2 };
    m_state.player->weapon->m_attack[m_state.player->weapon->RIGHT] = new int[1] { 3 };

    m_state.player->weapon->m_animation_indices = m_state.player->weapon->m_attack[m_state.player->weapon->DOWN];
    m_state.player->weapon->m_animation_frames = 1;
    m_state.player->weapon->m_animation_index = 0;
    m_state.player->weapon->m_animation_time = 0.0f;
    m_state.player->weapon->m_animation_cols = 4;
    m_state.player->weapon->m_animation_rows = 1;
    m_state.player->weapon->set_height(0.5f);
    m_state.player->weapon->set_width(0.5f);
    m_state.player->weapon->deactivate();

    // ————— ENEMY SET-UP ————— //
    GLuint snake_texture_id = Utility::load_texture("assets/images/snake_spritesheet.png");

    m_state.enemies = new Entity[ENEMY_COUNT];

    // SNAKES
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        // Existing
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_ai_type(SNAKE);
        m_state.enemies[i].m_texture_id = snake_texture_id;
        m_state.enemies[i].set_speed(2.0f);
        m_state.enemies[i].set_hp(1);

        // Animation
        m_state.enemies[i].m_walking[m_state.enemies[i].DOWN] = new int[4] { 0, 4, 8, 12 };
        m_state.enemies[i].m_walking[m_state.enemies[i].UP] = new int[4] { 1, 5, 9, 13 };
        m_state.enemies[i].m_walking[m_state.enemies[i].LEFT] = new int[4] { 2, 6, 10, 14 };
        m_state.enemies[i].m_walking[m_state.enemies[i].RIGHT] = new int[4] { 3, 7, 11, 15 };

        m_state.enemies[i].m_animation_frames = 4;
        m_state.enemies[i].m_animation_index = 0;
        m_state.enemies[i].m_animation_time = 0.0f;
        m_state.enemies[i].m_animation_cols = 4;
        m_state.enemies[i].m_animation_rows = 4;
        m_state.enemies[i].set_height(1.0f);
        m_state.enemies[i].set_width(1.0f);
    }
    // SNAKE 1
    m_state.enemies[0].set_ai_state(ROAM_HORIZONTAL);
    m_state.enemies[0].set_position(glm::vec3(25.0f, -12.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(25.0f, -12.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].LEFT];

    // SNAKE 2
    m_state.enemies[1].set_ai_state(ROAM_VERTICAL);
    m_state.enemies[1].set_position(glm::vec3(19.0f, -10.0f, 0.0f));
    m_state.enemies[1].set_init_pos(glm::vec3(19.0f, -10.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));

    m_state.enemies[1].m_animation_indices = m_state.enemies[1].m_walking[m_state.enemies[1].DOWN];

    // Shadows
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].shadow = new Entity();
        m_state.enemies[i].shadow->set_entity_type(SHADOW);
        m_state.enemies[i].shadow->set_position(glm::vec3(m_state.enemies[i].get_position().x, m_state.enemies[i].get_position().y - 0.5f, 0.0f));
        m_state.enemies[i].shadow->set_movement(glm::vec3(0.0f));
        m_state.enemies[i].shadow->set_scale(glm::vec3(0.8f, 0.3f, 0.0f));
        m_state.enemies[i].shadow->m_texture_id = shadow_texture_id;
    }

    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/Overdose.wav");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 32.0f);

    m_state.sfx = new Mix_Chunk*[m_number_of_sfx];

    m_state.sfx[SWORD_SFX] = Mix_LoadWAV("assets/audio/Sword.wav");
    Mix_VolumeChunk(m_state.sfx[SWORD_SFX], MIX_MAX_VOLUME / 128.0f);

    m_state.sfx[KILL_SFX] = Mix_LoadWAV("assets/audio/Kill.wav");
    Mix_VolumeChunk(m_state.sfx[KILL_SFX], MIX_MAX_VOLUME / 64.0f);

    m_state.sfx[DEATH_SFX] = Mix_LoadWAV("assets/audio/Death.wav");
    Mix_VolumeChunk(m_state.sfx[DEATH_SFX], MIX_MAX_VOLUME / 16.0f);

    m_state.sfx[GAMEOVER_SFX] = Mix_LoadWAV("assets/audio/GameOver.wav");
    Mix_VolumeChunk(m_state.sfx[GAMEOVER_SFX], MIX_MAX_VOLUME / 16.0f);

    m_state.sfx[WIN_SFX] = Mix_LoadWAV("assets/audio/Win.wav");
    Mix_VolumeChunk(m_state.sfx[WIN_SFX], MIX_MAX_VOLUME / 16.0f);

    m_state.sfx[ENTER_SFX] = Mix_LoadWAV("assets/audio/Enter.wav");
    Mix_VolumeChunk(m_state.sfx[ENTER_SFX], MIX_MAX_VOLUME / 8.0f);
    
}

void Town::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.sfx);
    m_state.player->weapon->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.sfx);
    m_state.player->shadow->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);
        m_state.enemies[i].shadow->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);
    }
}

void Town::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->shadow->render(program);
    m_state.player->render(program);
    m_state.player->weapon->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].shadow->render(program);
        m_state.enemies[i].render(program);
    }
    
}
