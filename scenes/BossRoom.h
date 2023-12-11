#include "../Scene.h"

class BossRoom : public Scene {
public:    
    // ————— CONSTRUCTOR ————— //
    ~BossRoom();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
