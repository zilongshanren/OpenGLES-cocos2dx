#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Interfaces.hpp"

using namespace std;
using namespace cocos2d;

struct Vertex {
    vec3 Position;
    vec4 Color;
};

static const int SurfaceCount = 6;
static const int ButtonCount = SurfaceCount - 1;

struct TXAnimation {
    bool Active;
    float Elapsed;
    float Duration;
    Visual StartingVisuals[SurfaceCount];
    Visual EndingVisuals[SurfaceCount];
};

struct Drawable {
    GLuint VertexBuffer;
    GLuint TriangleIndexBuffer;
    GLuint LineIndexBuffer;
    int TriangleIndexCount;
    int LineIndexCount;
};



class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initialize();
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    //we call our actual opengl commands here
    void onDraw();
    void updateAnimation(float dt);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    kmMat4 _modelViewMV;
    CustomCommand _customCommand;
    
    GLProgram *mShaderProgram;
    GLint mColorLocation;
    GLint mPositionLocation;
    GLuint _textureID;
    Texture2D *_texture;
    
    
    //add wirefame
    void PopulateVisuals(Visual* visuals) const;
    int MapToButton(ivec2 touchpoint) const;
    vec3 MapToSphere(ivec2 touchpoint) const;
    
    
    float m_trackballRadius;
    ivec2 m_screenSize;
    ivec2 m_centerPoint;
    ivec2 m_fingerStart;
    bool m_spinning;
    Quaternion m_orientation;
    Quaternion m_previousOrientation;
    int m_currentSurface;
    ivec2 m_buttonSize;
    int m_pressedButton;
    int m_buttonSurfaces[ButtonCount];
    TXAnimation m_animation;
    vector<ISurface*> m_surfaces;
    
    
    vector<Drawable> m_drawables;
    GLuint m_colorRenderbuffer;
    GLuint m_depthRenderbuffer;
    mat4 m_translation;
};

#endif // __HELLOWORLD_SCENE_H__
