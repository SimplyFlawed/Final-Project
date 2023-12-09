/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelB.h"
#include "../Utility.h"

#define LEVEL_WIDTH 26
#define LEVEL_HEIGHT 14
#define ENEMY_COUNT 3

unsigned int LEVEL_B_DATA[] =
{
    122, 122, 122, 122, 122, 122, 122, 122, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0,
    122, 122, 122, 122, 122, 122, 122, 122, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0,
    122, 122, 122, 122, 122, 4, 142, 142, 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0,
    122, 122, 122, 122, 122, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0,
    122, 122, 122, 122, 122, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0,
    4, 142, 142, 142, 142, 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 49, 49, 49, 50, 0, 0, 0, 0,
    123, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    123, 0, 0, 0, 89, 0, 0, 0, 0, 0, 21, 22, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    123, 0, 0, 0, 89, 0, 0, 0, 125, 0, 121, 122, 122, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, 0, 0, 
    123, 0, 0, 0, 109, 0, 0, 21, 22, 22, 25, 122, 122, 123, 0, 0, 0, 0, 0, 21, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 25, 122, 122, 122, 122, 122, 24, 22, 22, 22, 22, 22, 25, 122, 122, 122, 122, 122, 122,
    22, 22, 23, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 21, 22, 23, 122, 122, 122, 122, 122, 122,
    122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 121, 122, 123, 122, 122, 122, 122, 122, 122,
    122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 121, 122, 123, 122, 122, 122, 122, 122, 122
};

LevelB::~LevelB()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.pop_sfx);
}

void LevelB::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap_packed.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_B_DATA, map_texture_id, 1.0f, 20, 9);
    
    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(1.0f, -9.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(1.0f, -9.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/tilemap-player.png");
    
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
    m_state.enemies[0].set_position(glm::vec3(16.0f, -9.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(16.0f, -9.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_speed(2.5f);
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

    //WASP
    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(WASP);
    m_state.enemies[1].set_ai_state(FLY);
    m_state.enemies[1].m_texture_id = wasp_texture_id;
    m_state.enemies[1].set_position(glm::vec3(8.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_init_pos(glm::vec3(8.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[1].set_speed(1.5f);
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_state.enemies[1].m_walking[m_state.enemies[1].IDLE] = new int[3] { 24, 25, 26 };
    
    m_state.enemies[1].m_animation_indices = m_state.enemies[1].m_walking[m_state.enemies[1].IDLE];
    m_state.enemies[1].m_animation_frames = 3;
    m_state.enemies[1].m_animation_index = 0;
    m_state.enemies[1].m_animation_time = 0.0f;
    m_state.enemies[1].m_animation_cols = 9;
    m_state.enemies[1].m_animation_rows = 3;
    m_state.enemies[1].set_height(0.8f);
    m_state.enemies[1].set_width(0.8f);
    
    m_state.enemies[2].set_entity_type(ENEMY);
    m_state.enemies[2].set_ai_type(WASP);
    m_state.enemies[2].set_ai_state(CIRCLE);
    m_state.enemies[2].m_texture_id = wasp_texture_id;
    m_state.enemies[2].set_position(glm::vec3(19.0f, -3.5f, 0.0f));
    m_state.enemies[2].set_init_pos(glm::vec3(19.0f, -3.5f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_state.enemies[2].set_speed(1.5f);
    m_state.enemies[2].set_radius(3.0f);
    m_state.enemies[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_state.enemies[2].m_walking[m_state.enemies[2].IDLE] = new int[3] { 24, 25, 26 };
    
    m_state.enemies[2].m_animation_indices = m_state.enemies[2].m_walking[m_state.enemies[2].IDLE];
    m_state.enemies[2].m_animation_frames = 3;
    m_state.enemies[2].m_animation_index = 0;
    m_state.enemies[2].m_animation_time = 0.0f;
    m_state.enemies[2].m_animation_cols = 9;
    m_state.enemies[2].m_animation_rows = 3;
    m_state.enemies[2].set_height(0.8f);
    m_state.enemies[2].set_width(0.8f);
    
    
    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump1.wav");
    Mix_VolumeChunk(m_state.jump_sfx, MIX_MAX_VOLUME / 32.0f);

    m_state.pop_sfx = Mix_LoadWAV("assets/audio/bubble1.wav");
    Mix_VolumeChunk(m_state.pop_sfx, MIX_MAX_VOLUME / 32.0f);
}

void LevelB::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.pop_sfx);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.pop_sfx);
    }
}


void LevelB::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
