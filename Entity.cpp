/**
* Author: Raymond Lin
* Assignment: Knight's Descent
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

Entity::Entity()
{
    // ––––– PHYSICS ––––– //
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);
    m_rotation = glm::vec3(0.0f);

    // ––––– TRANSLATION ––––– //
    m_movement = glm::vec3(0.0f);
    m_scale = glm::vec3(1.0f, 1.0f, 0.0f);
    m_speed = 0;
    m_model_matrix = glm::mat4(1.0f);
    m_angle = 0.0f;
    m_radius = 0;
}

Entity::~Entity()
{
    delete[] m_animation_up;
    delete[] m_animation_down;
    delete[] m_animation_left;
    delete[] m_animation_right;
    delete[] m_walking;
    delete[] m_animation_idle;
    delete weapon;
    delete shadow;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::ai_activate(Entity* player, float delta_time)
{
    switch (m_ai_type)
    {
    case SNAKE:
        ai_snake();
        break;

    case REPTILE:
        ai_reptile(player, delta_time);
        break;

    case SPIRIT:
        ai_spirit(player, delta_time);
        break;

    default:
        break;
    }
}

void Entity::ai_snake()
{
    switch (m_ai_state) {
    case ROAM_HORIZONTAL:
        if (m_position.x < m_init_pos.x - 1.9f)
        {
            m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
            m_animation_indices = m_walking[RIGHT];
        }
        if (m_position.x > m_init_pos.x + 1.9f)
        {
            m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
            m_animation_indices = m_walking[LEFT];
        }
        break;

    case ROAM_VERTICAL:
        if (m_position.y > m_init_pos.y + 2.5f) 
        {
            m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
            m_animation_indices = m_walking[DOWN];
        }
        if (m_position.y < m_init_pos.y - 2.5f) 
        {
            m_movement = glm::vec3(0.0f, 1.0f, 0.0f);
            m_animation_indices = m_walking[UP];
        }
        break;

    default:
        break;
    } 
}

void Entity::ai_reptile(Entity* player, float delta_time)
{
    float x_distance = fabs(m_position.x - player->get_position().x);
    float y_distance = fabs(m_position.y - player->get_position().y);

    if (x_distance < 5.0f && y_distance < 5.0f) 
    { 
        m_agro_time += delta_time;

        if (m_agro_time >= 5.0f) { m_agro_time = 0.0f; }
        else if (m_agro_time >= 3.0f) {  set_ai_state(STAND); }
        else if (m_agro_time >= 1.0f) { set_ai_state(RUSH); }
        else { set_ai_state(WALK); }
    }
    else 
    { 
        set_ai_state(STAND);
        m_agro_time = 0.0f;
    }

    switch (m_ai_state) {
    case RUSH:
        set_speed(4.0f);

        if (x_distance > y_distance)
        {
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
                m_animation_indices = m_walking[LEFT];
            }
            else
            {
                m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
                m_animation_indices = m_walking[RIGHT];
            }
        }
        else
        {
            if (m_position.y > player->get_position().y) {
                m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
                m_animation_indices = m_walking[DOWN];
            }
            else
            {
                m_movement = glm::vec3(0.0f, 1.0f, 0.0f);
                m_animation_indices = m_walking[UP];
            }
        }
        
        break;

    case WALK:
        set_speed(0.5f);
        if (x_distance > y_distance)
        {
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
                m_animation_indices = m_walking[LEFT];
            }
            else
            {
                m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
                m_animation_indices = m_walking[RIGHT];
            }
        }
        else
        {
            if (m_position.y > player->get_position().y) {
                m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
                m_animation_indices = m_walking[DOWN];
            }
            else
            {
                m_movement = glm::vec3(0.0f, 1.0f, 0.0f);
                m_animation_indices = m_walking[UP];
            }
        }
        break;

    case STAND:
        m_movement = glm::vec3(0.0f);
        break;

    default:
        break;
    }
}

void Entity::ai_spirit(Entity* player, float delta_time)
{
    float x_distance = fabs(m_position.x - player->get_position().x);
    float y_distance = fabs(m_position.y - player->get_position().y);

    m_agro_time += delta_time;

    if (m_agro_time >= 8.0f) { m_agro_time = 0.0f; }
    else if (m_agro_time >= 4.0f) { set_ai_state(MOVE_AWAY); }
    else { set_ai_state(MOVE_TOWARDS); }

    switch (m_ai_state)
    {
    case MOVE_AWAY:
        set_speed(m_hp < 2 ? 2.5f : 1.5f);

        if (x_distance < y_distance)
        {
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
            }
            else
            {
                m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            if (m_position.y > player->get_position().y) {
                m_movement = glm::vec3(0.0f, 1.0f, 0.0f);
            }
            else
            {
                m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
            }
        }
        break;

    case MOVE_TOWARDS:
        set_speed(m_hp < 2 ? 2.0f : 1.0f);

        if (x_distance > y_distance)
        {
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
            }
            else
            {
                m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            if (m_position.y > player->get_position().y) {
                m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
            }
            else
            {
                m_movement = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }
        break;

    default:
        break;
    }
}

void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map, Mix_Chunk** sfx)
{
    if (!m_is_active) return;

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    m_angle += m_speed * delta_time;

    if (m_entity_type == ENEMY) ai_activate(player, delta_time);

    if (m_animation_indices != NULL)
    {
        if (glm::length(m_movement) != 0 || get_entity_type() == WEAPON || get_ai_type() == SPIRIT)
        {
            m_animation_time += delta_time;
            float frames_per_second = (float)1 / SECONDS_PER_FRAME;

            if (m_animation_time >= frames_per_second)
            {
                m_animation_time = 0.0f;
                m_animation_index++;

                if (m_animation_index >= m_animation_frames)
                {
                    if (get_entity_type() == WEAPON)
                    {
                        player->weapon->deactivate();
                        player->m_animation_indices = player->m_walking[player->m_facing];
                    }
                    if (get_ai_type() == SPIRIT) m_animation_indices = m_walking[IDLE];
                    m_animation_index = 0;
                }
            }
        }
    }

    // PLAYER ATTACK DIRECTION
    if (m_is_attacking) {
        m_is_attacking = false;

        switch (player->m_facing)
        {
        case DOWN:
            player->weapon->set_position(glm::vec3(m_position.x, m_position.y - 1.0f, 0.0f));
            break;

        case UP:
            player->weapon->set_position(glm::vec3(m_position.x, m_position.y + 1.0f, 0.0f));
            break;

        case LEFT:
            player->weapon->set_position(glm::vec3(m_position.x - 1.0f, m_position.y, 0.0f));
            break;

        case RIGHT:
            player->weapon->set_position(glm::vec3(m_position.x + 1.0f, m_position.y, 0.0f));
            break;

        default:
            break;
        }
        player->weapon->activate();
        player->weapon->m_animation_indices = weapon->m_attack[player->m_facing];
        player->weapon->m_animation_index = 0;
        Mix_PlayChannel(-1, sfx[0], 0);
    }

    // SWORD COLLISION
        for (int i = 0; i < object_count; i++)
        {
            objects[i].m_damage_cooldown += delta_time;
            if (player->weapon->check_collision(&objects[i]) && objects[i].get_entity_type() == ENEMY)
            {
                if (objects[i].m_damage_cooldown >= 1.0f)
                { 
                    objects[i].m_damage_cooldown = 0.0f;
                    objects[i].m_hp--;
                    objects[i].m_animation_indices = objects[i].m_walking[DOWN];
                    objects[i].m_animation_index = 0;
                    Mix_PlayChannel(-1, sfx[1], 0);
                }
                if (objects[i].m_hp < 1)
                {
                    objects[i].deactivate();
                    objects[i].shadow->deactivate();
                    if (objects[i].get_ai_type() == SPIRIT) m_player_win = true;
                }
            }
        }

    // ENTITY SHADOW
    if (get_entity_type() == PLAYER || get_entity_type() == ENEMY) {
        this->shadow->set_position(glm::vec3(this->get_position().x, this->get_position().y - 0.5f, 0.0f));
    }

    if (m_position.y < -map->get_height()) m_player_dead = true;

    m_velocity = m_movement * m_speed;
    //m_velocity += m_acceleration * delta_time;

    // We make two calls to our check_collision methods, one for the collidable objects and one for
    // the map.
    m_position.y += m_velocity.y * delta_time;
    if (get_entity_type() != WEAPON) check_collision_y(objects, object_count);
    check_collision_y(map);

    m_position.x += m_velocity.x * delta_time;
    if (get_entity_type() != WEAPON) check_collision_x(objects, object_count);
    check_collision_x(map);

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position) * glm::scale(m_model_matrix, m_scale);
}

void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float y_distance = fabs(m_position.y - collidable_entity->get_position().y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->get_height() / 2.0f));
            if (m_velocity.y >= 0) {
                m_position.y -= y_overlap;
                m_velocity.y = 0;
                m_collided_top = true;

                if (collidable_entity->get_entity_type() == ENEMY) m_player_dead = true;
                
            }
            else if (m_velocity.y <= 0) {
                m_position.y += y_overlap;
                m_velocity.y = 0;
                m_collided_bottom = true;

                if (collidable_entity->get_entity_type() == ENEMY) m_player_dead = true;

            }
        }

    }
}

void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float x_distance = fabs(m_position.x - collidable_entity->get_position().x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->get_width() / 2.0f));
            if (m_velocity.x >= 0) {
                m_position.x -= x_overlap;
                m_velocity.x = 0;
                m_collided_right = true;

                if (collidable_entity->get_entity_type() == ENEMY) m_player_dead = true;
            }
            else if (m_velocity.x <= 0) {
                m_position.x += x_overlap;
                m_velocity.x = 0;
                m_collided_left = true;

                if (collidable_entity->get_entity_type() == ENEMY) m_player_dead = true;
            }
        }
    }
}


void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;

    }

    if (m_collided_top && (map->check_tile(top) == 111 || map->check_tile(top) == 112)) m_next_level = true;
    if (m_collided_top && (map->check_tile(top) == 142 || map->check_tile(top) == 143)) m_next_level = true;

    //if ((m_collided_top || m_collided_bottom) && (map->check_tile(top) == 173 || map->check_tile(bottom) == 173)) m_player_dead = true;
    if (map->check_tile(bottom) == 173) m_player_dead = true;
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }

    if (map->check_tile(right) == 173 || map->check_tile(left) == 173) m_player_dead = true;
}



void Entity::render(ShaderProgram* program)
{
    if (!m_is_active) return;

    program->set_model_matrix(m_model_matrix);

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}


bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    // If either entity is inactive, there shouldn't be any collision
    if (!m_is_active || !other->m_is_active) return false;

    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}
