/***** SceneVirt *****/
#ifndef DEF_SCENEVIRT_H
#define DEF_SCENEVIRT_H

class SceneVirt {  
    public:
        SceneVirt(){}
        virtual ~SceneVirt(){}
        virtual void Init(){}
        virtual void End(){}
        virtual void Update(){}
        virtual void Draw(){}
};

#endif