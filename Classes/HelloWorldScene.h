#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Interfaces.hpp"

using namespace std;
using namespace cocos2d;



class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    //we call our actual opengl commands here
    void onDraw();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    CustomCommand _command;

};

#endif // __HELLOWORLD_SCENE_H__
