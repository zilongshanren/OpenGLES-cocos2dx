#include "HelloWorldScene.h"
#include "ParametricEquations.hpp"
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

void HelloWorld::initialize()
{
    
    m_spinning = false;
    m_pressedButton = -1;
    m_animation.Active = false;
    m_buttonSurfaces[0] = 0;
    m_buttonSurfaces[1] = 1;
    m_buttonSurfaces[2] = 4;
    m_buttonSurfaces[3] = 3;
    m_buttonSurfaces[4] = 2;
    m_currentSurface = 5;
        
        
    auto width = Director::getInstance()->getVisibleSize().width;
    auto height = Director::getInstance()->getVisibleSize().height;
    m_trackballRadius = width / 3;
    m_buttonSize.y = height / 10;
    m_buttonSize.x = 4 * m_buttonSize.y / 3;
    m_screenSize = ivec2(width, height - m_buttonSize.y);
    m_centerPoint = m_screenSize / 2;
    
    vector<ISurface*> surfaces(SurfaceCount);
    surfaces[0] = new Cone(3, 1);
    surfaces[1] = new Sphere(1.4f);
    surfaces[2] = new Torus(1.4, 0.3);
    surfaces[3] = new TrefoilKnot(1.8f);
    surfaces[4] = new KleinBottle(0.2f);
    surfaces[5] = new MobiusStrip(1);    
    
    //init VBO
    vector<ISurface*>::const_iterator surface;
    for (surface = surfaces.begin(); surface != surfaces.end(); ++surface) {
        
        // Create the VBO for the vertices.
        vector<float> vertices;
        (*surface)->GenerateVertices(vertices);
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(vertices[0]),
                     &vertices[0],
                     GL_STATIC_DRAW);
        
        // Create a new VBO for the indices if needed.
        int indexCount = (*surface)->GetLineIndexCount();
        GLuint indexBuffer;
        if (!m_drawables.empty() && indexCount == m_drawables[0].IndexCount) {
            indexBuffer = m_drawables[0].IndexBuffer;
        } else {
            vector<GLushort> indices(indexCount);
            (*surface)->GenerateLineIndices(indices);
            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indexCount * sizeof(GLushort),
                         &indices[0],
                         GL_STATIC_DRAW);
        }
        
        Drawable drawable = { vertexBuffer, indexBuffer, indexCount};
        m_drawables.push_back(drawable);
    }
    
    //do cleanup
    for (int i = 0; i < SurfaceCount; i++)
        delete surfaces[i];
    
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
    
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

    
    //how to map texture HelloWorld.png to my triangles
    _textureID =  Director::getInstance()->getTextureCache()->addImage("guanyu1.png")->getName();

    m_colorSlot = mShaderProgram->getAttribLocation("a_color");
    m_positionSlot = mShaderProgram->getAttribLocation("a_position");
    //we must specify the renderingEngine
    
    this->initialize();

    
    this->schedule(schedule_selector(HelloWorld::updateAnimation), 1.0 / 60);
    
    
    
    return true;
}

void HelloWorld::updateAnimation(float dt)
{
    if (m_animation.Active) {
        m_animation.Elapsed += dt;
        if (m_animation.Elapsed > m_animation.Duration)
            m_animation.Active = false;
    }
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
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    //add your own draw code here
    vector<Visual> visuals(SurfaceCount);
    
    if (!m_animation.Active) {
        PopulateVisuals(&visuals[0]);
    } else {
        float t = m_animation.Elapsed / m_animation.Duration;
        
        for (int i = 0; i < SurfaceCount; i++) {
            
            const Visual& start = m_animation.StartingVisuals[i];
            const Visual& end = m_animation.EndingVisuals[i];
            Visual& tweened = visuals[i];
            
            tweened.Color = start.Color.Lerp(t, end.Color);
            tweened.LowerLeft = start.LowerLeft.Lerp(t, end.LowerLeft);
            tweened.ViewportSize = start.ViewportSize.Lerp(t, end.ViewportSize);
            tweened.Orientation = start.Orientation.Slerp(t, end.Orientation);
        }
    }
    
    //drawing
    
    this->rendering(visuals);
    
    
    CHECK_GL_ERROR_DEBUG();
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadMatrix(&oldProj);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadMatrix(&oldMv);

}

void HelloWorld::rendering(const vector<Visual>& visuals)
{
    vector<Visual>::const_iterator visual = visuals.begin();
    int drawCall = 0;
    int drawIndices = 0;
    for (int visualIndex = 0; visual != visuals.end(); ++visual, ++visualIndex) {
        
        // Set the viewport transform.
        ivec2 size = visual->ViewportSize;
        ivec2 lowerLeft = visual->LowerLeft;
        glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
        
        // Set the model-view transform.
        m_translation = mat4::Translate(0, 0, -0.5);
        mat4 rotation = visual->Orientation.ToMatrix();
        mat4 scale = mat4::Scale(0.4);
        mat4 modelview = scale * rotation * m_translation;
        m_modelviewUniform = mShaderProgram->getUniformLocation("modelView");
        glUniformMatrix4fv(m_modelviewUniform, 1, 0, modelview.Pointer());
        
        // Set the projection transform.
        //Since it's cocos2d-x, we don't need any projectionMatrix
//        float h = 4.0f * size.y / size.x;
//        mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
//        glUniformMatrix4fv(m_projectionUniform, 1, 0, projectionMatrix.Pointer());
        
        // Set the color.
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION);
        vec3 color = visual->Color;
        glVertexAttrib4f(m_colorSlot, color.x, color.y, color.z, 1);
        
        // Draw the wireframe.
        int stride = sizeof(vec3);
        const Drawable& drawable = m_drawables[visualIndex];
        glBindBuffer(GL_ARRAY_BUFFER, drawable.VertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.IndexBuffer);

        
        glVertexAttribPointer(m_positionSlot, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
        
        
        glDrawElements(GL_TRIANGLES, drawable.IndexCount, GL_UNSIGNED_SHORT, (GLvoid*)0);
        
        drawCall++;
        drawIndices += drawable.IndexCount;

    }
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(drawCall, drawIndices);

}


bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto location = touch->getLocationInView();
    
    m_fingerStart = ivec2(location.x, location.y);
    m_previousOrientation = m_orientation;
    m_pressedButton = MapToButton(ivec2(location.x, location.y));
    if (m_pressedButton == -1)
        m_spinning = true;
    
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto location = touch->getLocationInView();

    if (m_spinning) {
        vec3 start = MapToSphere(m_fingerStart);
        vec3 end = MapToSphere(ivec2(location.x, location.y));
        Quaternion delta = Quaternion::CreateFromVectors(start, end);
        m_orientation = delta.Rotated(m_previousOrientation);
    }
    
    if (m_pressedButton != -1 && m_pressedButton != MapToButton(ivec2(location.x, location.y)))
        m_pressedButton = -1;
}

void HelloWorld::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    m_spinning = false;
    auto location = touch->getLocationInView();
    if (m_pressedButton != -1 && m_pressedButton == MapToButton(ivec2(location.x, location.y)) &&
        !m_animation.Active)
    {
        m_animation.Active = true;
        m_animation.Elapsed = 0;
        m_animation.Duration = 0.25f;
        
        PopulateVisuals(&m_animation.StartingVisuals[0]);
        swap(m_buttonSurfaces[m_pressedButton], m_currentSurface);
        PopulateVisuals(&m_animation.EndingVisuals[0]);
        CCLOG("touch end");
    }
    
    m_pressedButton = -1;
}


int HelloWorld::MapToButton(ivec2 touchpoint) const
{
    if (touchpoint.y  < m_screenSize.y - m_buttonSize.y)
        return -1;
    
    int buttonIndex = touchpoint.x / m_buttonSize.x;
    if (buttonIndex >= ButtonCount)
        return -1;
    
    return buttonIndex;
}

vec3 HelloWorld::MapToSphere(ivec2 touchpoint) const
{
    vec2 p = touchpoint - m_centerPoint;
    
    // Flip the Y axis because pixel coords increase towards the bottom.
    p.y = -p.y;
    
    const float radius = m_trackballRadius;
    const float safeRadius = radius - 1;
    
    if (p.Length() > safeRadius) {
        float theta = atan2(p.y, p.x);
        p.x = safeRadius * cos(theta);
        p.y = safeRadius * sin(theta);
    }
    
    float z = sqrt(radius * radius - p.LengthSquared());
    vec3 mapped = vec3(p.x, p.y, z);
    return mapped / radius;
}

void HelloWorld::PopulateVisuals(Visual *visuals)const
{
    for (int buttonIndex = 0; buttonIndex < ButtonCount; buttonIndex++) {
        
        int visualIndex = m_buttonSurfaces[buttonIndex];
        visuals[visualIndex].Color = vec3(0.25f, 0.25f, 0.25f);
        if (m_pressedButton == buttonIndex)
            visuals[visualIndex].Color = vec3(0.5f, 0.5f, 0.5f);
        
        visuals[visualIndex].ViewportSize = m_buttonSize;
        visuals[visualIndex].LowerLeft.x = buttonIndex * m_buttonSize.x;
        visuals[visualIndex].LowerLeft.y = 0;
        visuals[visualIndex].Orientation = Quaternion();
    }
    
    visuals[m_currentSurface].Color = m_spinning ? vec3(1, 1, 0.75f) : vec3(1, 1, 0.5f);
    visuals[m_currentSurface].LowerLeft = ivec2(0, m_buttonSize.y);
    visuals[m_currentSurface].ViewportSize = ivec2(m_screenSize.x, m_screenSize.y);
    visuals[m_currentSurface].Orientation = m_orientation;

}
