#include "/home/user/june/test/camera/opengl/opengles3-book/Common/Include/esUtil.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct {

GLuint programObject;

}UserData;

//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   {
      return 0;
   }

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

 int Init(ESContext *esContext)
 {
     UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = vPosition;              \n"
      "}                                        \n";

   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "out vec4 fragColor;                          \n"
      "void main()                                  \n"
      "{                                            \n"
      "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
      "}                                            \n";
GLuint vertexShader;
GLuint fragmentShader;
GLuint programObject;
GLint linked;

vertexShader  =LoadShader(GL_VERTEX_SHADER,vShaderStr);
fragmentShader =LoadShader(GL_FRAGMENT_SHADER,fShaderStr);
programObject = glCreateProgram();
if(programObject==0) return 0;
glAttachShader(programObject,vertexShader);
glAttachShader(programObject,fragmentShader);
/*链接program*/
glLinkProgram(programObject);
/*检查链接状态*/
glGetProgramiv(programObject,GL_LINK_STATUS,&linked);

if(!linked)
{
    GLint infoLen = 0;
        glGetProgramiv (programObject,GL_INFO_LOG_LENGTH,&infoLen);
        if(infoLen>1)
        {
            char *infoLog = malloc(sizeof(char)*infoLen);
            glGetShaderInfoLog(programObject,infoLen,NULL,infoLog);
            esLogMessage("Error compiling shader:\n%s\n",infoLog);
            free(infoLog);

        }
        glDeleteShader(programObject);
        return FALSE;

}
/*存储program 对象*/
userData->programObject =programObject;
glClearColor(0.0f,0.0f,0.0f,0.0f);

return TRUE;


 }

 void Draw(ESContext * esContext)
 {
     UserData *useData =  esContext->userData;
     //三角形三个定点的x y和z坐标
     GLfloat  vVertices[]={0.0f,0.5f,0.0f,
     -0.5f,-0.5f,0.0f,
     0.5f,-0.5f,1.0f};
    //设置视点,原点和高度
    glViewport(0,50,esContext->width,esContext->height);
    // 清除 颜色的缓冲区域
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(useData->programObject);
    //加载顶点信息
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES,0,3);



 }
void ShutDown(ESContext *esContext)
{
    UserData * userDate = esContext->userData;
    glDeleteProgram(userDate->programObject);
}
int esMain(ESContext *esContext)
{
    esContext->userData =malloc(sizeof(UserData));
    esCreateWindow(esContext,"Hello Trianglr",320,240,ES_WINDOW_RGB);
    if(!Init(esContext))
    return FALSE;
    esRegisterShutdownFunc(esContext,ShutDown);
    esRegisterDrawFunc(esContext,Draw);

    return GL_TRUE;
}
