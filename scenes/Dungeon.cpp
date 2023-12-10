/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Dungeon.h"
#include "../Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 32
#define ENEMY_COUNT 6

unsigned int DUNGEON_DATA[] =
{
    144,133,134,134,135,132,145,181,181,180,180,180,180,147,145,181,180,181,181,181,
    132,145,142,143,147,132,145,180,180,223,223,180,181,147,145,181,201,202,202,202,
    156,145,183,182,147,144,145,195,180,180,181,180,180,147,145,180,187,136,158,158,
    132,145,180,174,147,132,157,158,158,158,158,158,158,159,145,180,200,147,132,156,
    132,145,180,181,148,138,134,134,134,134,134,134,134,138,149,181,180,147,132,132,
    156,145,174,180,189,150,172,172,125,178,179,125,172,150,191,208,208,147,132,132,
    132,145,181,181,180,162,183,173,183,182,182,183,185,162,181,181,180,147,144,132,
    132,145,181,173,180,180,180,180,180,180,174,181,181,173,174,181,181,147,132,132,
    132,145,181,180,181,174,181,173,180,181,181,180,180,180,180,181,181,147,156,132,
    144,157,158,158,158,158,158,158,158,158,158,158,158,158,137,181,174,147,132,132,
    132,144,156,132,156,132,156,133,134,134,134,138,134,134,149,181,180,147,132,132,
    132,132,132,132,132,144,132,145,172,152,172,150,172,152,191,180,173,147,156,132,
    132,132,133,134,138,134,138,149,182,164,183,162,182,164,185,174,181,147,132,132,
    132,132,145,172,150,172,150,191,181,181,180,181,181,181,181,181,181,147,132,144,
    132,132,145,182,162,183,162,173,180,180,180,180,174,180,180,180,181,147,132,132,
    132,144,145,174,180,181,174,181,180,174,180,180,180,180,181,174,181,147,132,156,
    144,132,145,180,180,180,180,173,181,181,181,180,181,180,180,136,158,159,132,132,
    132,132,145,174,180,181,136,137,180,180,180,180,180,180,180,147,144,132,132,132,
    132,132,145,181,181,181,147,157,158,158,158,158,158,158,158,159,132,132,132,132,
    144,132,145,181,174,180,147,144,132,133,134,134,134,134,134,135,132,132,132,132,
    132,132,145,180,180,181,147,132,144,145,172,172,172,172,172,147,132,156,132,132,
    132,132,145,174,180,181,147,132,132,145,181,224,180,181,181,147,132,132,132,132,
    132,156,145,181,181,174,147,156,132,145,180,181,180,195,180,147,132,132,144,132,
    132,132,145,180,181,180,147,132,156,157,158,158,158,158,158,159,132,144,132,132,
    156,132,145,181,181,174,148,138,134,138,134,134,134,138,134,138,134,135,132,132,
    132,132,145,180,181,181,189,150,172,150,172,177,172,150,172,150,172,147,132,132,
    132,132,145,181,181,181,182,162,182,162,182,182,182,162,182,162,185,147,132,132,
    132,156,145,181,173,181,174,181,181,181,181,180,180,174,174,181,181,147,132,132,
    132,132,145,181,180,181,181,174,180,180,181,180,181,180,181,181,180,147,156,132,
    156,132,157,158,158,158,158,158,158,158,158,158,158,158,137,180,174,147,156,132,
    134,134,134,134,134,134,134,134,134,134,134,134,134,134,149,174,180,148,134,134,
    172,172,172,172,172,172,172,172,172,172,172,172,172,172,191,181,174,189,172,172



};

Dungeon::~Dungeon()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.sfx[SWORD_SFX]);
    Mix_FreeChunk(m_state.sfx[KILL_SFX]);

    for (int i = 0; i < m_number_of_sfx; i++)
    {
        delete m_state.sfx[i];
    }

    delete[] m_state.sfx;
}

void Dungeon::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, DUNGEON_DATA, map_texture_id, 1.0f, 12, 22);

    GLuint shadow_texture_id = Utility::load_texture("assets/images/shadow.png");
    
    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(15.0f, -31.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(15.0f, -31.0f, 0.0f));
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
    GLuint snake_texture_id = Utility::load_texture("assets/images/snake_spritesheet.png");
    GLuint reptile_texture_id = Utility::load_texture("assets/images/reptile_spritesheet.png");

    m_state.enemies = new Entity[ENEMY_COUNT];

    // SNAKES
    for (int i = 0; i < 2; i++)
    {
        // Existing
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_ai_type(SNAKE);
        m_state.enemies[i].m_texture_id = snake_texture_id;
        m_state.enemies[i].set_speed(2.0f);

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
    m_state.enemies[0].set_position(glm::vec3(11.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(11.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].LEFT];

    // SNAKE 2
    m_state.enemies[1].set_ai_state(ROAM_VERTICAL);
    m_state.enemies[1].set_position(glm::vec3(4.0f, -21.0f, 0.0f));
    m_state.enemies[1].set_init_pos(glm::vec3(4.0f, -21.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));

    m_state.enemies[1].m_animation_indices = m_state.enemies[1].m_walking[m_state.enemies[1].DOWN];

    // REPTILES
    for (int i = 2; i < ENEMY_COUNT; i++)
    {
        // Existing
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_ai_type(REPTILE);
        m_state.enemies[i].set_ai_state(NONE);
        m_state.enemies[i].m_texture_id = reptile_texture_id;
        m_state.enemies[i].set_speed(0.5f);

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
    // REPTILE 1
    m_state.enemies[2].set_position(glm::vec3(6.0f, -27.0f, 0.0f));
    m_state.enemies[2].set_init_pos(glm::vec3(6.0f, -27.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));

    m_state.enemies[2].m_animation_indices = m_state.enemies[2].m_walking[m_state.enemies[2].RIGHT];

    // REPTILE 2
    m_state.enemies[3].set_position(glm::vec3(14.0f, -13.0f, 0.0f));
    m_state.enemies[3].set_init_pos(glm::vec3(14.0f, -13.0f, 0.0f));
    m_state.enemies[3].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));

    m_state.enemies[3].m_animation_indices = m_state.enemies[3].m_walking[m_state.enemies[3].LEFT];

    // REPTILE 3
    m_state.enemies[4].set_position(glm::vec3(14.0f, -15.0f, 0.0f));
    m_state.enemies[4].set_init_pos(glm::vec3(14.0f, -15.0f, 0.0f));
    m_state.enemies[4].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));

    m_state.enemies[4].m_animation_indices = m_state.enemies[4].m_walking[m_state.enemies[4].LEFT];

    // REPTILE 4
    m_state.enemies[5].set_position(glm::vec3(6.0f, -7.0f, 0.0f));
    m_state.enemies[5].set_init_pos(glm::vec3(6.0f, -7.0f, 0.0f));
    m_state.enemies[5].set_movement(glm::vec3(0.0f, 0.0f, 0.0f));

    m_state.enemies[5].m_animation_indices = m_state.enemies[5].m_walking[m_state.enemies[5].RIGHT];

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

    m_state.sfx = new Mix_Chunk * [m_number_of_sfx];

    m_state.sfx[SWORD_SFX] = Mix_LoadWAV("assets/audio/Sword.wav");
    Mix_VolumeChunk(m_state.sfx[SWORD_SFX], MIX_MAX_VOLUME / 128.0f);

    m_state.sfx[KILL_SFX] = Mix_LoadWAV("assets/audio/Kill.wav");
    Mix_VolumeChunk(m_state.sfx[KILL_SFX], MIX_MAX_VOLUME / 64.0f);

    m_state.sfx[DEATH_SFX] = Mix_LoadWAV("assets/audio/DEATH.wav");
    Mix_VolumeChunk(m_state.sfx[DEATH_SFX], MIX_MAX_VOLUME / 16.0f);
}

void Dungeon::update(float delta_time)
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

void Dungeon::render(ShaderProgram *program)
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
