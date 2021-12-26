/****** SceneControl.h *****/
#ifndef DEF_SCENECONTROL_H
#define DEF_SCENECONTROL_H

#include <cstdint>
#include "SceneVirt.h"
class SceneControl
{
    public:
        SceneControl(uint_fast8_t startup = 1);
        void Change(uint_fast8_t nextscene);
        int Init();
        int End();
        int Update();
        int Draw();
    private:  
        SceneVirt *sPtr;
        uint_fast8_t lScene;
        uint_fast8_t nScene;
};

typedef enum {
    NO_SCENE,
    DEBUG_SCENE,
    PRACTICE_SCENE
} scenes_t;

#endif