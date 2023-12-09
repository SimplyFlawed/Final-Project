/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelC.h"
#include "../Utility.h"

#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 30
#define ENEMY_COUNT 2

unsigned int LEVEL_C_DATA[] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,37,38,38,
    0,111,0,0,0,0,0,0,0,0,0,0,37,38,38,
    0,131,0,0,0,0,0,0,0,0,0,0,57,58,97,
    153,154,155,0,0,0,0,0,0,0,0,0,0,0,138,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,
    0,0,0,0,0,0,0,156,0,0,0,0,0,0,116,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,
    0,0,0,0,0,0,0,0,0,0,0,98,119,99,136,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,
    0,0,0,0,0,0,0,153,154,154,155,0,0,0,96,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,
    0,156,0,0,0,0,0,0,0,0,0,0,0,0,116,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,138,
    0,0,153,154,155,0,0,0,0,0,0,0,0,0,116,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,
    0,0,0,0,0,0,153,154,155,0,0,0,0,0,116,
    0,0,0,0,0,0,0,0,0,0,0,0,0,125,137,
    0,0,0,0,0,0,0,0,0,0,0,21,22,22,22,
    0,0,0,0,0,0,0,0,0,0,0,121,122,122,122,
    0,0,0,0,0,0,0,0,0,0,0,121,122,122,122,
    0,0,0,0,0,0,0,0,0,0,26,121,122,122,122,
    0,0,0,0,0,0,0,0,0,21,22,121,122,122,122,
    0,0,0,0,0,0,0,0,0,121,122,121,122,122,122,
    0,0,0,124,0,0,128,0,0,121,122,121,122,21,22,
    22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
    122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,
    122,122,122,122,122,122,122,122,21,22,23,122,122,122,122,
    122,122,122,122,122,122,122,122,121,122,123,122,122,122,122

};

LevelC::~LevelC()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.pop_sfx);
}

void LevelC::initialise()
{
    
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap_packed.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_C_DATA, map_texture_id, 1.0f, 20, 9);
    

    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(0.0f, -25.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(0.0f, -25.0f, 0.0f));
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
    //WASP
    GLuint wasp_texture_id = Utility::load_texture("assets/images/tilemap-characters_packed.png");

    m_state.enemies = new Entity[ENEMY_COUNT];

    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(WASP);
    m_state.enemies[0].set_ai_state(FLY);
    m_state.enemies[0].m_texture_id = wasp_texture_id;
    m_state.enemies[0].set_position(glm::vec3(4.0f, -12.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(4.0f, -12.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[0].set_speed(3.5f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_state.enemies[0].m_walking[m_state.enemies[0].IDLE] = new int[3] { 24, 25, 26 };
    
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].IDLE];
    m_state.enemies[0].m_animation_frames = 3;
    m_state.enemies[0].m_animation_index = 0;
    m_state.enemies[0].m_animation_time = 0.0f;
    m_state.enemies[0].m_animation_cols = 9;
    m_state.enemies[0].m_animation_rows = 3;
    m_state.enemies[0].set_height(0.8f);
    m_state.enemies[0].set_width(0.8f);
    
    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(WASP);
    m_state.enemies[1].set_ai_state(FLY);
    m_state.enemies[1].m_texture_id = wasp_texture_id;
    m_state.enemies[1].set_position(glm::vec3(4.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_init_pos(glm::vec3(4.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[1].set_speed(2.0f);
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
    
    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump1.wav");
    Mix_VolumeChunk(m_state.jump_sfx, MIX_MAX_VOLUME / 32.0f);

    m_state.pop_sfx = Mix_LoadWAV("assets/audio/bubble1.wav");
    Mix_VolumeChunk(m_state.pop_sfx, MIX_MAX_VOLUME / 32.0f);

}

void LevelC::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.pop_sfx);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.pop_sfx);
    }
}


void LevelC::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.enemies->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
