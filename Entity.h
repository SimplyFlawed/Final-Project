#pragma once
#include "Map.h"

enum EntityType { PLAYER, ENEMY, WEAPON, LOGO, SHADOW };
enum AIType { SNAKE, REPTILE, SPIRIT };
enum AIState { ROAM_HORIZONTAL, ROAM_VERTICAL, RUSH, WALK, REST, MOVE_AWAY, MOVE_TOWARDS };

class Entity
{
private:
    bool m_is_active = true;

    // ––––– ANIMATION ––––– //
    int* m_animation_right = NULL, // move to the right
        * m_animation_left = NULL, // move to the left
        * m_animation_up = NULL, // move upwards
        * m_animation_down = NULL, // move downwards
        * m_animation_idle = NULL, // move downwards
        * m_attack_left = NULL,
        * m_attack_right = NULL,
        * m_attack_up = NULL,
        * m_attack_down = NULL;

    // ––––– PHYSICS (GRAVITY) ––––– //
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    // ————— TRANSFORMATIONS ————— //
    float     m_speed;
    glm::vec3 m_init_pos;
    glm::vec3 m_movement;
    glm::mat4 m_model_matrix;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    float     m_angle,
              m_radius;


    // ————— ENEMY AI ————— //
    EntityType m_entity_type;
    AIType     m_ai_type;
    AIState    m_ai_state;
    float      m_agro_time = 0.0f;

    float m_width = 1;
    float m_height = 1;


public:
    // ————— STATIC VARIABLES ————— //
    static const int    SECONDS_PER_FRAME = 6;
    static const int    LEFT = 0,
                        RIGHT = 1,
                        UP = 2,
                        DOWN = 3,
                        IDLE = 4;

    // ————— ANIMATION ————— //
    int** m_walking = new int* [5]
        {
            m_animation_left,
            m_animation_right,
            m_animation_up,
            m_animation_down,
            m_animation_idle
        };

    int m_animation_frames = 0,
        m_animation_index = 0,
        m_animation_cols = 0,
        m_animation_rows = 0;

    int* m_animation_indices = NULL;
    float   m_animation_time = 0.0f;

    int m_facing;

    // ––––– HEALTH ––––– //
    int m_hp;
    float m_damage_cooldown= 0.0f;

    // ––––– ATTACKING ––––– //
    int** m_attack = new int* [4]
        {
            m_attack_left,
            m_attack_right,
            m_attack_up,
            m_attack_down
        };

    bool m_is_attacking = false;

    Entity* weapon;

    // ––––– SHADOW ––––– //
    Entity* shadow;

    // ––––– PHYSICS (JUMPING) ––––– //
    bool  m_is_jumping = false;
    float m_jumping_power = 0;

    // ––––– PHYSICS (COLLISIONS) ––––– //
    bool m_collided_top = false;
    bool m_collided_bottom = false;
    bool m_collided_left = false;
    bool m_collided_right = false;

    GLuint    m_texture_id;

    bool m_player_dead = false;
    bool m_player_win  = false;
    bool m_next_level  = false;

    // ————— METHODS ————— //
    Entity();
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map, Mix_Chunk** sfx);
    void render(ShaderProgram* program);

    bool const check_collision(Entity* other) const;
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);

    // Overloading our methods to check for only the map
    void const check_collision_y(Map* map);
    void const check_collision_x(Map* map);

    void move_left() { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f; };
    void move_up() { m_movement.y = 1.0f; };
    void move_down() { m_movement.y = -1.0f; };

    void ai_activate(Entity* player, float delta_time);
    void ai_snake();
    void ai_reptile(Entity* player, float delta_time);
    void ai_spirit(Entity* player, float delta_time);

    void activate() { m_is_active = true; };
    void deactivate() { m_is_active = false; };

    // ————— GETTERS ————— //
    EntityType const get_entity_type()    const { return m_entity_type; };
    AIType     const get_ai_type()        const { return m_ai_type; };
    AIState    const get_ai_state()       const { return m_ai_state; };
    glm::vec3  const get_position()       const { return m_position; };
    glm::vec3  const get_movement()       const { return m_movement; };
    glm::vec3  const get_velocity()       const { return m_velocity; };
    glm::vec3  const get_acceleration()   const { return m_acceleration; };
    glm::vec3  const get_rotation()       const { return m_rotation; };
    glm::vec3  const get_scale()          const { return m_scale; };
    float      const get_jumping_power()  const { return m_jumping_power; };
    float      const get_speed()          const { return m_speed; };
    int        const get_width()          const { return m_width; };
    int        const get_height()         const { return m_height; };
    int        const get_hp()             const { return m_hp; };
    glm::mat4  const get_model_matrix()   const { return m_model_matrix; };

    // ————— SETTERS ————— //
    void const set_entity_type(EntityType new_entity_type) { m_entity_type = new_entity_type; };
    void const set_ai_type(AIType new_ai_type) { m_ai_type = new_ai_type; };
    void const set_ai_state(AIState new_state) { m_ai_state = new_state; };
    void const set_position(glm::vec3 new_position) { m_position = new_position; };
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; };
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; };
    void const set_speed(float new_speed) { m_speed = new_speed; };
    void const set_jumping_power(float new_jumping_power) { m_jumping_power = new_jumping_power; };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; };
    void const set_width(float new_width) { m_width = new_width; };
    void const set_height(float new_height) { m_height = new_height; };
    void const set_scale(glm::vec3 new_scale) { m_scale = new_scale; };
    void const set_rotation(glm::vec3 new_rotation) { m_rotation = new_rotation; };
    void const set_angle(float new_angle) { m_angle = new_angle; };
    void const set_radius(float new_radius) { m_radius = new_radius; };
    void const set_init_pos(glm::vec3 new_position) { m_init_pos = new_position; };
    void const set_model_matrix(glm::mat4 new_matrix) { m_model_matrix = new_matrix; };
    void const set_hp(int new_hp) { m_hp = new_hp; };
};
