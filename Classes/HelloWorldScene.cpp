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

void HelloWorld::initCone()
{
    const float coneRadius = 0.5f;
    const float coneHeight = 1.866f;
    const int coneSlices = 40;
    const float dtheta = TwoPi / coneSlices;
    const int vertexCount = coneSlices * 2 + 1;
    
    m_coneVertices.resize(vertexCount);
    vector<Vertex>::iterator vertex = m_coneVertices.begin();
    // Cone's body
    for (float theta = 0; vertex != m_coneVertices.end() - 1; theta += dtheta) {
        // Grayscale gradient
        float brightness = abs(sin(theta));
        vec4 color(brightness, brightness, brightness, 1);
        // Apex vertex
        vertex->Position = vec3(0, 1, 0);
        vertex->Color = color;
        vertex++;
        // Rim vertex
        vertex->Position.x = coneRadius * cos(theta);
        vertex->Position.y = 1 - coneHeight;
        vertex->Position.z = coneRadius * sin(theta);
        vertex->Color = color;
        vertex++;
    }
    // Disk center
    vertex->Position = vec3(0, 1 - coneHeight, 0);
    vertex->Color = vec4(1, 1, 1, 1);
    
    
    m_bodyIndexCount = coneSlices * 3;
    m_diskIndexCount = coneSlices * 3;
    m_coneIndices.resize(m_bodyIndexCount + m_diskIndexCount);
    
    vector<GLubyte>::iterator index = m_coneIndices.begin();
    // Body triangles
    for (int i = 0; i < coneSlices * 2; i += 2) {
        *index++ = i;
        *index++ = (i + 1) % (2 * coneSlices);
        *index++ = (i + 3) % (2 * coneSlices);
    }
    // Disk triangles
    const int diskCenterIndex = vertexCount - 1;
    for (int i = 1; i < coneSlices * 2 + 1; i += 2) {
        *index++ = diskCenterIndex;
        *index++ = i;
        *index++ = (i + 2) % (2 * coneSlices);
    }

}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
//    this->initCone();

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();
    
    auto label = Label::create("Hello Triangle!", "Arial", 40);
    label->setPosition(Point(winSize.width/2, winSize.height/2));
    label->setVisible(false);
    this->addChild(label);

    
    //方法1：使用有mvp的shader  注意坐标的变化
//    mShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);

    
    //方法2:使用系统内置的shader 无mvp
//    mShaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    //方法3：使用自己的shader
    mShaderProgram = new GLProgram; //TODO:记得在某个地方释放资源
    //如果使用下面的bindAttribLocation的话，那么position和color分别要命名为 a_position和a_color
    mShaderProgram->initWithFilenames("myshader.vert", "myshader.frag");
    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    mShaderProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    
    //don't forget to call link and updateUniforms.
    mShaderProgram->link();
    mShaderProgram->updateUniforms();

    //maybe I should call delete when the HelloWorldScene get destoryed

    //init the member function
    _pivotPosition = ivec2(winSize.width/2, winSize.height/2);
    _scale = 1.0f;
    _rotateAngle = 0.0f;
    
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    
    mColorLocation = glGetUniformLocation( mShaderProgram->getProgram(), "a_color");
    
    //how to map texture HelloWorld.png to my triangles
    _textureID =  Director::getInstance()->getTextureCache()->addImage("guanyu1.png")->getName();

    
    const float coneRadius = 0.5f;
    const float coneHeight = 1.866f;
    const int coneSlices = 40;
    const float dtheta = TwoPi / coneSlices;
    const int vertexCount = coneSlices * 2 + 1;
    const int diskCenterIndex = vertexCount - 1;
    
    m_bodyIndexCount = coneSlices * 3;
    m_diskIndexCount = coneSlices * 3;
    
    vector<Vertex> coneVertices(vertexCount);
    vector<Vertex>::iterator vertex = coneVertices.begin();
    
    // Cone's body
    for (float theta = 0; vertex != coneVertices.end() - 1; theta += dtheta) {
        
        // Grayscale gradient
        float brightness = abs(sin(theta));
        vec4 color(brightness, brightness, brightness, 1);
        
        // Apex vertex
        vertex->Position = vec3(0, 1, 0);
        vertex->Color = color;
        vertex++;
        
        // Rim vertex
        vertex->Position.x = coneRadius * cos(theta);
        vertex->Position.y = 1 - coneHeight;
        vertex->Position.z = coneRadius * sin(theta);
        vertex->Color = color;
        vertex++;
    }
    
    // Disk center
    vertex->Position = vec3(0, 1 - coneHeight, 0);
    vertex->Color = vec4(1, 1, 1, 1);
    
    // Create the VBO for the vertices.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 coneVertices.size() * sizeof(coneVertices[0]),
                 &coneVertices[0],
                 GL_STATIC_DRAW);
    
    vector<GLubyte> coneIndices(m_bodyIndexCount + m_diskIndexCount);
    vector<GLubyte>::iterator index = coneIndices.begin();
    
    // Body triangles
    for (int i = 0; i < coneSlices * 2; i += 2) {
        *index++ = i;
        *index++ = (i + 1) % (2 * coneSlices);
        *index++ = (i + 3) % (2 * coneSlices);
    }
    
    // Disk triangles
    for (int i = 1; i < coneSlices * 2 + 1; i += 2) {
        *index++ = diskCenterIndex;
        *index++ = i;
        *index++ = (i + 2) % (2 * coneSlices);
    }
    
    // Create the VBO for the indices.
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 coneIndices.size() * sizeof(coneIndices[0]),
                 &coneIndices[0],
                 GL_STATIC_DRAW);
    
 
    
    
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
    
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
//    Director::getInstance()->setDepthTest(true);
    
    GLuint modelViewLocation = mShaderProgram->getUniformLocation("modelView");
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLRotatef(_rotateAngle, 0, 0, 1);
    kmGLScalef(_scale, _scale, _scale);
    kmGLGetMatrix(KM_GL_MODELVIEW, &_modelViewMV); // we must call kmGLGetMatrix to retrieve the matrix
    mShaderProgram->setUniformLocationWithMatrix4fv(modelViewLocation, _modelViewMV.mat, 1);
    
    //set color for each vertex  note:the color value is between 0-1

//    mShaderProgram->setUniformLocationWith4f(mColorLocation, 0.5, 0.5, 0.5, 1);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &m_cone[0], GL_STATIC_DRAW);
    GLsizei stride = sizeof(Vertex);
//    const GLvoid* pCoords = &m_coneVertices[0].Position.x;
//    const GLvoid* pColors = &m_coneVertices[0].Color.x;
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);

    

    glVertexAttribPointer(GL::VERTEX_ATTRIB_FLAG_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glVertexAttribPointer(GL::VERTEX_ATTRIB_FLAG_COLOR, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)sizeof(vec3));
    
//    const GLvoid* bodyIndices = 0;
    const GLvoid* diskIndices = &m_bodyIndexCount;
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, m_bodyIndexCount, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glDrawElements(GL_TRIANGLES, m_diskIndexCount, GL_UNSIGNED_BYTE, diskIndices);
    
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,m_diskIndexCount + m_bodyIndexCount);
    
    CHECK_GL_ERROR_DEBUG();
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&oldProj);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&oldMv);

}


bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _scale = 1.5;
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
    Point location = touch->getLocationInView();
    vec2 direction = vec2(vec2(location.x, location.y) - _pivotPosition).Normalized();
    // Flip the y-axis because pixel coords increase toward the bottom.
    
    direction.y = -direction.y;
    
    _rotateAngle = std::acos(direction.y) * 180.0f / 3.14159f;
    
    if (direction.x > 0){
        _rotateAngle = -_rotateAngle;
    }
    CCLOG("rotate = %f", _rotateAngle);
}

void HelloWorld::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    _scale = 1.0f;
}

