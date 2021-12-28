/***** NoScene.h *****/
#ifndef DEF_PRACTICESCENE_H
#define DEF_PRACTICESCENE_H

#include "SceneVirt.h"
class PracticeScene: public SceneVirt {
    public:
        void Init();
        void Load();
        void End();
        void Update();
        void Draw();
};

#endif