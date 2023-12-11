/**
* Author: Raymond Lin
* Assignment: Knight's Descent
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "BossRoom.h"
#include "../Utility.h"

#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 16
#define ENEMY_COUNT 1

unsigned int BOSS_ROOM_DATA[] =
{
    147,133,134,134,134,134,134,134,134,134,134,134,134,134,135,145,
    147,145,172,172,172,172,172,172,172,172,172,172,172,172,147,145,
    147,145,182,183,183,182,182,183,182,183,183,183,183,182,147,145,
    147,145,180,180,180,181,180,180,181,181,180,181,181,180,147,145,
    147,145,180,180,181,181,181,180,181,181,180,181,180,181,147,145,
    147,145,180,181,180,173,180,180,181,180,173,181,180,180,147,145,
    147,145,180,181,180,181,180,180,180,180,180,181,181,180,147,145,
    147,145,180,181,181,181,180,180,180,180,181,181,180,181,147,145,
    147,145,180,181,180,181,180,181,181,180,180,181,180,181,147,145,
    147,145,181,180,180,180,180,180,181,181,180,181,181,180,147,145,
    147,145,181,181,181,173,180,181,181,181,173,180,181,181,147,145,
    147,145,180,181,180,181,180,181,180,181,181,180,181,180,147,145,
    147,145,181,180,180,180,181,181,181,180,181,180,181,181,147,145,
    147,145,180,181,181,180,180,180,181,181,180,180,180,181,147,145,
    147,145,180,181,136,158,158,158,158,158,158,158,158,158,159,145,
    147,145,181,180,147,144,132,132,132,156,156,144,132,132,144,145



};

BossRoom::~BossRoom()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.sfx[SWORD_SFX]);
    Mix_FreeChunk(m_state.sfx[KILL_SFX]);
    Mix_FreeChunk(m_state.sfx[DEATH_SFX]);
    Mix_FreeChunk(m_state.sfx[GAMEOVER_SFX]);
    Mix_FreeChunk(m_state.sfx[WIN_SFX]);

    for (int i = 0; i < m_number_of_sfx; i++)
    {
        delete m_state.sfx[i];
    }

    delete[] m_state.sfx;
}

void BossRoom::initialise()
{
    
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, BOSS_ROOM_DATA, map_texture_id, 1.0f, 12, 22);
    
    GLuint shadow_texture_id = Utility::load_texture("assets/images/shadow.png");

    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(3.0f, -15.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(3.0f, -15.0f, 0.0f));
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

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->UP];
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 7;
    m_state.player->m_facing = m_state.player->DOWN;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

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
    GLuint spirit_texture_id = Utility::load_texture("assets/images/spirit_spritesheet.png");

    // SPIRIT
    // Existing
    m_state.enemies = new Entity();
    m_state.enemies->set_entity_type(ENEMY);
    m_state.enemies->set_ai_type(SPIRIT);
    m_state.enemies->set_ai_state(MOVE_TOWARDS);
    m_state.enemies->m_texture_id = spirit_texture_id;
    m_state.enemies->set_position(glm::vec3(7.5f, -7.5f, 0.0f));
    m_state.enemies->set_init_pos(glm::vec3(7.5f, -7.5f, 0.0f));
    m_state.enemies->set_scale(glm::vec3(3.0f, 3.0f, 0.0f));
    m_state.enemies->set_movement(glm::vec3(0.0f));
    m_state.enemies->set_speed(1.0f);
    m_state.enemies->set_hp(3);

    // Animation
    m_state.enemies->m_walking[m_state.enemies->IDLE] = new int[5] { 0, 1, 2, 3, 4 };
    m_state.enemies->m_walking[m_state.enemies->DOWN] = new int[5] { 5, 6, 7, 6, 7 };

    m_state.enemies->m_animation_indices = m_state.enemies->m_walking[m_state.enemies->IDLE];
    m_state.enemies->m_animation_frames = 5;
    m_state.enemies->m_animation_index = 0;
    m_state.enemies->m_animation_time = 0.0f;
    m_state.enemies->m_animation_cols = 5;
    m_state.enemies->m_animation_rows = 2;
    m_state.enemies->set_height(2.0f);
    m_state.enemies->set_width(2.0f);

    m_state.enemies->shadow = new Entity();
    m_state.enemies->shadow->set_entity_type(SHADOW);
    m_state.enemies->shadow->set_position(glm::vec3(m_state.enemies->get_position().x, m_state.enemies->get_position().y - 0.5f, 0.0f));
    m_state.enemies->shadow->set_movement(glm::vec3(0.0f));
    m_state.enemies->shadow->set_scale(glm::vec3(0.8f, 0.3f, 0.0f));
    m_state.enemies->shadow->m_texture_id = shadow_texture_id;
    
    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.sfx = new Mix_Chunk * [m_number_of_sfx];

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

}

void BossRoom::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.sfx);
    m_state.player->weapon->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.sfx);
    m_state.player->shadow->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);

    m_state.enemies->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);
    m_state.enemies->shadow->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.sfx);
}


void BossRoom::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->shadow->render(program);
    m_state.player->render(program);
    m_state.player->weapon->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies->shadow->render(program);
        m_state.enemies->render(program);
    }
}
