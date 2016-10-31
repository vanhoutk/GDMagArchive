//
//  Curves and Curved Surfaces driver header file
//
//      Brian Sharp, 6/2/98
//      brian_sharp@cognitoy.com
//
//  This file contains prototypes for the functions
//  used by the driver file.  The app* functions are
//  not defined by the driver; the application must
//  implement them.
//

// Used for initialization.
void init();
void appInit(int, char**);

// Used to handle keystrokes.
void key(unsigned char, int, int);
void appKey(unsigned char, int, int);

// Used when drawing each frame.
void draw();
void appDraw();

// Used to load a texture.  256x256, 3-bytes per color (RGB), interleaved .raw file.
int loadTexture( const char* rawFile );

// Used to load a texture.  256x256, 4-bytes per color (RGBA), interleaved .raw file.
int loadAlphaTexture( const char* rawFile );

// It's main.
int main(int, char**);

// This is used so that the application can choose its own window title.
const char* appWindowName();

// This is used to let the application clean up before dying.
void appCleanup();