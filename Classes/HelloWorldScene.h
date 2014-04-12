#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

#include "Quaternion.hpp"
#include "Vector.hpp"

using namespace std;
using namespace cocos2d;

struct Vertex {
    vec3 Position;
    vec4 Color;
};

struct MyAnimation {
    Quaternion Start;
    Quaternion End;
    Quaternion Current;
    float Elapsed;
    float Duration;
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initCone();
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    //we call our actual opengl commands here
    void onDraw();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    kmMat4 _modelViewMV;
    CustomCommand _customCommand;
    
    GLProgram *mShaderProgram;
    GLint mColorLocation;
    GLuint _textureID;
    Texture2D *_texture;
    
    vector<Vertex> m_cone;
    vector<Vertex> m_disk;
    MyAnimation m_animation;
};

#endif // __HELLOWORLD_SCENE_H__
