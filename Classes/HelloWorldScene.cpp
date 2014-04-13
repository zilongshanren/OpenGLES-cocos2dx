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
    
    {
        // Allocate space for the cone vertices.
        m_cone.resize((coneSlices + 1) * 2);
        
        // Initialize the vertices of the triangle strip.
        vector<Vertex>::iterator vertex = m_cone.begin();
        const float dtheta = TwoPi / coneSlices;
        for (float theta = 0; vertex != m_cone.end(); theta += dtheta) {
            
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
    }
    
    {
        // Allocate space for the disk vertices.
        m_disk.resize(coneSlices + 2);
        
        // Initialize the center vertex of the triangle fan.
        vector<Vertex>::iterator vertex = m_disk.begin();
        vertex->Color = vec4(0.75, 0.75, 0.75, 1);
        vertex->Position.x = 0;
        vertex->Position.y = 1 - coneHeight;
        vertex->Position.z = 0;
        vertex++;
        
        // Initialize the rim vertices of the triangle fan.
        const float dtheta = TwoPi / coneSlices;
        for (float theta = 0; vertex != m_disk.end(); theta += dtheta) {
            vertex->Color = vec4(0.75, 0.75, 0.75, 1);
            vertex->Position.x = coneRadius * cos(theta);
            vertex->Position.y = 1 - coneHeight;
            vertex->Position.z = coneRadius * sin(theta);
            vertex++;
        }
    }

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
    label->setVisible(false);
    this->addChild(label);

    this->initCone();
    
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

    
    mColorLocation = glGetUniformLocation( mShaderProgram->getProgram(), "a_color");
    
    //how to map texture HelloWorld.png to my triangles
    _textureID =  Director::getInstance()->getTextureCache()->addImage("guanyu1.png")->getName();

    
    m_animation.Elapsed = 0;
    m_animation.Start = m_animation.Current = m_animation.End;
    m_animation.End = Quaternion::CreateFromAxisAngle(vec3(1.0,1.0,0), 3.14 / 2);

    
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
    kmGLLoadIdentity();
    
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    Director::getInstance()->setDepthTest(true);
    
    mat4 rotation(m_animation.Current.ToMatrix());
    mat4 translation = mat4::Translate(0, 0, -0.5);
    
    // Set the model-view matrix.
    GLint modelviewUniform = glGetUniformLocation(mShaderProgram->getProgram(), "modelView");
    mat4 modelviewMatrix = translation * rotation;
    glUniformMatrix4fv(modelviewUniform, 1, 0, modelviewMatrix.Pointer());
    

 
    static float timeStep = 1.0 / 60.0f;
    m_animation.Elapsed += timeStep;
 
    float mu = m_animation.Elapsed / 0.25f;
    m_animation.Current = m_animation.Start.Slerp(mu, m_animation.End);

           //set color for each vertex  note:the color value is between 0-1

//    mShaderProgram->setUniformLocationWith4f(mColorLocation, 0.5, 0.5, 0.5, 1);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &m_cone[0], GL_STATIC_DRAW);
   
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_POSITION);
    
    // Draw the cone.
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_cone[0].Position.x;
        const GLvoid* pColors = &m_cone[0].Color.x;
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_cone.size());
    }
    
    // Draw the disk that caps off the base of the cone.
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_disk[0].Position.x;
        const GLvoid* pColors = &m_disk[0].Color.x;
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_disk.size());
    }
    
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(2,m_cone.size() + m_disk.size());
    
    CHECK_GL_ERROR_DEBUG();
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&oldProj);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&oldMv);

}


