#include "HelloWorldScene.h"

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
    
    auto label = Label::create("Hello Triangle!", "Arial", 40);
    label->setPosition(Point(winSize.width/2, winSize.height/2));
    this->addChild(label);
    
    //方法1：使用有mvp的shader  注意坐标的变化
//    mShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);

    
    //方法2:使用系统内置的shader 无mvp
    mShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    //方法3：使用自己的shader
//    mShaderProgram = new GLProgram;
//    //如果使用下面的bindAttribLocation的话，那么position和color分别要命名为 a_position和a_color
//    mShaderProgram->initWithFilenames("myshader.vert", "myshader.frag");
//    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
//    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
//    
//    //don't forget to call link and updateUniforms.
//    mShaderProgram->link();
//    mShaderProgram->updateUniforms();

    //maybe I should call delete when the HelloWorldScene get destoryed

    
    mColorLocation = glGetUniformLocation( mShaderProgram->getProgram(), "a_color");
    
    //how to map texture HelloWorld.png to my triangles
    _textureID =  Director::getInstance()->getTextureCache()->addImage("HelloWorld.png")->getName();

    
    return true;
}

void HelloWorld::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    
    
    kmGLPushMatrix();
    kmGLGetMatrix(KM_GL_MODELVIEW, &_modelViewMV);
   
    
    //send custom command to tell the renderer to call opengl commands
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    renderer->addCommand(&_customCommand);
    
    
    kmGLPopMatrix();
}

void HelloWorld::onDraw()
{
    kmMat4 oldProj;
    kmMat4 oldMv;
//    //先获得旧的matrix，然后设置新的matrix
    kmGLGetMatrix(KM_GL_MODELVIEW, &oldMv);
    
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&_modelViewMV);

    
    kmGLGetMatrix(KM_GL_PROJECTION, &oldProj);
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    
    //call your own opengl commands here
    
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    //set color for each vertex  note:the color value is between 0-1

//    mShaderProgram->setUniformLocationWith4f(mColorLocation, 0.5, 0.5, 0.5, 1);
    
    
    int vertexCount = 6;
//    GLfloat    glVertices[] =
//    {
//        200.f, 300.0f,
//        300.f, 300.0f,
//        250.0f,400
//    };
    GLfloat    glVertices[] =
    {
        -1.f, -1.0f, 0,
        1.f, -1.0f, 0,
        1.0f,1.0f, 0,
        1.f, 1.0f, 0,
        -1.0f,1.0f, 0,
        -1.f, -1.0f, 0,
    };
    
    GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f};
    
    
    //note: opengl texture coordinate origin at top-left corner of the screen
    GLfloat textureCoord[] = {  0,1,
                                1,1,
                                1,0,
                                1,0,
                                0,0,
                                0,1};
    
//    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR
//                            | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
    //method 2:
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, glVertices);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, textureCoord);
    
    GL::bindTexture2D(_textureID);
    
    
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,vertexCount);
    
    CHECK_GL_ERROR_DEBUG();
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&oldProj);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&oldMv);

}


