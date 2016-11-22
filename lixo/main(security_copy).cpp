
#define GLM_FORCE_RADIANS
#include "include.h"
#define PI 3.14159265359f
#define GRAD_RAD 0.01745329251f

unsigned int ViewPortWidth  = 1024; //screen dimensions
unsigned int ViewPortHeight = 768;

/*****TEXTURES*****/
unsigned char* busimg;
int busimg_width;
int busimg_height;
int busimg_channels;

unsigned char* groundimg;
int groundimg_width;
int groundimg_height;
int groundimg_channels;

unsigned char* sphereimg;
int sphereimg_width;
int sphereimg_height;
int sphereimg_channels;

GLuint texture_id[4];
/*****TEXTURES*****/

/**FLAGS**/
bool text = false;
bool vec = false;
bool sky = false;
/**FLAGS**/

float angle = 0; //dome rotation angle
bool start = false;
Loader* busLoader;
Loader* groundLoader;
Loader* sphereLoader;
SDL_Window *window = NULL; //window
SDL_GLContext glcontext; //to use with opengl
SDL_Event event; //for key handle

Cam* cam; //camera class
Arma* arma;//Bus class

typedef struct flags{
    bool flagUp =false;
    bool flagDown =false;
    bool flagLeft =false;
    bool flagRight =false;
}Flag;
Flag keyFlags;



void DrawText(std::string s)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    gluOrtho2D(0.0, ViewPortWidth, 0.0, ViewPortHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 0.0f);//needs to be called before RasterPos
    glRasterPos2i(50, 650);
    
    void * font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0); 
        glutBitmapCharacter(font, c);
    }

    glRasterPos2i(800, 650);
    std::string laps = "Volta: 1/3";
    for (std::string::iterator i = laps.begin(); i != laps.end(); ++i)
    {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0); 
        glutBitmapCharacter(font, c);
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

}

void textures(const char* buspath, const char* groundpath, const char* spherepath){
    busimg =    SOIL_load_image
    (
        buspath,
        &busimg_width,
        &busimg_height,
        &busimg_channels,
        SOIL_LOAD_AUTO
        );

    groundimg =    SOIL_load_image
    (
        groundpath,
        &groundimg_width,
        &groundimg_height,
        &groundimg_channels,
        SOIL_LOAD_AUTO
        );

    sphereimg =    SOIL_load_image
    (
        spherepath,
        &sphereimg_width,
        &sphereimg_height,
        &sphereimg_channels,
        SOIL_LOAD_AUTO
        );

    // prints out image information
    if (busimg && sphereimg && groundimg)
    {
        //std::clog << " Track SOIL texture loading: success!\n";
        std::clog << " Ground SOIL texture loading: success!\n";
        std::clog << " Sphere SOIL texture loading: success!\n";
        std::clog << " Bus SOIL texture loading: success!\n";

    }
    else
    {
        std::cerr << "SOIL loading error: " << SOIL_last_result() << "\n";
    }

    // generates a texture id
    glGenTextures(4, texture_id );

    // binds the newly generated texture
    glBindTexture( GL_TEXTURE_2D, texture_id[0] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, busimg_width, busimg_height, GL_RGBA, GL_UNSIGNED_BYTE, busimg);
    //glBindTexture( GL_TEXTURE_2D, texture_id[1] );
    //gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, trackimg_width, trackimg_height, GL_RGBA, GL_UNSIGNED_BYTE, trackimg);
    glBindTexture( GL_TEXTURE_2D, texture_id[2] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, groundimg_width, groundimg_height, GL_RGBA, GL_UNSIGNED_BYTE, groundimg);
    glBindTexture( GL_TEXTURE_2D, texture_id[3] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, sphereimg_width, sphereimg_height, GL_RGBA, GL_UNSIGNED_BYTE, sphereimg);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

bool initSDL(){

    //INITIALISING SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)==0){
        std::clog<<"SDL initiated..."<<std::endl;
    } else {
        std::clog<<"Error initialising SDL inside function"<<SDL_GetError()<<std::endl;
        return false;
    }

    //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    //CREATING WINDOW
        window = SDL_CreateWindow("Jampa Bus Racing Game",200,200,ViewPortWidth, ViewPortHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        if(window){
            std::clog<<"Window created..."<<std::endl;
        } else {
            std::clog<<"Error creating Window "<<SDL_GetError()<<std::endl;
            return false;
        }

    //CREATING CONTEXT
        glcontext = SDL_GL_CreateContext(window);
        if(glcontext){
            std::clog<<"Context Created..."<<std::endl;
        } else {
            std::clog<<"Error creating context "<<SDL_GetError()<<std::endl;
            return false;
        }

        return true;
}

void close(){

           delete cam;
           delete arma;
           delete sphereLoader;
           delete groundLoader;
           delete busLoader;
           
           SDL_DestroyWindow( window );
    
           window = NULL;

           //Quit SDL subsystems
           SDL_Quit();
}

void DrawGround(){

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texture_id[2]);

        glBegin(GL_TRIANGLES);
        for(int i=0; i<groundLoader->numVertices;i+=3)
        {
            glTexCoord2d(groundLoader->textureArray[i].x, groundLoader->textureArray[i].y);
            glNormal3f(groundLoader->normalArray[i].x, groundLoader->normalArray[i].y, groundLoader->normalArray[i].z);
            glVertex3f(groundLoader->vertexArray[i].x,groundLoader->vertexArray[i].y,groundLoader->vertexArray[i].z);
        
            glTexCoord2d(groundLoader->textureArray[i+1].x, groundLoader->textureArray[i+1].y);
            glNormal3f(groundLoader->normalArray[i+1].x, groundLoader->normalArray[i+1].y, groundLoader->normalArray[i+1].z);
            glVertex3f(groundLoader->vertexArray[i+1].x, groundLoader->vertexArray[i+1].y, groundLoader->vertexArray[i+1].z);
       
            glTexCoord2d(groundLoader->textureArray[i+2].x, groundLoader->textureArray[i+2].y);
            glNormal3f(groundLoader->normalArray[i+2].x, groundLoader->normalArray[i+2].y, groundLoader->normalArray[i+2].z);
            glVertex3f(groundLoader->vertexArray[i+2].x, groundLoader->vertexArray[i+2].y, groundLoader->vertexArray[i+2].z);
        
        }
        glEnd();
}

void DrawSphere(){

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texture_id[3]);

        glBegin(GL_TRIANGLES);
        for(int i=0; i<sphereLoader->numVertices;i+=3)
        {
            glTexCoord2d(sphereLoader->textureArray[i].x, sphereLoader->textureArray[i].y);
            glNormal3f(sphereLoader->normalArray[i].x, sphereLoader->normalArray[i].y, sphereLoader->normalArray[i].z);
            glVertex3f(sphereLoader->vertexArray[i].x,sphereLoader->vertexArray[i].y,sphereLoader->vertexArray[i].z);
        
            glTexCoord2d(sphereLoader->textureArray[i+1].x, sphereLoader->textureArray[i+1].y);
            glNormal3f(sphereLoader->normalArray[i+1].x, sphereLoader->normalArray[i+1].y, sphereLoader->normalArray[i+1].z);
            glVertex3f(sphereLoader->vertexArray[i+1].x, sphereLoader->vertexArray[i+1].y, sphereLoader->vertexArray[i+1].z);
        
            glTexCoord2d(sphereLoader->textureArray[i+2].x, sphereLoader->textureArray[i+2].y);
            glNormal3f(sphereLoader->normalArray[i+2].x, sphereLoader->normalArray[i+2].y, sphereLoader->normalArray[i+2].z);
            glVertex3f(sphereLoader->vertexArray[i+2].x, sphereLoader->vertexArray[i+2].y, sphereLoader->vertexArray[i+2].z);
        
        }
        glEnd();
}

void DrawBus(){

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);

    glBegin(GL_TRIANGLES);
        for(int i=0; i<busLoader->numVertices;i+=3)
        {
            glTexCoord2d(busLoader->textureArray[i].x, busLoader->textureArray[i].y);
            glNormal3f(busLoader->normalArray[i].x, busLoader->normalArray[i].y, busLoader->normalArray[i].z);
            glVertex3f(busLoader->vertexArray[i].x,busLoader->vertexArray[i].y,busLoader->vertexArray[i].z);
            
            glTexCoord2d(busLoader->textureArray[i+1].x, busLoader->textureArray[i+1].y);
            glNormal3f(busLoader->normalArray[i+1].x, busLoader->normalArray[i+1].y, busLoader->normalArray[i+1].z);
            glVertex3f(busLoader->vertexArray[i+1].x, busLoader->vertexArray[i+1].y, busLoader->vertexArray[i+1].z);
            
            glTexCoord2d(busLoader->textureArray[i+2].x, busLoader->textureArray[i+2].y);
            glNormal3f(busLoader->normalArray[i+2].x, busLoader->normalArray[i+2].y, busLoader->normalArray[i+2].z);
            glVertex3f(busLoader->vertexArray[i+2].x, busLoader->vertexArray[i+2].y, busLoader->vertexArray[i+2].z);
            
        }
    glEnd();
}


void DrawAxis(){

   glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);    // eixo X
        glVertex3f(0.0f, 0.0f, 0.0f);   //vermelho
        glVertex3f(2.0f, 0.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);    // eixo Y
        glVertex3f(0.0f, 0.0f, 0.0f);   //verde
        glVertex3f(0.0f, 2.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);    // eixo Z
        glVertex3f(0.0f, 0.0f, 0.0f);   //azul
        glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
}


void light(){
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};    // "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
}


/*****IMAGE******/
void loadImg(const char* imgPath, SDL_Window* window, bool* running){
    
    SDL_Surface *screen;
    SDL_Surface *image;

    image = SDL_LoadBMP(imgPath); // loads image
    if(!image)
        std::clog<<"ERROR LOADING.BMP"<<std::endl;

        //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = 0;
    offset.y = 0;

    screen = SDL_GetWindowSurface(window);

     //Blit the surface
     SDL_BlitSurface(image, NULL, screen, &offset); // blit it to the screen
     SDL_FreeSurface(image);

     // this works just like SDL_Flip() in SDL 1.2
     SDL_UpdateWindowSurface(window);
     // show image for 30 seconds
     


     while(SDL_PollEvent(&event))
     {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_RETURN:
                std::clog<<"game started"<<std::endl;
                *running = true;
                break;

                case  SDLK_ESCAPE:
                std::clog<<"bye bye"<<std::endl;
                close();
                return;
                break;

                default:
                break;
            }
        }
    }
}
/*****IMAGE******/


void handleKeys(bool* running){


    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            *(running) = false;

        } else if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_UP:
                std::clog<<"pressed up"<<std::endl;
                keyFlags.flagUp = true;
                break;

                case SDLK_DOWN:
                std::clog<<"pressed down"<<std::endl;
                keyFlags.flagDown = true;

                break;

                case SDLK_RIGHT:
                std::clog<<"pressed right"<<std::endl;
                keyFlags.flagRight = true;
                break;

                case SDLK_LEFT:
                std::clog<<"pressed left"<<std::endl;
                keyFlags.flagLeft = true;
                break;

                case SDLK_v:
                std::clog<<"Vector status changed"<<std::endl;
                vec = !vec;
                break;

                case SDLK_t:
                std::clog<<"Text status changed"<<std::endl;
                text = !text;
                break;

                case SDLK_s:
                std::clog<<"let's see the sky"<<std::endl;
                sky = !sky;
                break;

                default:
                break;
            }

        } else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_UP:
                std::clog<<"released up"<<std::endl;
                keyFlags.flagUp = false;
                break;

                case SDLK_DOWN:
                std::clog<<"released down"<<std::endl;
                keyFlags.flagDown = false;
                break;

                case SDLK_RIGHT:
                std::clog<<"released right"<<std::endl;
                keyFlags.flagRight = false;
                break;

                case SDLK_LEFT:
                std::clog<<"released left"<<std::endl;
                keyFlags.flagLeft = false;
                start = true;
                break;

                default:
                break;
            }
        }
    }
}
void configScene(Arma* arma){
}

void display(void){


    glColor3f(1.0,1.0,1.0);


    glViewport(0, 0, ViewPortWidth, ViewPortHeight);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    


	gluPerspective(	55.0f, 						        // angulo de abertura da camera
				  (1.0f * ViewPortWidth)/ViewPortWidth, // aspecto da imagem
				   0.5f, 	   							// near plane
				   6000.0f);  							//far plane

    if(!sky){//x,y,z
	gluLookAt(	0, 1, 8,	// posição da câmera
				0, 0, 0,	// ponto para o qual a camera está olhando
				0.0f, 1.0f, 0.0f);	// vetor "up"
    } else {
   gluLookAt(	3, 2, 50,	// posição da câmera
				0, 0, 0,	// ponto para o qual a camera está olhando
				0.0f, 1.0f, 0.0f);	// vetor "up"
    }



    glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix(); 
	//glScalef(60.0,60.0,60.0);
	glScalef(10, 10, 10); 
    DrawGround(); //ground
	glPopMatrix();


    //bus->acceleration.z = 0.1;
    //bus->updatePosition();
	

    glPushMatrix();
    glTranslatef(arma->position.x, arma->position.y, arma->position.z); //desloc.x, desloc.y, desloc.z
    //glRotatef(bus->busAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    DrawBus();
    glPopMatrix();

	arma->moveArma(keyFlags.flagUp, keyFlags.flagDown, keyFlags.flagLeft, keyFlags.flagRight);
    

    glColor3f(1.0,1.0,1.0);
	glPushMatrix();
    glRotatef(angle+=0.01, 0, 1, 0);
	glScalef(20, 20, 20);
	DrawSphere(); //sky
	glPopMatrix();
    }


int main(int argc, char** argv){

    glutInit(&argc, argv);
    std::clog<<"/*********TRYING TO INIT SDL**********/"<<std::endl;
    if(initSDL()){
        std::clog<<"SDL Initialised Successfully..."<<std::endl;
    } else {
        std::clog<<"Error initialising SDL "<<SDL_GetError()<<std::endl;
        return 1;
    }
    std::clog<<"/*********TRYING TO INIT SDL**********/"<<std::endl;

    std::clog<<"/*********LOADING MODEL**********/"<<std::endl;
    busLoader = new Loader();
    //trackLoader = new Loader();
    groundLoader = new Loader();
    sphereLoader = new Loader();
    //trackLoader->loadScene("Models/track.obj");
    busLoader->loadScene("Models/h1.obj");
    groundLoader->loadScene("Models/ground2.obj");
    sphereLoader->loadScene("Models/sphere.obj");

    arma = new Arma();
    cam = new Cam(arma->position);
    std::clog<<"/*********LOADING MODEL**********/"<<std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);

    light();
    textures("Models/texth1.jpg", "Models/terrain.png", "Models/dome.png");

    //VelocityText(renderer,font, message, text, textRect, textColor);
    bool running = false;

    /*****IMAGE*****/
    while(!running){
        loadImg("menu.bmp", window, &running);
    }
    /*****IMAGE*****/
    
    while(running)
    {

        handleKeys(&running);
        display();

            //Update screen
        SDL_GL_SwapWindow(window);
    }



    close();

    return 0;
}
