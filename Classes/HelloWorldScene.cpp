#include "HelloWorldScene.h"

USING_NS_CC;



GLuint vertexBuffer;
GLuint indexBuffer;


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
//    mShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    
    //方法3：使用自己的shader
    mShaderProgram = new GLProgram;
    //如果使用下面的bindAttribLocation的话，那么position和color分别要命名为 a_position和a_color
    mShaderProgram->initWithFilenames("myshader.vert", "myshader.frag");
    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    
    //don't forget to call link and updateUniforms.
    mShaderProgram->link();
    mShaderProgram->updateUniforms();

    //maybe I should call delete when the HelloWorldScene get destoryed

    
    _colorLocation = glGetUniformLocation( mShaderProgram->getProgram(), "SourceColor");
    
    //how to map texture HelloWorld.png to my triangles
    _textureID =  Director::getInstance()->getTextureCache()->addImage("HelloWorld.png")->getName();

    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
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
    
    //想显示3d cubic的时候，不能够把projection matrix设置为对等矩阵
//    kmGLLoadIdentity();
    
    kmGLMatrixMode(KM_GL_MODELVIEW);
    
    //mShaderProgram->use();
    //we don't need any builtin Uniforms, since we will add our mvp matrix all by hand
    
    typedef struct {
        float Position[3];
        float Color[4];
        float tex[2];
    } Vertex;
#define TEX_COORD_MAX   1

    Vertex Vertices[] = {
        // Front
        {{1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{-1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        {{-1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Back
        {{1, 1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{-1, -1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{1, -1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        {{-1, 1, -2}, {0, 0, 0, 1}, {0, 0}},
        // Left
        {{-1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{-1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}},
        // Right
        {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        {{1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Top
        {{1, 1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        {{-1, 1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Bottom
        {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        {{1, -1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        {{-1, -1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}}, 
        {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}}
    };
    int vertexCount = sizeof(Vertices) / sizeof(Vertices[0]);
    
    GLubyte Indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        4, 5, 7,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    };
    
       //set color for each vertex  note:the color value is between 0-1

//    mShaderProgram->setUniformLocationWith4f(mColorLocation, 0.5, 0.5, 0.5, 1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
    
    
    _positionLocation = glGetAttribLocation(mShaderProgram->getProgram(), "Position");
    _colorLocation = glGetAttribLocation(mShaderProgram->getProgram(), "SourceColor");
    _projectionUniform = glGetUniformLocation(mShaderProgram->getProgram(), "Projection");
    _modelViewUniform = glGetUniformLocation(mShaderProgram->getProgram(), "Modelview");
    _textureLocation = glGetAttribLocation(mShaderProgram->getProgram(), "TextureCoord");
    _textureUniform = glGetUniformLocation(mShaderProgram->getProgram(), "CC_Texture0");
    
    kmGLPushMatrix();
    
    kmGLLoadIdentity();
   
    auto winSize = Director::getInstance()->getWinSizeInPixels();
//    float zeye = Director::getInstance()->getZEye();
//    float zfarPlane = zeye + winSize.height/2;
    static float rotation = 0;
    //kmGLRotatef(20, 0, 0, 1);
    //the order to call translate/rotate/scale matters
    //in kzMath, the order should be translate -> rotate -> scale
    //in generate mv = t * r * s * m;
    kmGLTranslatef(winSize.width/2, winSize.height/2, 0 );
    kmGLRotatef(rotation++, 1, 1, 1);
    kmGLScalef(150, 150, 150 ); //we must scale the model coordinate such that we can see the model with the zNear and far plane


    if (rotation >= 360 ) {
        rotation = 0;
    }
    //kmGLTranslatef(0, 0, -0.1);
    kmMat4 modelView;
    kmGLGetMatrix(KM_GL_MODELVIEW, &modelView);
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();

    mShaderProgram->setUniformLocationWithMatrix4fv(_modelViewUniform, modelView.mat , 1);
    
   
    
    kmGLPopMatrix();
    
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    //this two calls are replacement for the above call
    glEnableVertexAttribArray(_positionLocation);
    glEnableVertexAttribArray(_colorLocation);
    glEnableVertexAttribArray(_textureLocation);
    
    glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    
    glVertexAttribPointer(_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)(sizeof(float) * 3));
    
    glVertexAttribPointer(_textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)(sizeof(float) * 7));
//
    //set sampler
    GL::bindTexture2D(_textureID);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]),
                   GL_UNSIGNED_BYTE, 0);
    
    
       mShaderProgram->setUniformLocationWith1i(_textureUniform, 0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,vertexCount);
    
    CHECK_GL_ERROR_DEBUG();
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&oldProj);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&oldMv);

}


