#pragma once

#include "Application.h"

class Game : public Engine::Application {
public:
    void createWindow() override;

    void init() override;

    void loop() override;

    void cleanup() override;

    bool shouldClose() override;
};
