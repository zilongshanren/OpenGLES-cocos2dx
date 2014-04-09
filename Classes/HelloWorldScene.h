#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

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
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    kmMat4 _modelViewMV;
    CustomCommand _customCommand;
    
    GLProgram *mShaderProgram;
    GLint _colorLocation;
    GLint _positionLocation;
    GLint _textureLocation;
    
    
    GLuint _projectionUniform;
    GLuint _modelViewUniform;
    GLuint _textureUniform;
    
    
    GLuint _textureID;
    GLuint _textureID2;
    Texture2D *_texture;
};

#endif // __HELLOWORLD_SCENE_H__
