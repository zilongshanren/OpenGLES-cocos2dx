/* 
  myshader.vert
  MyCppGame

  Created by guanghui on 4/8/14.

*/


attribute vec4 Position;

//all these are lighting
attribute vec3 Normal;
attribute vec3 DiffuseMaterial;

//attribute vec3 AmbientMaterial;  //is it a_color?
//attribute vec3 SpecularMaterial;
//attribute float Shininess;

uniform mat3 NormalMatrix;


uniform mat4 Modelview;
uniform mat4 Projection;

varying vec3 EyespaceNormal;
varying vec3 Diffuse;


void main()
{
    EyespaceNormal = NormalMatrix * Normal;
    Diffuse = DiffuseMaterial;
//    vec3 N = NormalMatrix * Normal;
//    vec3 L = normalize(LightPosition);
//    vec3 E = vec3(0, 0, 1);
//    vec3 H = normalize(L + E);
//    float df = max(0.0, dot(N, L));
//    float sf = max(0.0, dot(N, H));
//    sf = pow(sf, Shininess);
//    
//    vec3 color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;
//    fragmentColor = vec4(color, 1);
//    gl_Position = CC_PMatrix * modelView * a_position;
    gl_Position = Projection * Modelview * Position;
}