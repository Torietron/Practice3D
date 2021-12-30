/****** SceneControl.h *****/
#ifndef DEF_SCENECONTROL_H
#define DEF_SCENECONTROL_H

#include <cstdint>
#include "SceneVirt.h"
class SceneControl {
    public:
        SceneControl(uint_least8_t startup = 1);
        void Change(uint_least8_t nextscene);
        int Init();
        int Load();
        int End();
        int Update();
        int Draw();
    private:  
        SceneVirt *sPtr;
        uint_least8_t lScene;
        uint_least8_t nScene;
};

typedef enum {
    NO_SCENE,
    DEBUG_SCENE,
    PRACTICE_SCENE
} Scenes_t;

#endif