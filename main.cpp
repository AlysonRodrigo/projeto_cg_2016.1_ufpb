#include "include.h"

using namespace std;

unsigned int ViewPortWidth  = 1024; //screen dimensions
unsigned int ViewPortHeight = 768;

/*****TEXTURES*****/
unsigned char* armaimg;
int armaimg_width;
int armaimg_height;
int armaimg_channels;

unsigned char* groundimg;
int groundimg_width;
int groundimg_height;
int groundimg_channels;

unsigned char* sphereimg;
int sphereimg_width;
int sphereimg_height;
int sphereimg_channels;

unsigned char* cuboimg;
int cuboimg_width;
int cuboimg_height;
int cuboimg_channels;

unsigned char* balaimg;
int balaimg_width;
int balaimg_height;
int balaimg_channels;

GLuint texture_id[5];
/*****TEXTURES*****/

/*****FLAGS*****/
bool eixo = false;
bool texto = false;

typedef struct flags{
    bool flagUp = false;
    bool flagDown = false;
    bool flagLeft = false;
    bool flagRight = false;
	bool flagButtonLeft = false;
}Flag;
Flag keyFlags;
/*****FLAGS*****/

/*****AUDIO*****/
Mix_Chunk* tiro;
/*****ÁUDIO*****/

/*****TEMPO*****/
time_t inicio, fim;
int tempo_execucao;
/*****TEMPO*****/

/*****LOADERS*****/
Loader* armaLoader;
Loader* cuboLoader;
Loader* groundLoader;
Loader* sphereLoader;
Loader* balaLoader;
/*****LOADERS*****/

/*****OBJETOS*****/
Arma* arma;
/*****OBJETOS*****/

float angle_camera = 55.0;
float angle_sky = 0; //dome rotation angle
bool start = false;
bool running = false;//verifica se o jogo está rodando
int acertos = 0;
bool trava = true;


SDL_Window *window = NULL; //window
SDL_GLContext glcontext; //to use with opengl
SDL_Event event; //for key handle


/*******************************************************************************************************************
					FUNÇÃO DrawTempo
********************************************************************************************************************/
void DrawTexto(std::string tempo, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0.0, ViewPortWidth, 0.0, ViewPortHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 0.0f);//needs to be called before RasterPos

	//Desenha o nome "Time:" na tela
	glRasterPos2i(x, y);
    for (std::string::iterator i = tempo.begin(); i != tempo.end(); ++i)
    {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

}

void contAcertos(){
}


/*******************************************************************************************************************
					FUNÇÃO textures
********************************************************************************************************************/
void textures(const char* armapath, const char* groundpath, const char* spherepath, const char* cubopath, const char* balapath){
    armaimg =    SOIL_load_image
    (
        armapath,
        &armaimg_width,
        &armaimg_height,
        &armaimg_channels,
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

	cuboimg =    SOIL_load_image
    (
        cubopath,
        &cuboimg_width,
        &cuboimg_height,
        &cuboimg_channels,
        SOIL_LOAD_AUTO
        );

	balaimg =    SOIL_load_image
    (
        balapath,
        &balaimg_width,
        &balaimg_height,
        &balaimg_channels,
        SOIL_LOAD_AUTO
        );


    // prints out image information
    if (armaimg && groundimg && sphereimg && cuboimg && balaimg)
    {
        std::clog << " Arma SOIL texture loading: success!\n";
        std::clog << " Ground SOIL texture loading: success!\n";
        std::clog << " Sphere SOIL texture loading: success!\n";
        std::clog << " Cubo SOIL texture loading: success!\n";
	    std::clog << " Bala SOIL texture loading: success!\n";

    }
    else
    {
        std::cerr << "SOIL loading error: " << SOIL_last_result() << "\n";
    }

    // generates a texture id
    glGenTextures(5, texture_id );

    // binds the newly generated texture
    glBindTexture( GL_TEXTURE_2D, texture_id[0] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, armaimg_width, armaimg_height, GL_RGBA, GL_UNSIGNED_BYTE, armaimg);
    glBindTexture( GL_TEXTURE_2D, texture_id[1] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, cuboimg_width, cuboimg_height, GL_RGBA, GL_UNSIGNED_BYTE, cuboimg);
    glBindTexture( GL_TEXTURE_2D, texture_id[2] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, groundimg_width, groundimg_height, GL_RGBA, GL_UNSIGNED_BYTE, groundimg);
    glBindTexture( GL_TEXTURE_2D, texture_id[3] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, sphereimg_width, sphereimg_height, GL_RGBA, GL_UNSIGNED_BYTE, sphereimg);
	glBindTexture( GL_TEXTURE_2D, texture_id[4] );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, balaimg_width, balaimg_height, GL_RGBA, GL_UNSIGNED_BYTE, balaimg);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

/*******************************************************************************************************************
										FUNÇÃO initSDL
********************************************************************************************************************/
bool initSDL(){

    //INITIALISING SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)==0){
       std::clog<<"SDL initiated..."<<std::endl;
    } else {
       std::clog<<"Error initialising SDL inside function"<<SDL_GetError()<<std::endl;
     //   return false;
   }
	//SDL_Init();
	//Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);//1 mono e 2 stereo
    //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    //CREATING WINDOW
        window = SDL_CreateWindow("Shooter",200,200,ViewPortWidth, ViewPortHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
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

/*******************************************************************************************************************
										FUNÇÃO close
********************************************************************************************************************/
void close(){
		  // Mix_CloseAudio();
		 //  Mix_FreeChunk(tiro);
           delete arma;
           delete sphereLoader;
           delete groundLoader;
           delete armaLoader;
		   delete cuboLoader;
		   delete balaLoader;


           SDL_DestroyWindow( window );

           window = NULL;

           //Quit SDL subsystems
           SDL_Quit();
}

/*******************************************************************************************************************
										FUNÇÃO DrawArma
********************************************************************************************************************/
void DrawArma(){

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);

    glBegin(GL_TRIANGLES);
        for(int i=0; i<armaLoader->numVertices;i+=3)
        {
            glTexCoord2d(armaLoader->textureArray[i].x, armaLoader->textureArray[i].y);
            glNormal3f(armaLoader->normalArray[i].x, armaLoader->normalArray[i].y, armaLoader->normalArray[i].z);
            glVertex3f(armaLoader->vertexArray[i].x,armaLoader->vertexArray[i].y,armaLoader->vertexArray[i].z);

            glTexCoord2d(armaLoader->textureArray[i+1].x, armaLoader->textureArray[i+1].y);
            glNormal3f(armaLoader->normalArray[i+1].x, armaLoader->normalArray[i+1].y, armaLoader->normalArray[i+1].z);
            glVertex3f(armaLoader->vertexArray[i+1].x, armaLoader->vertexArray[i+1].y, armaLoader->vertexArray[i+1].z);

            glTexCoord2d(armaLoader->textureArray[i+2].x, armaLoader->textureArray[i+2].y);
            glNormal3f(armaLoader->normalArray[i+2].x, armaLoader->normalArray[i+2].y, armaLoader->normalArray[i+2].z);
            glVertex3f(armaLoader->vertexArray[i+2].x, armaLoader->vertexArray[i+2].y, armaLoader->vertexArray[i+2].z);

        }
    glEnd();
}

/*******************************************************************************************************************
										FUNÇÃO DrawCubo
********************************************************************************************************************/
void DrawCubo(){

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, texture_id[1]);

    glBegin(GL_TRIANGLES);
        for(int i=0; i<cuboLoader->numVertices;i+=3)
        {
            glTexCoord2d(cuboLoader->textureArray[i].x, cuboLoader->textureArray[i].y);
            glNormal3f(cuboLoader->normalArray[i].x, cuboLoader->normalArray[i].y, cuboLoader->normalArray[i].z);
            glVertex3f(cuboLoader->vertexArray[i].x,cuboLoader->vertexArray[i].y, cuboLoader->vertexArray[i].z);

            glTexCoord2d(cuboLoader->textureArray[i+1].x, cuboLoader->textureArray[i+1].y);
            glNormal3f(cuboLoader->normalArray[i+1].x, cuboLoader->normalArray[i+1].y, cuboLoader->normalArray[i+1].z);
            glVertex3f(cuboLoader->vertexArray[i+1].x, cuboLoader->vertexArray[i+1].y, cuboLoader->vertexArray[i+1].z);

            glTexCoord2d(cuboLoader->textureArray[i+2].x, cuboLoader->textureArray[i+2].y);
            glNormal3f(cuboLoader->normalArray[i+2].x, cuboLoader->normalArray[i+2].y, cuboLoader->normalArray[i+2].z);
            glVertex3f(cuboLoader->vertexArray[i+2].x, cuboLoader->vertexArray[i+2].y, cuboLoader->vertexArray[i+2].z);
        }
    glEnd();

}

/*******************************************************************************************************************
										FUNÇÃO DrawGround
********************************************************************************************************************/
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

/*******************************************************************************************************************
										FUNÇÃO DrawSphere
********************************************************************************************************************/
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

/*******************************************************************************************************************
										FUNÇÃO DrawBala
********************************************************************************************************************/
void DrawBala(){

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texture_id[4]);

        glBegin(GL_QUADS);
        for(int i=0; i<balaLoader->numVertices;i+=3)
        {
            glTexCoord2d(balaLoader->textureArray[i].x, balaLoader->textureArray[i].y);
            glNormal3f(balaLoader->normalArray[i].x, balaLoader->normalArray[i].y, balaLoader->normalArray[i].z);
            glVertex3f(balaLoader->vertexArray[i].x, balaLoader->vertexArray[i].y, balaLoader->vertexArray[i].z);

            glTexCoord2d(balaLoader->textureArray[i+1].x, balaLoader->textureArray[i+1].y);
            glNormal3f(balaLoader->normalArray[i+1].x, balaLoader->normalArray[i+1].y, balaLoader->normalArray[i+1].z);
            glVertex3f(balaLoader->vertexArray[i+1].x, balaLoader->vertexArray[i+1].y, balaLoader->vertexArray[i+1].z);

            glTexCoord2d(balaLoader->textureArray[i+2].x, balaLoader->textureArray[i+2].y);
            glNormal3f(balaLoader->normalArray[i+2].x, balaLoader->normalArray[i+2].y, balaLoader->normalArray[i+2].z);
            glVertex3f(balaLoader->vertexArray[i+2].x, balaLoader->vertexArray[i+2].y, balaLoader->vertexArray[i+2].z);

        }
        glEnd();
}
/*******************************************************************************************************************
										FUNÇÃO DrawAxis
********************************************************************************************************************/
void DrawAxis(){

   glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);    // eixo X
        glVertex3f(0.0f, 0.0f, 0.0f);   //vermelho
        glVertex3f(2.0f, 0.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);    // eixo Y
        glVertex3f(0.0f, 0.0f, 0.0f);   //verde
        glVertex3f(0.0f, 2.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 1.0f);    // eixo Z
        glVertex3f(0.0f, 0.0f, -100.0f);   //azul
        glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
}

/*******************************************************************************************************************
										FUNÇÃO light
********************************************************************************************************************/
void light(){
    GLfloat luzAmbiente[4]={0.3,0.3,0.3,1.0};
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


/*******************************************************************************************************************
										FUNÇÃO loadImg
********************************************************************************************************************/
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


/*******************************************************************************************************************
										FUNÇÃO handleKeys
********************************************************************************************************************/
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

				case SDLK_e:
                std::clog<<"Exibe eixos"<<std::endl;
                eixo = !eixo;
                break;

				case SDLK_t:
                std::clog<<"Exibe tempo"<<std::endl;
                texto = !texto;
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
        } else if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			 switch(event.button.button)
             {
				case SDL_BUTTON_LEFT:
				std::clog<<"pressed mouse"<<std::endl;
			 	keyFlags.flagButtonLeft = true;
				trava = true;
				break;

				case SDL_BUTTON_RIGHT:
				std::clog<<"pressed mouse"<<std::endl;
			 	angle_camera = 30.0;
				break;

				default:
				break;
			 }
        } else if(event.type == SDL_MOUSEBUTTONUP)
		{
			 switch(event.button.button)
             {
				case SDL_BUTTON_LEFT:
				std::clog<<"released mouse"<<std::endl;
				keyFlags.flagButtonLeft = false;
				break;

				case SDL_BUTTON_RIGHT:
				std::clog<<"released mouse"<<std::endl;
			 	angle_camera = 55.0;
				break;

				default:
				break;
			 }
        }
    }
}

/*******************************************************************************************************************
										FUNÇÃO display
********************************************************************************************************************/
void display(void){


    glColor3f(1.0,1.0,1.0);


    glViewport(0, 0, ViewPortWidth, ViewPortHeight);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


	gluPerspective(	angle_camera, 						        // angulo de abertura da camera
				  (1.0f * ViewPortWidth)/ViewPortWidth, // aspecto da imagem
				   0.5f, 	   							// near plane
				   6000.0f);  							//far plane


	gluLookAt(	0, 4, 20,	// posição da câmera
				-arma->rotation.y, 0, 0,	// ponto para o qual a camera está olhando
				0.0f, 1.0f, 0.0f);	// vetor "up"

	if(eixo)
		DrawAxis();//desenha os eixos na tela

	//Desenha tempo e acertos na tela
	if(texto){
		std::ostringstream conversor_tempo, conversor_acertos;
        conversor_tempo << "Time: " << tempo_execucao;
		conversor_acertos << "Acertos: " << acertos;
		DrawTexto(conversor_tempo.str() , 850, 630);//Desenha o cronômetro na tela
		DrawTexto(conversor_acertos.str(), 850, 590);//Desenha o contador de tiros na tela
	}

	//Acertou 5 vezes e ganhou o jogo
	//if(acertos == 5){
		//usleep(5000000);//5 segundos
		//running = false;
	//}


    glColor3f(1.0f, 1.0f, 1.0f);
	/**********PARTE PARA O SOLO*************/
	glPushMatrix();
		glScalef(10, 10, 10);
		DrawGround(); //ground
	glPopMatrix();


	/**********PARTE PARA COLISÃO*************/
	glPushMatrix();
		glTranslatef(1, 1, -9); //desloc.x, desloc.y, desloc.z
		DrawCubo();
    glPopMatrix();

	glPushMatrix();
		glTranslatef(-8, 1, -9); //desloc.x, desloc.y, desloc.z
		DrawCubo();
    glPopMatrix();

	/**********PARTE PARA A ARMA*************/
    glPushMatrix();
		glTranslatef(arma->position.x, 0.3, 12.0); //desloc.x, desloc.y, desloc.z
		glRotatef(arma->rotation.y, 0.0f, 1.0f, 0.0f);

		if(keyFlags.flagButtonLeft && trava){
		//if(keyFlags.flagButtonLeft){
			//trava = false;
			glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
			//Mix_PlayChannel(-1, tiro, 1);
				//std::clog<<"MUSICA TOCANDO: "<< Mix_Playing(-1)<< std::endl;
			//Mix_FadeInChannel(1, tiro, 1, 20);
			//Mix_FadeOutChannel(1, 20000);


			//Verifica acerto no cubo 1
			//if(arma->verificaColisao(vertices_ini, vertices_fina)){
			if(arma->position.x <= 0.5  && arma->position.x >= -0.1){
				acertos++;
				std::clog<<"Desenha Bala***********************"<<std::endl;
				glPushMatrix();
					glTranslatef(arma->position.x, 1.8, -4.4); //desloc.x, desloc.y, desloc.z
					glScalef(0.1, 0.1, 0.1);
					DrawBala();
			 	glPopMatrix();
			}

			//Verifica acerto no cubo 2
			if(arma->position.x <= -2.5  && arma->position.x >= -3.0){
				acertos++;
				std::clog<<"Desenha Bala***********************"<<std::endl;
				glPushMatrix();
					glTranslatef(arma->position.x + 2.7, 1.8, -4.4); //desloc.x, desloc.y, desloc.z
					glScalef(0.1, 0.1, 0.1);
					DrawBala();
			 	glPopMatrix();
			}

		}
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		arma->moveArma(keyFlags.flagUp, keyFlags.flagDown, keyFlags.flagLeft, keyFlags.flagRight);
		arma->rodaArma(keyFlags.flagLeft, keyFlags.flagRight);
		DrawArma();
    glPopMatrix();


	/**********PARTE PARA A ESFERA*************/
    glColor3f(1.0,1.0,1.0);
	glPushMatrix();
		glRotatef(angle_sky+=0.02, 0, 1, 0);
		glScalef(20, 20, 20);
		DrawSphere(); //sky
	glPopMatrix();

}

/*******************************************************************************************************************
										FUNÇÃO configuraAudio
********************************************************************************************************************/
void configuraAudio(){

	tiro = Mix_LoadWAV("tiro.wav");
	Mix_VolumeChunk(tiro, 100);

	Mix_AllocateChannels(2);
	Mix_Volume(-1, 100);
}
/*******************************************************************************************************************
										FUNÇÃO main
********************************************************************************************************************/
int main(int argc, char** argv){

    glutInit(&argc, argv);
    std::clog<<"/*********TRYING TO INIT SDL**********/"<<std::endl;
    if(initSDL()){
		//inicializa o som
		//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);//1 mono e 2 stereo
        std::clog<<"SDL Initialised Successfully..."<<std::endl;
    } else {
        std::clog<<"Error initialising SDL "<<SDL_GetError()<<std::endl;
		std::clog<<"Error initialising SDL_Mix "<<Mix_GetError()<<std::endl;
        return 1;
    }
    std::clog<<"/*********TRYING TO INIT SDL**********/"<<std::endl;

    std::clog<<"/*********LOADING MODEL**********/"<<std::endl;
    armaLoader = new Loader();
    cuboLoader = new Loader();
    groundLoader = new Loader();
    sphereLoader = new Loader();
	balaLoader = new Loader();

    cuboLoader->loadScene("Models/cubo.obj");
    armaLoader->loadScene("Models/h1.obj");
    groundLoader->loadScene("Models/ground2.obj");
    sphereLoader->loadScene("Models/sphere.obj");
	balaLoader->loadScene("Models/bala2.obj");

    arma = new Arma();

    std::clog<<"/*********LOADING MODEL**********/"<<std::endl;

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);

    light();
   textures("Models/texth1.jpg", "Models/terrain.png", "Models/dome.png", "Models/woodtexture.png", "Models/firetexture1.png");



    /*****IMAGE*****/
    while(!running){
        loadImg("menu.bmp", window, &running);
    }
    /*****IMAGE*****/

	//Mix_Chunk* boom_snd;
	inicio = time(NULL);//início da contagem do tempo de execução
    while(running)
    {
        handleKeys(&running);
        display();



		fim = time(NULL); //final da contagem do tempo de execução
		tempo_execucao = fim - inicio;


        //Update screen
        SDL_GL_SwapWindow(window);

		//configuraAudio();


    }



    close();

    return 0;
}
