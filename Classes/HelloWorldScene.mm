#include "HelloWorldScene.h"
#include "ParametricEquations.hpp"
#include "ObjSurface.hpp"
#include "RootViewController.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}




// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();
    
//    auto label = Label::create("Hello Triangle!", "Arial", 40);
//    label->setPosition(cocos2d::Point(winSize.width/2, winSize.height/2));
//    this->addChild(label);

    Sprite* tiger = Sprite::create("Tiger565.pvr");
    tiger->setPosition(cocos2d::Point(200,200));
    this->addChild(tiger);
    
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
   CHECK_GL_ERROR_DEBUG();
    
    return true;
}


void HelloWorld::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    
    //call my own opengl commands
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void HelloWorld::onDraw()
{
    glClearColor(1, 1, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //0. prepare stencil test
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 0xff, 0xfff);
    
    //step 1:  render the disk in stencil buffer
    glDisable(GL_TEXTURE_2D);
//    glTranslatef(0, DiskY, 0);
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    
//    RenderDrawable(m_drawables.Disk); // private method that calls glDrawElements
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
//    glTranslatef(0, -DiskY, 0);
    glEnable(GL_TEXTURE_2D);
    
    
    //step 2: clipping the reflection into the disk
    glTranslatef(0, KnotY, 0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0xff, 0xff);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, m_textures.Grille);
    const float alpha = 0.4f;
    vec4 diffuse(alpha, alpha, alpha, 1 - alpha);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.Pointer());
    glMatrixMode(GL_PROJECTION);

    glLoadMatrixf(m_mirrorProjection.Pointer());
    RenderDrawable(m_drawables.Knot); // private method that calls glDrawElements glLoadMatrixf(m_projection.Pointer());
    
    glMatrixMode(GL_MODELVIEW);
    
}



bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
//    [(RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController  takePhoto];

   
    
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void HelloWorld::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
//    Image *im = new Image;
//    im->initWithImageFile("");

}

