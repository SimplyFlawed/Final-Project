#include "../Scene.h"

class Dungeon : public Scene {
public:    
    // ————— CONSTRUCTOR ————— //
    ~Dungeon();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
