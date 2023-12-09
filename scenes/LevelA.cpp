/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelA.h"
#include "../Utility.h"

#define LEVEL_WIDTH 32
#define LEVEL_HEIGHT 16
#define ENEMY_COUNT 1

unsigned int LEVEL_A_DATA[] =
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

LevelA::~LevelA()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.pop_sfx);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_A_DATA, map_texture_id, 1.0f, 12, 22);
    
    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(7.0f, -6.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(7.0f, -6.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/knight_spritesheet.png");
    
    // Animation
    m_state.player->m_walking[m_state.player->RIGHT] = new int[2] { 0, 1 };
    m_state.player->m_walking[m_state.player->LEFT] = new int[2] { 2, 3 };
    
    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_frames = 2;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 1;
    m_state.player->set_height(0.9f);
    m_state.player->set_width(0.9f);
    
    // Jumping
    m_state.player->m_jumping_power = 8.0f;
    
    // ————— ENEMY SET-UP ————— //
    //BUG
    GLuint bug_texture_id = Utility::load_texture("assets/images/tilemap-bug.png");
    GLuint wasp_texture_id = Utility::load_texture("assets/images/tilemap-characters_packed.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(BUG);
    m_state.enemies[0].set_ai_state(PATROL);
    m_state.enemies[0].m_texture_id = bug_texture_id;
    m_state.enemies[0].set_position(glm::vec3(7.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(7.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_speed(0.5f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    m_state.enemies[0].m_walking[m_state.enemies[0].LEFT] = new int[2] { 0, 1 };
    m_state.enemies[0].m_walking[m_state.enemies[0].RIGHT] = new int[2] { 2, 3 };
                 
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].LEFT];
    m_state.enemies[0].m_animation_frames = 2;
    m_state.enemies[0].m_animation_index = 0;
    m_state.enemies[0].m_animation_time = 0.0f;
    m_state.enemies[0].m_animation_cols = 4;
    m_state.enemies[0].m_animation_rows = 1;
    m_state.enemies[0].set_height(1.0f);
    m_state.enemies[0].set_width(1.0f);
    
    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/tabun.wav");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 32.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump1.wav");
    Mix_VolumeChunk(m_state.jump_sfx, MIX_MAX_VOLUME / 32.0f);

    m_state.pop_sfx = Mix_LoadWAV("assets/audio/bubble1.wav");
    Mix_VolumeChunk(m_state.pop_sfx, MIX_MAX_VOLUME / 32.0f);
}

void LevelA::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.pop_sfx);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.pop_sfx);
    }
}


void LevelA::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
