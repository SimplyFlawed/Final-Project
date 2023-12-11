/**
* Author: Raymond Lin
* Assignment: Knight's Descent
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "StartScreen.h"
#include "../Utility.h"

GLuint text_texture_id;

StartScreen::~StartScreen()
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

void StartScreen::initialise()
{
    text_texture_id = Utility::load_texture("assets/fonts/font1.png");

    m_state.enemies = new Entity();

    m_state.enemies->set_entity_type(LOGO);
    m_state.enemies->m_texture_id = Utility::load_texture("assets/images/logo.png");
    m_state.enemies->set_model_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.75f, 0.0f)));
    m_state.enemies->set_model_matrix(glm::scale(m_state.enemies->get_model_matrix(), glm::vec3(8.0f, 8.0f, 0.0f)));
}

void StartScreen::update(float delta_time)
{

}


void StartScreen::render(ShaderProgram *program)
{
    m_state.enemies->render(program);
    Utility::draw_text(program, text_texture_id, "Press Enter to Start", 0.5f, 0.0f, glm::vec3(-4.75f, -2.25f, 0.0f));
    Utility::draw_text(program, text_texture_id, "Defeat the Spirit Boss to win", 0.4f, -0.2f, glm::vec3(-2.75f, -3.25f, 0.0f));
}
