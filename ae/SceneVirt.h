/***** SceneVirt *****/
#ifndef DEF_SCENEVIRT_H
#define DEF_SCENEVIRT_H

class SceneVirt {  
    public:
        SceneVirt(){}
        virtual ~SceneVirt(){}
        virtual void Init(){}
        virtual void Load(){}
        virtual void End(){}
        virtual void Update() = 0;
        virtual void Draw() = 0;
};

#endif