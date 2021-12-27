/***** NoScene.h *****/
#ifndef DEF_NOSCENE_H
#define DEF_NOSCENE_H

#include "SceneVirt.h"
class NoScene: public SceneVirt {
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif