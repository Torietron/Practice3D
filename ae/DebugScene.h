/***** DebugScene.h *****/
#ifndef DEF_DEBUGSCENE_H
#define DEF_DEBUGSCENE_H

#include "SceneVirt.h"
class DebugScene: public SceneVirt {
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif