////////////////////////////////////////////////////////////////////////////
// 
//
// Copyright (c) 2006 Mick West
// http://mickwest.com/
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// HOW TO COMPILE
//
// You should have Visual studio files bude.sln and bude.vcproj which will allow
// you to compile under visual studio 2005.
// if not, then the following info should be enought to get you compiling.
//
// The entire project consists of two source files:
//   bude.cpp (this file)
//   vector2.h  (the 2D vector library)
//
// This code will compile under Visual Studio 2005, including the express 
// edition, available free at:
// http://msdn.microsoft.com/vstudio/express/
//
// You also need the windows SDK
// http://www.microsoft.com/downloads/details.aspx?FamilyId=A55B6B43-E24F-4EA3-A93E-40C0EC4F68E5&displaylang=en
//
// And the DirectX SDK
// http://msdn.microsoft.com/directx/sdk/
//
// Link with the following libraries:
// d3dxof.lib dxguid.lib d3dx9.lib d3d9.lib dxerr9.lib dinput8.lib winmm.lib user32.lib
//  
// You will also need to set the correct include and lib path for the Windows SDK and DirectX SDK.
// these vary by install, but for me were:
//
// C:\Program Files\Microsoft DirectX SDK (December 2005)\Include
// C:\Program Files\Microsoft Platform SDK\Include
// C:\Program Files\Microsoft DirectX SDK (December 2005)\Lib\x86
// C:\Program Files\Microsoft Platform SDK\Lib
//
// Article specific code start around line 2600
//
// Configuration defines, this is as I tested it
// but feel free to mess around with it

//#define	MANUAL_OPT					// some manual optimizations
#define	PRETTY_UP					// Much nicer looking graphics and effect, a bit slower
//#define	NBODY						// all particles affect each other, reduce number of particles.
#define	USE_WORKER_THREADS				// define either of these to enable threading
//#define USE_FORKED_THREADS
//#define SET_AFFINITY
#define PROCESS_CHUNKS
#define	MAX_THREAD	1					// any number you like, but typically 1,2,4 or 8 will work best
//#define	USE_DUMMY_PHYSICS			// compile in a full tilt thread
//#define	USE_FORK_MARKING			// fork/join a quick thread at the end of the update as a marker
//#define	CRITICAL_SECTION_RND		// Not strictly necessary, so off for speed



#ifdef NBODY
const int	NUM_PARTICLES = 1000;		// we have NxN interactions, so reduce number of particles.
#else
const int	NUM_PARTICLES = 80000;  // 80000 is good on a 3.2ghz DC/HT.  
									 // But you can go up to 240000, which is quite impressive
#endif

// Checking optimizations:
//
// FP Model:
// Precise:				0.0214  << DEFAULT BASELINE
// Strict:				0.0253
// Fast:				0.0182  *** (15% faster)
//
// Exceptions -			No diff                          
//
// SIMD 2				0.0208  ***
//
// Opt Minimize size	0.0250
// Opt Full				unch

// Intrinsics			unch
//
// link time code gen	0.0206  ***
//
// RTTI off				unch
//
// Calling conv fastcall	unch

// All three Fast FP, SIMD2, Link time code
// =   0.0095
// with four threads:	0.0036, 0.0138


#pragma warning(disable: 4995)          // don't warn about deprecated functions
#define _CRT_SECURE_NO_DEPRECATE		// ditto

// Hooks into the simple sample
void MX_Init();
void MX_Render();
void MX_Logic(float time);
void MX_Cleanup();

void    debug_log( const char* text, ...);

const int   g_viewport_width = 1024;
const int   g_viewport_height = 768;

bool    g_resize = false;

// System and DirectX includes.
#include <d3d9.h>
#include <d3dx9.h>
#include <strsafe.h>
#include <math.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// local includes
#include "vector2.h"

bool	dragging;
int		mouse_x,mouse_y;



class CParticle
{

public:
		Vector2	m_old_pos;
		Vector2	m_pos;
		Vector2	m_vel;
		DWORD	m_color;
		void	Update(float time);

// later		void	Update(float t);
};

class	CParticleManager
{
public:
		CParticleManager();
		~CParticleManager();
		void Init(int n);
		void Update(float time);
		void Render();
//private:
		CParticle	*mp_particles;
		int			m_num_particles;
};

CParticleManager	g_ParticleManager;


#ifdef		CRITICAL_SECTION_RND
CRITICAL_SECTION	rnd_CS;
#endif

////
// since rand is weak, I implement my own random numbers

unsigned int	rnd()				   
{

#ifdef		CRITICAL_SECTION_RND
	EnterCriticalSection(&rnd_CS);
#endif
	static int rnd_a = 12345678;
	static int rnd_b = 12393455;
	static int rnd_c = 45432838;	   

	rnd_a = rnd_a ^ 0x10010100;
	rnd_a = (rnd_a << 1) | ((rnd_a>> 31)&1);
	rnd_a ^= rnd_b ^ rnd_c;
	rnd_b = rnd_b * 255 + 32769;
	rnd_c = rnd_a + rnd_b + rnd_c + 1;
	unsigned int return_value = rnd_a;
#ifdef		CRITICAL_SECTION_RND
	LeaveCriticalSection(&rnd_CS);
#endif
	return return_value;
}

// return random number in the range 0 .. a-1 
int rnd (unsigned int a)
{
	return rnd() % a;
}


//-----------------------------------------------------------------------------
// Function-prototypes for directinput handlers
//-----------------------------------------------------------------------------
BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
HRESULT InitDirectInput( HWND hDlg );
VOID    FreeDirectInput();
HRESULT UpdateInputState( HWND hDlg );


LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB2       = NULL; // Another Buffer to hold Vertices for lines
ID3DXFont*              g_pFont      = NULL;

RECT                    window_rect;
int                     g_window_width,g_window_height;

inline float  scale_x(float x) {return x * (float)g_window_width / (float) g_viewport_width;}
inline float  scale_y(float y) {return y * (float)g_window_height / (float) g_viewport_height;}

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

static 	LARGE_INTEGER Freq;
static 	LARGE_INTEGER BaseTime;
static 	LARGE_INTEGER PausedTime;
static 	LARGE_INTEGER UnPausedTime;


void Timer_Reset()
{
	QueryPerformanceCounter(&BaseTime);
}

void Timer_Init()
{
	QueryPerformanceFrequency(&Freq);
	Timer_Reset();
}

float Timer_Seconds()
{
	static 	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	return (float)(Time.QuadPart-BaseTime.QuadPart)/(float)(Freq.QuadPart);
}

void Timer_Pause()
{
	QueryPerformanceCounter(&PausedTime);
}
void Timer_Resume()
{
	QueryPerformanceCounter(&UnPausedTime);
    // adjust the base time by the time we have been paused 
    BaseTime.QuadPart += (UnPausedTime.QuadPart - PausedTime.QuadPart); 
}

// Buffers for lines and triangles 
CUSTOMVERTEX *g_pTriVerts;
const int MAX_TRIS=500000;
int	g_nTris = 0;

CUSTOMVERTEX *g_pLineVerts;
const int MAX_LINES=500000;
int	g_nLines = 0;

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    if ( NULL == g_pD3D)
    {
        // Create the D3D object.
        if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
            return E_FAIL;
    }

    // We might be re-creating the font and device, so release them if so.
    if ( g_pFont != NULL )
    {
      g_pFont->Release();     
      g_pFont = NULL;
    }
    if( g_pd3dDevice != NULL ) 
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
    
    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
    d3dpp.BackBufferWidth = g_window_width;
	d3dpp.BackBufferHeight = g_window_height;
    
	d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //D3DPRESENT_INTERVAL_ONE;
    //d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }
    
               
    if( FAILED( D3DXCreateFont( g_pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         (LPCSTR)"Arial", &g_pFont ) ) )
    {
        return E_FAIL;
    }


    return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
// DirectX Joystick handling starts here:

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8       g_pDI              = NULL;         
LPDIRECTINPUTDEVICE8 g_pJoystick        = NULL;     

#define JOY_BUFFERSIZE  64
DIDEVICEOBJECTDATA   g_inputbuffer[JOY_BUFFERSIZE];        // Input buffer for joystick events


//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT InitDirectInput( HWND hDlg )
{
    HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;

    // Look for a simple Joystick we can use for this sample program.
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;                                            

    // Make sure we got a Joystick
    if( NULL == g_pJoystick )
    {
      //  MessageBox( NULL, TEXT("Joystick not found."),  
      //              TEXT("DirectInput Sample"), 
      //              MB_ICONERROR | MB_OK );
      //  EndDialog( hDlg, 0 );
		// Not too worried about this for this application      
		return S_OK;
    }

    // Set the data format to "simple Joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | 
                                                             DISCL_FOREGROUND ) ) )
        return hr;

    // Enumerate the Joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, 
                                                (VOID*)hDlg, DIDFT_ALL ) ) )
        return hr;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated Joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;

    // Obtain an interface to the enumerated Joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

    DIDEVICEINSTANCE    device_info;

    g_pJoystick->GetDeviceInfo(&device_info);

    // If it failed, then we can't use this Joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first Joystick we get. You
    // could store all the enumerated Joysticks and let the user pick.
    return DIENUM_STOP;
    
    // Continue Enumeration, let's pick the best joystick
//    return DIENUM_CONTINUE; 
      
}

//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       Joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;


        DIPROPDWORD  dipdw; 
        dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        dipdw.diph.dwObj = 0; 
        dipdw.diph.dwHow = DIPH_DEVICE; 
        dipdw.dwData = JOY_BUFFERSIZE; 

        // Set the size of the data buffer
        if( FAILED( g_pJoystick->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) ) )
            return DIENUM_STOP;
         
    }
    
    return DIENUM_CONTINUE;

}

DIJOYSTATE2 js;           // DInput Joystick state 


//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pJoystick ) 
        g_pJoystick->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( g_pJoystick );
    SAFE_RELEASE( g_pDI );
}


///////////////////////////////////////////////////////////////////////////
// ReadControllerState is an interface function between my code
// and the DirectInput framework
// takes a 256 entry byte array
// returns with the first 12 entries being the gamepad buttons
// in the order X,A,B,Y,L1,R1,L2,R2,SELECT,START,L3,R3
// and entries 128,129,130,132 being U,D,L,R
void ReadControllerState(unsigned char *buttons)
{
    // copy over the first 128 buttons
    for (int i=0;i<128;i++)
    buttons[i] = js.rgbButtons[i];
    // and create remaining buttons from the D-Pad direction
    
    // POV in directX correspond to the DPad    
    // -1 = nothing
    // anything else = angle in degrees, time 100, so, 9000 for right. 22500 for down left

    // This is a rather odd mapping, but it works    
    switch (js.rgdwPOV[0])
    {
        case -1:
            buttons[128] = 0;
            buttons[129] = 0;
            buttons[130] = 0;
            buttons[131] = 0;
            break;
        case 0:
            buttons[128] = 1;
            buttons[129] = 0;
            buttons[130] = 0;
            buttons[131] = 0;
            break;
        case 4500:
            buttons[128] = 1;
            buttons[129] = 0;
            buttons[130] = 0;
            buttons[131] = 1;
            break;
        case 9000:
            buttons[128] = 0;
            buttons[129] = 0;
            buttons[130] = 0;
            buttons[131] = 1;
            break;
        case 13500:
            buttons[128] = 0;
            buttons[129] = 1;
            buttons[130] = 0;
            buttons[131] = 1;
            break;
        case 18000:
            buttons[128] = 0;
            buttons[129] = 1;
            buttons[130] = 0;
            buttons[131] = 0;
            break;
        case 22500:
            buttons[128] = 0;
            buttons[129] = 1;
            buttons[130] = 1;
            buttons[131] = 0;
            break;
        case 27000:
            buttons[128] = 0;
            buttons[129] = 0;
            buttons[130] = 1;
            buttons[131] = 0;
            break;
        case 31500:
            buttons[128] = 1;
            buttons[129] = 0;
            buttons[130] = 1;
            buttons[131] = 0;
            break;
    }
}


// End of DirectX joystick handling code
/////////////////////////////////////////////////////////////////////////////////

const int max_strings = 200;
const int max_string_length = 255;
struct SDrawText {
    float x,y;
    char text[max_string_length+1];
    DWORD color;
};

int num_draw_strings;

SDrawText   texts_to_draw[max_strings];
void    DrawString(float x, float y, const char *p_text, DWORD color = 0xff000000)
{
    if (num_draw_strings == max_strings)
        return;
    texts_to_draw[num_draw_strings].x = scale_x(x);    
    texts_to_draw[num_draw_strings].y = scale_y(y);    
    texts_to_draw[num_draw_strings].color = color;
    strncpy(texts_to_draw[num_draw_strings].text,p_text,max_string_length);    
    texts_to_draw[num_draw_strings].text[max_string_length]='\0'; // NULL terminator for iff p_text is >255 chars
    num_draw_strings++;
    
}


//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our Vertices. The vertex
//       buffer is basically just a chuck of memory that holds Vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{

	g_pTriVerts = new CUSTOMVERTEX[MAX_TRIS * 3];
	g_nTris = 0;
	g_pLineVerts = new CUSTOMVERTEX[MAX_LINES * 2];
	g_nLines = 0;

    // Create the vertex buffer.  We also
    // specify the FVF, so the vertex buffer knows what data it contains.
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( MAX_TRIS*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( MAX_LINES*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB2, NULL ) ) )
    {
        return E_FAIL;
    }


    return S_OK;
}


HRESULT FillVB()
{
	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the Vertices. This mechanism is required becuase vertex
    // buffers may be in device memory.
    VOID* pVertices;
    if( FAILED( g_pVB->Lock( 0, g_nTris * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_pTriVerts, g_nTris * sizeof(CUSTOMVERTEX) );
    g_pVB->Unlock();

    // Repeat for lines    
    if( FAILED( g_pVB2->Lock( 0, g_nLines * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_pLineVerts, g_nLines * sizeof(CUSTOMVERTEX) );
    g_pVB->Unlock();
	
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pVB != NULL )        
        g_pVB->Release();
    if( g_pVB2 != NULL )        
        g_pVB2->Release();
        
    if ( g_pFont != NULL )
      g_pFont->Release();     

    if( g_pd3dDevice != NULL ) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )       
        g_pD3D->Release();

	if (g_pTriVerts != NULL)
		delete g_pTriVerts;
	if (g_pLineVerts != NULL)
		delete g_pLineVerts;
        
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    // Clear the backbuffer to a neutral color
    //g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(180,180,200), 1.0f, 0 );
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {

        MX_Render();
        
        // Refill the VB, allowing us to draw whatever
		FillVB();
		
		// Draw the triangles in the vertex buffer. This is broken into a few
        // steps. We are passing the Vertices down a "stream", so first we need
        // to specify the source of that stream, which is our vertex buffer. Then
        // we need to let D3D know what vertex shader to use. Full, custom vertex
        // shaders are an advanced topic, but in most cases the vertex shader is
        // just the FVF, so that D3D knows what type of Vertices we are dealing
        // with. Finally, we call DrawPrimitive() which does the actual rendering
        // of our geometry (in this case, just one triangle).
        g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, g_nTris/3 );

        // And the lines
        g_pd3dDevice->SetStreamSource( 0, g_pVB2, 0, sizeof(CUSTOMVERTEX) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
        g_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, g_nLines/2 );

        RECT rc;
        for (int i=0;i<num_draw_strings;i++)
        {
            SetRect( &rc, (int)texts_to_draw[i].x, (int)texts_to_draw[i].y, 0, 0 );        
            g_pFont->DrawText(NULL, (LPCSTR)texts_to_draw[i].text, -1, &rc, DT_NOCLIP, texts_to_draw[i].color);
        }
        num_draw_strings = 0;

        // and reset
        g_nTris = 0;
        g_nLines = 0;

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Windows application initialization and message handling
// Based on DirectX SDK example applications



//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "ButtonDisambiguation", NULL };
    RegisterClassEx( &wc );


    // Create the application's window
	HWND hWnd = CreateWindow( "ButtonDisambiguation", "Mick West: Particle Systems Threaded Example",
                              WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );
                              
	// Get the size of the renderable rectangle
	GetClientRect(hWnd,&window_rect);
    g_window_width = window_rect.right-window_rect.left;
	g_window_height = window_rect.bottom-window_rect.top;

 //   debug_log("ClientRect Window = %dx%d",g_window_width,g_window_height);
                              


    if( FAILED( InitDirectInput( hWnd ) ) )
    {
        MessageBox( NULL, TEXT("Error Initializing DirectInput"), 
                    TEXT("Button Disambiguation Example"), MB_ICONERROR | MB_OK );
    }

    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        if( SUCCEEDED( InitVB() ) )
        {
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            MX_Init();

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof(msg) );
            Timer_Init();
            while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                {
                    float   start = Timer_Seconds();
                    UpdateInputState( hWnd );
                    MX_Logic(Timer_Seconds());
                    Render();
                    //while (Timer_Seconds() > start && Timer_Seconds() < start + 4.0f * 0.016666f)
                    {
                        // Waiting for a frame to elapse, so we go to 60 fps
                    }
                        
                }
            }            
            MX_Cleanup();
        }
    }

    UnregisterClass( "ButtonDisambiguation", wc.hInstance );
    return 0;
}

// end of Windows Stuff

// The following function uses the directX wrapper stuff directly

// triangle with clockwise ordered points
void DrawTri(float x0,float y0,float x1,float y1,float x2,float y2, DWORD color)
{
    if (g_nTris > MAX_TRIS-3)
    {
        // Error - run out of triangle buffer
    }

	g_pTriVerts[g_nTris+0].x = scale_x(x0);
	g_pTriVerts[g_nTris+0].y = scale_y(y0);
	g_pTriVerts[g_nTris+0].z = 0.5f;
	g_pTriVerts[g_nTris+0].rhw = 1.0f;
	g_pTriVerts[g_nTris+0].color = color;

	g_pTriVerts[g_nTris+1].x = scale_x(x1);
	g_pTriVerts[g_nTris+1].y = scale_y(y1);
	g_pTriVerts[g_nTris+1].z = 0.5f;
	g_pTriVerts[g_nTris+1].rhw = 1.0f;
	g_pTriVerts[g_nTris+1].color = color;

	g_pTriVerts[g_nTris+2].x = scale_x(x2);
	g_pTriVerts[g_nTris+2].y = scale_y(y2);
	g_pTriVerts[g_nTris+2].z = 0.5f;
	g_pTriVerts[g_nTris+2].rhw = 1.0f;
	g_pTriVerts[g_nTris+2].color = color;
    
    g_nTris+=3;    
    
}

void DrawLine2(float x0,float y0,float x1,float y1, DWORD color0, DWORD color1)
{
    if (g_nLines > MAX_LINES-2)
    {
        // Error - run out of line buffer
    }
	g_pLineVerts[g_nLines+0].x = scale_x(x0);
	g_pLineVerts[g_nLines+0].y = scale_y(y0);
	g_pLineVerts[g_nLines+0].z = 0.5f;
	g_pLineVerts[g_nLines+0].rhw = 1.0f;
	g_pLineVerts[g_nLines+0].color = color0;
                                         
	g_pLineVerts[g_nLines+1].x = scale_x(x1);
	g_pLineVerts[g_nLines+1].y = scale_y(y1);
	g_pLineVerts[g_nLines+1].z = 0.5f;
	g_pLineVerts[g_nLines+1].rhw = 1.0f;
	g_pLineVerts[g_nLines+1].color = color1;
    
    g_nLines+=2;    
}


void DrawLine(float x0,float y0,float x1,float y1, DWORD color)
{
    DrawLine2(x0,y0,x1,y1,color,color);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Basic 2d Primitive rendering code - Triangles and Lines


void DrawLine(Vector2 start , Vector2 end, DWORD color)
{
    DrawLine(start.x, start.y, end.x, end.y,color );
}


void DrawX(Vector2 pos, float size, DWORD color)
{
    DrawLine(pos.x-size, pos.y-size, pos.x+size, pos.y+size,color);
    DrawLine(pos.x-size, pos.y+size, pos.x+size, pos.y-size,color);
}

// Draw a quad with four points in clockwise order
void DrawQuad(float x0,float y0,float x1,float y1,float x2,float y2, float x3, float y3, DWORD color)
{
	DrawTri(x0,y0,x1,y1,x2,y2, color);
	DrawTri(x2,y2,x3,y3,x0,y0, color);
}

// Draw an axis aligned rectangle
void DrawRect(float x, float y, float w, float h, DWORD color)
{
	DrawQuad(x,y,x+w,y,x+w,y+h,x,y+h,color);
}

///////////////////////////////////////////////////////////////////////////////////
// World handling code
// handles rendering and collision detection 
// The world is a very simple 2D map, 80 blocks wide by 60 high


unsigned char world[] = 
"11111111111111111111111111111111111111111111111111111111111111111111111111111111"
"1                                                                              1"
"1                                                                              1"
"1                               11111111111111111111                           1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                   111111111111111111         1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                222222222222221"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                            22222222222                                       1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1        22222222                                                              1"
"1        22222222                                   11111111                   1"
"1        22222222                                                              1"
"1        22222222                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"111111111111111111111                                    11111111111111111111111"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                        11111111111111111111111"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1                                                                              1"
"1111111111111111111111                      111111111111111111111111111111111111"
"1                                                                              1"
"1                                                                              3"
"1                                                                              3"
"1                                                                              3"
"1                                                                              3"
"11111111         1111111111111111111111111111111111111111111111111             3"
"1                                                                         111113"
"1                                                                         111113"
"1                                                                         111113"
"1                                                                         111113"
"1                                                                         111113"
"1          22222                    1111111                            111111113"
"1          22222                    1111111                            111111113"
"1          22222                    1111111                            111111113"
"1          22222                    1111111                            111111113"
"1          22222                    1111111                            111111113"
"11111111111111111111111111111111111111111111111111111111111111111111111111111111";

const int world_width   = 80;
const int world_height  = 60;

DWORD   get_color_from_cell(unsigned char cell)
{
    switch (cell)
    {
        case ' ':
            return 0;          // space = transparent
        case '1':
            return 0xff804040;          // 1 = lt red
        case '2':
            return 0xff408040;          // 2 = lt green
        case '3':
            return 0xff404040;          // 3 = dark grey

    default:
            return  0xffff00ff;         // default = magenta;
        
    }
}


void RenderWorld()
{
    unsigned char *p_world = world;
    float cell_width = (float)g_viewport_width/(float)(world_width);
    float cell_height = (float)g_viewport_height/(float)(world_height);
    
    for (int y=0;y<world_height;y++)
    {
        for (int x=0;x<world_width;x++)
        {
            unsigned char cell = *p_world++;
            DWORD color = get_color_from_cell(cell);
            if (color != 0)
            {
                DrawRect(x*cell_width,y*cell_height,cell_width,cell_height,color);
            }
        }
    }
}

// Just tells you if a point in world coordinates is inside a cell that is solid.
bool    WorldCollisionAt(Vector2 &pos)
{
    if (pos.x < 0.0f || pos.x >= g_viewport_width || pos.y < 0.0f || pos.y >= g_viewport_height)
    {
        return true;
    }

    float cell_width = (float)g_viewport_width/(float)(world_width);
    float cell_height = (float)g_viewport_height/(float)(world_height);
    int row = (int) (pos.y / cell_height);
    int col = (int) (pos.x / cell_width);
    return (world[row*world_width + col] != ' ');
}


////////////////////////////////////////////////////////////////////////////////////
// CWhisker - a class for a line/world collision detection
// A whisker is a lin in world space, you set the line start end end points
// and then call CheckCollision to see if there is a collision there
// if there is, then you can furthur query it to get the collision point, and the 
// vector for the unit vector of the surface we collided with
// this just uses our rather simple cell based world for now
// 
// The Whisker class is currently hard wired into the simple cell based world
// but the interface is independent of this, so if the world changes
// then we should easily be able to fix this without breaking anything.

class CWhisker
{
    public:
                    CWhisker();
                    CWhisker(Vector2 &start, Vector2 &end);
        void        Set(Vector2 &start, Vector2 &end);        
        bool        CheckCollision();
        bool        CheckCollision(Vector2 &start, Vector2 &end);        
        Vector2     GetNormal() {return m_normal;}
        Vector2     GetPoint()  {return m_point;}
        Vector2     GetStart()  {return m_start;}
        Vector2     GetEnd()  {return m_end;}
    private:
        // Input data
        Vector2     m_start, m_end;
        // Output/Calculated Data
        Vector2     m_point, m_normal;
};

CWhisker::CWhisker()
{
    m_start.x = 0.0f;
    m_start.y = 0.0f;
    m_end.x = 1.0f;
    m_start.y = 1.0f;
}

CWhisker::CWhisker(Vector2 &start, Vector2 &end)
{
    m_start = start;
    m_end = end;
}

void CWhisker::Set(Vector2 &start, Vector2 &end)
{
    m_start = start;
    m_end = end;
}


bool CWhisker::CheckCollision()
{

    float cell_width = (float)g_viewport_width/(float)(world_width);
    float cell_height = (float)g_viewport_height/(float)(world_height);
    
    // Okay, we know the line starts in the clear
    // so we need to trace along the line, checking all the cells along the way
    // and see if we hit one of them
    // we do this seperatly for X 
    //  - find how many column edges we cross
    //  - iterate over them
    //    - find the cell from line intersection
    //    - if the cell is occupied
    //    - we have a collision
    // repeat for Y
    // return the collision closest to m_start

    bool    found_x = false;                                      
    Vector2 point_x;
    Vector2 normal_x;
    int xdir = (m_end.x > m_start.x) ? 1 : -1 ;                 // which direction is the line goin in, right or left
    int col_start = (int) (m_start.x / cell_width);             // map column to start in
    int col_end   = (int) (m_end.x / cell_width);               // and end in
    for (int col_test = col_start+xdir; col_test != col_end+xdir; col_test+=xdir)
    {
        // col_test is the column the line is now going into
        float x = col_test * cell_width;
        // if going left, then we want to be on the right side of this cell
        if (xdir < 0.0f) { x+=cell_width; }
        // we use a simple similar triangles line collision to find the y point
        float y = m_start.y + (x-m_start.x)*(m_end.y-m_start.y)/(m_end.x-m_start.x);
        int row = (int) (y / cell_height);
        unsigned char cell = world[row*world_width + col_test];
        if (cell != ' ')
        {
            found_x = true;
            point_x = Vector2(x,y);                
            normal_x = Vector2((float)-xdir, 0.0f);
            break;         
        }
    }

    // repeate exactly the same for y, just changing x/y w/h row/col senses.
    bool    found_y = false;                                      
    Vector2 point_y;
    Vector2 normal_y;
    int ydir = (m_end.y > m_start.y) ? 1 : -1 ;
    int row_start = (int) (m_start.y / cell_height);
    int row_end   = (int) (m_end.y / cell_height);
    for (int row_test = row_start+ydir; row_test != row_end+ydir; row_test+=ydir)
    {
        // row_test is the row the line is now going into
        float y = row_test * cell_height;
        // if going up, then we want to be on the bottom side of this cell
        if (ydir < 0.0f) { y+=cell_height; }
        // we use a simple similar triangles line collision to find the x point
        float x = m_start.x + (y-m_start.y)*(m_end.x-m_start.x)/(m_end.y-m_start.y);
        int col = (int) (x / cell_width);
        unsigned char cell = world[row_test*world_width + col];
        if (cell != ' ')
        {
            found_y = true;
            point_y = Vector2(x,y);                
            normal_y = Vector2(0.0f, (float)-ydir);
            break;         
        }
    }

    // Return the found collision which is closes to the start point, if any.
    if (found_x)
    {
        if (found_y && (point_y - m_start).Length2() < (point_x - m_start).Length2())  
        {
            m_point = point_y;
            m_normal = normal_y;
            return true;
        }
        else
        {
            m_point = point_x;
            m_normal = normal_x;
            return true;
        }
    }
    else
    {
        if (found_y)
        {
            m_point = point_y;
            m_normal = normal_y;
            return true;
         }
    }
    
    // not found anything - m_point and m_normal will be undefined.
    return false;       
}

bool CWhisker::CheckCollision(Vector2 &start, Vector2 &end)
{
    Set(start,end);
    return CheckCollision(); 
}

///////////////////////////////////////////////////////////////////////////////

class   CPadButton
{
    public:
        CPadButton();
        ~CPadButton();
        // The UpdateState function is called by the manager to update the state
        // based on hardware specific implementation
        // passing in the button 
        void     UpdateState(bool pressed, float current_time);
    
    
// The simplest implementation of a button would just return if it is pressed right now        
        bool    Pressed() {return m_pressed;}
// More involved would tell you when it was pressed         
        float   PressedTime() {return m_pressed_time;}
        float   ReleasedTime() {return m_released_time;}
// Even more, detecting a "Trigger", and being able to clear it after using it        
        bool    Triggered() {return m_triggered;};
        void    ClearTrigger() {m_triggered = false;}
        bool    Released() {return m_released;}
        void    ClearReleased() {m_released = false;} 
//private:
        bool    m_pressed;
        bool    m_triggered;
        bool    m_released;
        float   m_pressed_time;
        float   m_released_time;
        char    *mp_name;
};

CPadButton::CPadButton()
{
//    g_pWatchManager->SetWatch(&m_pressed);    
    m_pressed = false;
    m_triggered = false;
    m_released = false;
}

CPadButton::~CPadButton()
{
//    delete  mp_watch;
}

void     CPadButton::UpdateState(bool pressed, float current_time)
{

    if (pressed)
    {
        if (!m_pressed)
        {
            debug_log("%4.3f: + Pressed %s",current_time, mp_name);
            m_triggered = true;
            m_pressed_time = current_time;
        }
    }
    else
    {
        if (m_pressed)
        {
            debug_log("%4.3f: - Released %s",current_time, mp_name);
            m_released = true;
            m_released_time = current_time;
        }
    }
    m_pressed = pressed;
}

class   CGamepad
{
    public:
        CGamepad();
        // We are just going to store an array of buttons
        // so we give each one a name so we can refer to it.
        enum    EButton
        {
            BTN_A,
            BTN_B,
            BTN_X,
            BTN_Y,
            BTN_U,
            BTN_D,
            BTN_L,
            BTN_R,
            BTN_L1,
            BTN_R1,
            BTN_L2,
            BTN_R2,
            BTN_START,
            BTN_SELECT,
            BTN_L3,
            BTN_R3,
            
            
            NUM_BUTTONS
        };
        
        CPadButton  button[NUM_BUTTONS];
    
        void        Update( float time);

    private:
    
        
};

char * p_names[] =  {"A","B","X","Y","Up","Down","Left","Right","L1","R1","L2","R2","START","SELECT","L3","R3" };

CGamepad::CGamepad()
{
    for (int i=0;i<NUM_BUTTONS;i++)
    {
        button[i].mp_name = p_names[i];
    }
}


void    CGamepad::Update(float time)
{

    unsigned char buttons[256];
    ReadControllerState(buttons);      // Hook into the framework to get the controller state                
                   
    // For each button, update with the relevent state from the DirectInput joystick
    // (the ?true:false is to avoid the compiler's "performance warning" by making the conversion explicit
    
    button[BTN_X].UpdateState(buttons[0]?true:false, time);
    button[BTN_Y].UpdateState(buttons[3]?true:false, time);
    button[BTN_A].UpdateState(buttons[1]?true:false, time);
    button[BTN_B].UpdateState(buttons[2]?true:false, time);
    button[BTN_L1].UpdateState(buttons[4]?true:false, time);
    button[BTN_R1].UpdateState(buttons[5]?true:false, time);
    button[BTN_L2].UpdateState(buttons[6]?true:false, time);
    button[BTN_R2].UpdateState(buttons[7]?true:false, time);
    button[BTN_SELECT].UpdateState(buttons[8]?true:false, time);
    button[BTN_START].UpdateState(buttons[9]?true:false, time);
    button[BTN_L3].UpdateState(buttons[10]?true:false, time);
    button[BTN_R3].UpdateState(buttons[11]?true:false, time);
    button[BTN_U].UpdateState(buttons[128]?true:false, time);
    button[BTN_D].UpdateState(buttons[129]?true:false, time);
    button[BTN_L].UpdateState(buttons[130]?true:false, time);
    button[BTN_R].UpdateState(buttons[131]?true:false, time);
}

CGamepad                    * g_pGamepad;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// CWatch - Records the state of a particular variable
// this is a debugging class, and would not be used in production code
class   CWatch
{
    friend class    CWatchManager;
                 
                 CWatch();
    void        SetWatch(float *p);
    void        SetWatch(int *p);
    void        SetWatch(char *p);
    void        SetWatch(unsigned char *p);
    void        SetWatch(bool *p);
    void        Update(float time);
    void        Render(float pos_x, float pos_y, float width, float height, float border, float period, float left_area, float time, float time_off );
    
    private:
        enum    EWatchType {
            WATCH_CHAR,
            WATCH_INT,
            WATCH_FLOAT,
            WATCH_BOOL,
        };    
    struct SWatchValue{
        float time;
        union {
            int             i;
            float           f;   
        };
    };  
    
    void init(void*p, EWatchType type);
            
    int get_1_0_at_index(int index);

    const static int   max_watch = 1024;
    
    SWatchValue     m_values[max_watch];       //  MEMOPT!!!   1024*8  8K hmm
    int             m_value_index;
    int             m_num_values;
    void          * mp_watch;       // The address to watch
    EWatchType      m_type;

    DWORD           m_color;
    const char *    mp_desc;
    CWatch  *       mp_next;        // next one in the list

};

CWatch::CWatch()
{
    mp_next = NULL;
}

void CWatch::init(void*p, EWatchType type)
{
    m_value_index   = 0;
    m_num_values    = 0;
    mp_watch        = p;
    m_type          = type;
}

void        CWatch::SetWatch(float *p)
{
    init((void*)p, WATCH_FLOAT);
}

void        CWatch::SetWatch(int *p)
{
    init((void*)p, WATCH_INT);
}

void        CWatch::SetWatch(char *p)
{
    init((void*)p, WATCH_CHAR);
}

void        CWatch::SetWatch(unsigned char *p)
{
    init((void*)p, WATCH_CHAR);
}

void        CWatch::SetWatch(bool *p)
{
    init((void*)p, WATCH_BOOL);
}

void        CWatch::Update(float time)
{
// in theory we could just store it when it changes
// but for now it's simpler just to store all the entries.
    m_values[m_value_index].time = time;
    switch (m_type)
    {
        case WATCH_CHAR:
            m_values[m_value_index].i = *(char*)mp_watch;
            break;
        case WATCH_INT:
            m_values[m_value_index].i = *(int*)mp_watch;
            break;
        case WATCH_FLOAT:
            m_values[m_value_index].f = *(float*)mp_watch;
            break;
        case WATCH_BOOL:
            m_values[m_value_index].i = *(bool*)mp_watch;
            break;
    }
    // increment the buffer and wrap around
    m_value_index++;                // m_value index will point to the entry AFTER the latest valid entry
    if (m_value_index == max_watch) m_value_index = 0;
    if (m_num_values<max_watch) m_num_values++;       // counts up to maximum.
}

// get the value at a particular index and convert it to 1 or 0
int CWatch::get_1_0_at_index(int index)
{
    switch (m_type)
    {
        case WATCH_INT:
        case WATCH_CHAR:
        case WATCH_BOOL:
            return m_values[index].i == 0 ? 0 : 1;
        case WATCH_FLOAT:
            return m_values[index].f == 0.0f ? 0 : 1;
    }
    return 0;
}


void CWatch::Render(float pos_x, float pos_y, float width, float height, float border, float period, float left_area, float time, float time_off )
{

    float   x = width;
    int index = m_value_index;
    int num = m_num_values;
    
    index--;
    if (index <0) index = max_watch-1;

    float   prev_time = time;
    int     prev_value = get_1_0_at_index(index);

    DrawRect(pos_x-border,pos_y-border, width+border*2 + left_area ,height+border*2,0xffffffff);
    DrawString(pos_x, pos_y, mp_desc, m_color);
    
    while (x > 0.01f && num > 2)
    {
        int index_value = get_1_0_at_index(index);
        float   index_time = m_values[index].time + time_off;                
        x = width - width*(time-prev_time)/period;
        float xp = width - width*(time-index_time)/period;
        if (x<0) x = 0;
        if (xp<0) xp = 0;
        DrawLine(left_area+pos_x+x ,pos_y+height*(1-index_value) , left_area+pos_x+xp , pos_y+height*(1-index_value),m_color);              
        DrawLine(left_area+pos_x+x,pos_y+height*(1-prev_value)     , left_area+pos_x+x    , pos_y+height*(1-index_value),m_color);              
        num--;
        index--;
        if (index <0) index = max_watch-1;
        prev_time = index_time;
        prev_value = index_value;
    }
    

}

// End of CWatch
/////////////////////////////////////////////////////////////////////////////

class   CWatchManager
{
public:    
    CWatchManager();
    void        Update(float time);
    void        Render(float time, bool paused);
    void        SetWatch(float *p, DWORD color, const char*c); 
    void        SetWatch(int *p, DWORD color, const char*c); 
    void        SetWatch(char *p, DWORD color, const char*c);
    void        SetWatch(unsigned char *p, DWORD color, const char*c);
    void        SetWatch(bool *p, DWORD color, const char*c);

    void        RecordEvent(int event, float time, char *desc, DWORD color);

    void        ToggleEventDisplay() {m_display_events = !m_display_events;}
                                    
private:
    CWatch *    add_watch(DWORD color, const char *desc);    
    CWatch *    mp_head;

    struct SWatchEvent{
        int         event;
        float       time;
        char        desc[64];                   //  MEMOPT!!!
        DWORD       color;
        float y;
    };  
    
    const static int   max_watch_event = 1024;
    SWatchEvent     m_events[max_watch_event];        // 80 K
    int             m_event_index;
    int             m_num_events;
    float           m_last_event_time;
    float           m_last_event_y;

    float           m_tick_time;
    float           m_last_tick_time;

    bool            m_display_events;
    
};


CWatchManager           * g_pWatchManager;


CWatchManager::CWatchManager()
{
    mp_head = NULL;
    m_event_index = 0;
    m_num_events = 0;
    m_last_event_time =0;
    m_last_event_y =0.0f;
    m_display_events = true;
}

void        CWatchManager::RecordEvent(int event, float time, char *desc, DWORD color)
{

//    if (event >= 0) debug_log("%4.3f: Event %s",time, desc);

    float   event_y = 12.0f;

    // For the "tick" event, record the length of the tick, for later dispay
    if (event == -1)
    {
        m_tick_time = time - m_last_tick_time;
        m_last_tick_time = time;
    }
    else
    // for regular events
    {
        // alternate the height of the event text if it is close to the previous event    
        if (time - m_last_event_time < 0.50)
        {   
            event_y = 12.0f - m_last_event_y;
        }
        m_last_event_time = time;
        m_last_event_y = event_y;
    }
        
    m_events[m_event_index].event = event;
    m_events[m_event_index].time = time;
    m_events[m_event_index].y = event_y;
    m_events[m_event_index].color = color;
    strncpy(m_events[m_event_index].desc,desc,63);
    m_events[m_event_index].desc[63]='\0';           //  MEMOPT!!! (if you change above...... const it or something!!
    // increment the buffer and wrap around
    m_event_index++;                // m_event index will point to the entry AFTER the latest valid entry
    if (m_event_index == max_watch_event) m_event_index = 0;
    if (m_num_events<max_watch_event) m_num_events++;       // counts up to maximum.

}




void CWatchManager::Render(float time, bool paused)
{
    static float   period = 5.0f;           // period to draw in seconds
    static float   time_off = 0.0f;         // time offset to start drawing from
    
    
    float   height = 10;
    float   pos_x = 0;
    float   start_y = 0;
    float   pos_y = start_y;
    float   border = 3;
    const float left_area = 40;
    float   width = 1024-left_area;

    const   float   max_tick_period = 2.5f;


    if (paused)
    {
        // Zoom in and out, scaling around the center of the window
        if (g_pGamepad->button[CGamepad::BTN_A].Pressed())
        {
            time_off -= period * (1.01f - 1.0f) * 0.5f;
            period *= 1.01f;
        }
        if (g_pGamepad->button[CGamepad::BTN_Y].Pressed())
        {
            time_off -= period * (1.0f - 1.01f) * 0.5f;
            period *= 1.0f/1.01f;
        }
        if (g_pGamepad->button[CGamepad::BTN_L].Pressed())
            time_off += period / 100.0f;
        if (g_pGamepad->button[CGamepad::BTN_R].Pressed())
            time_off -= period / 100.0f;
        
    }
    else
    {
        time_off = 0;
     //   period = 5.0f;
    }

    if (!mp_head)
        return;

// Count how many there are, so we can put the events at the bottom
    int watches = 0;
    CWatch *p_watch = mp_head;
    while (p_watch)
    {
        watches++;
        p_watch=p_watch->mp_next;
    }

    pos_y = start_y + watches * ( height+border*3);

///////////////////////////////
// Now render the events

    float x = width;
    int index = m_event_index;
    int num = m_num_events;

    DrawRect(pos_x-border,pos_y-border, width+border*2 + left_area ,height*2+border*5,0xffffffff);
    if (m_display_events)
    {
        DrawString(pos_x, pos_y, "Events", 0xff000000);
    }
    char c[128];
    if  (period < max_tick_period)
    {
        sprintf(c,"%3.1fms",m_tick_time*1000);
    }
    else
    {
        sprintf(c,"%3.0fms",m_tick_time*10000);
    }
    DrawString(pos_x, pos_y+height+border, c, 0xff000000);
    float last_index_time = 0.0f;
    while (x > 0.01f && num > 0)
    {
        index--;
        if (index <0) index = max_watch_event-1;
        float   abs_index_time = m_events[index].time;
        float   index_time = m_events[index].time+time_off;                
        x = width - width*(time-index_time)/period;
        if (x>0)
        {
            if (m_events[index].event == -1)
            {
                // Draw the dicks as little grey lines at the bottom
                if (period < max_tick_period)
                {
                    DrawLine(left_area+pos_x+x,start_y - border, left_area+pos_x+x , pos_y + height * 3 , 0xfff0f0f0);              
                    DrawLine2(left_area+pos_x+x,pos_y + height , left_area+pos_x+x , pos_y + height * 3 , 0xfff0f0f0, 0xff808080);              
                }
                else if (last_index_time != 0.0f)
                {
                    //  draw the ticks that fall as close to 1/10th as possible
                    //  get prev, round it down to 1/10th, and see if this one is below it
                    // if so, draw it here as a blue line
                    float rounded = (float)((int)(last_index_time*10))/10.0f;
                    if (abs_index_time < rounded)
                    {
                        DrawLine2(left_area+pos_x+x,pos_y-border  , left_area+pos_x+x    , pos_y + height * 3 , 0xffffffff, 0xffe0f0f0);              
                        DrawLine2(left_area+pos_x+x+1,pos_y-border, left_area+pos_x+x+1 , pos_y + height * 3  , 0xffffffff, 0xffe0f0f0);              
                    }
                    
                }
                last_index_time = abs_index_time;
            }
            else
            {
                if (m_display_events)
                {
                    DrawLine(left_area+pos_x+x,start_y , left_area+pos_x+x , pos_y + m_events[index].y,m_events[index].color);              
                    DrawString(left_area+pos_x+x,pos_y + m_events[index].y, m_events[index].desc, m_events[index].color);
                }
            }
        }
        num--;
    }
    
    pos_y=start_y;

    p_watch = mp_head;
    while (p_watch)
    {
        p_watch->Render(pos_x, pos_y, width, height, border, period, left_area, time, time_off);
        pos_y += height+3.0f*border;
        p_watch = p_watch->mp_next;
        // Draw a line between individual watches 
        DrawRect(pos_x-border,pos_y-border*2, width+border*2 + left_area ,border,0xffe0e0e0);
    }
}

void CWatchManager::Update(float time)
{
    CWatch *p_watch = mp_head;
    while (p_watch)
    {
        p_watch->Update(time);
        p_watch = p_watch->mp_next;
    }
}


CWatch  *    CWatchManager::add_watch(DWORD color, const char *desc)
{
    CWatch * p_new_watch = new CWatch();
    p_new_watch->m_color = color;
    p_new_watch->mp_desc = desc;
    if (!mp_head)
    {
        mp_head = p_new_watch;
    }
    else
    {
        CWatch * p_tail = mp_head;
        while (p_tail->mp_next) p_tail = p_tail->mp_next;
        p_tail->mp_next = p_new_watch;
    }
    return p_new_watch;
}
                              
void        CWatchManager::SetWatch(float *p, DWORD color , const char*c)  { add_watch(color,c)->SetWatch(p); } 
void        CWatchManager::SetWatch(int *p, DWORD color, const char*c)     { add_watch(color,c)->SetWatch(p); } 
void        CWatchManager::SetWatch(char *p, DWORD color, const char*c)    { add_watch(color,c)->SetWatch(p); }
void        CWatchManager::SetWatch(unsigned char *p, DWORD color, const char*c)  { add_watch(color,c)->SetWatch(p); }
void        CWatchManager::SetWatch(bool *p, DWORD color, const char*c)  { add_watch(color,c)->SetWatch(p); }




class CGuy 
{

    public:
                    CGuy();  
        void        Logic(float timestep, float time);
        void        Render();
        Vector2     GetPos() {return m_pos;}
    
        enum EState {
            STATE_GROUND,
            STATE_AIR,
            STATE_JETPACK,
            
            
            NUM_STATES
        };
    private:    
        void            jetpack_logic();
        void            air_logic();
        void            ground_logic();
        void            state_transition(EState new_state);
        void            apply_acc_vel();
        float           height_above_ground();
        void            snap_to_ground();
        float           time_in_this_state() {return m_time-m_state_change_time;}
        
        float           m_time;                     // Current time
        float           m_timestep;                 // time since last update
    
        float           m_jump_time_start;
        
        int          m_state;                    // current state (ground, air, etc)
        float           m_state_change_time;        // time at which we entered this time
        int          m_old_state;                // previous state
        Vector2         m_old_pos;                  // position at the start of the update
        Vector2         m_pos;                      // current position
        Vector2         m_vel;                      // current velocity
        Vector2         m_acc;                      // temporary accelertion
        float           m_width;                    // visible width
        float           m_height;                   // visible height
        
        float           m_max_speed;                // overall limiting speed
        float           m_max_horizontal_speed;     // horizontal limitng speed

        bool            m_allow_double_jump;        // flag for if we are allowed to do a double jump
        bool            m_can_continue_jump;            // flag to allow them to contine accellerating at the start of the jump
        bool            m_crouched;                 // flag - true if crouched
        bool            m_can_ground_pound;
};

float   guy_gravity                     =   3000.0f;      
float   guy_jetpack_side_thrust         =   1000.0f;                 
float   guy_jetpack_thrust              =   5000.0f;
float   guy_contact_friction            =    200.0f;                 

float   guy_air_side_thrust             =   3000.0f;
float   guy_air_friction                =   1000.0f;

float   guy_ground_snap_dist            =   5.0f;
float   guy_ground_check_offset         =   5.0f;
float   guy_max_ground_check            =   1000.0f;
float   guy_ground_side_thrust          =   3000.0f;
float   guy_ground_friction             =   15000.0f;

float   guy_crouch_ground_friction      =   500.0f;

float   guy_max_speed                   =   5000.0f;        // Overall Maximum speed
float   guy_max_horizontal_speed        =   500.0f;         // maximum horizontal speed in any state
float   guy_max_run_speed               =   500.0f;         // maximum horizontal speed on ground

float   guy_super_jump_impulse          =   1200.0f;        // super jump speed is set to this, there is only one impulse
float   guy_ground_pound_speed          =   1500.0f;         // ground pound speed, straight down


float   guy_initial_jump_impulse        =   300.0f;         // initial jump speed is set to this
float   guy_continued_jump_thrust       =   100.0f;         // followed by an acceleration
float   guy_variable_jump_time          =   0.10f;          // for this amount of time
float   guy_double_jump_impulse         =   800.0f;
float   guy_double_jump_speed_window    =   300.0f;         // +/- velocity we have to be at the apex of a jump to allow double jump
float   guy_early_jump_time             =   0.050f;         // time before hitting the ground in which we are allowed an early jump
float   guy_late_jump_time              =   0.100f;         // time after leaving the ground in which we are allowed a late jump

CGuy::CGuy()
{
    m_pos.x = 40;
    m_pos.y = 150;
    m_vel.x = 0;
    m_vel.y = 0;
    m_width = 10;
    m_height = 30;
    m_state = STATE_AIR;
    m_crouched = false;

    // add various watches.  You can add or remove additional watches here.
    g_pWatchManager->SetWatch(&g_pGamepad->button[CGamepad::BTN_A].m_pressed, 0xff008000, "2/A");  // A = Green, primary
    g_pWatchManager->SetWatch(&g_pGamepad->button[CGamepad::BTN_X].m_pressed, 0xff000080, "1/X");  // X = blue
    g_pWatchManager->SetWatch(&g_pGamepad->button[CGamepad::BTN_B].m_pressed, 0xff800000, "3/B");  // B = Red
    g_pWatchManager->SetWatch(&g_pGamepad->button[CGamepad::BTN_Y].m_pressed, 0xff808000, "4/Y");  // Y = Yellow
    g_pWatchManager->SetWatch(&g_pGamepad->button[CGamepad::BTN_R1].m_pressed, 0xff808080, "6/R1");
    g_pWatchManager->SetWatch(&m_state, 0xff000000, "Air"); // Should be "State" but it's effectivley air as we only have 2 states now    
    g_pWatchManager->SetWatch(&m_crouched, 0xff000000, "Crouch");    


}

void    CGuy::Render()
{
    DWORD   color = 0xff000000;
    Vector2 s(1.8f,1.8f);
    float head_size = 3.0f;
    
    if (m_crouched)
    {
        s.y=1.0f;
        head_size = 4.0f; 
    }
    
    Vector2 LFoot(-5,0);
    Vector2 RFoot(5,0);
    Vector2 Hip(0,-8);
    Vector2 Shoulder(0,-16);
    Vector2 Head(0,-22);
    Vector2 LHand(-5,-12);
    Vector2 RHand(5,-12);
    DrawLine(m_pos+LFoot*s,m_pos+Hip*s,color);
    DrawLine(m_pos+Hip*s,m_pos+RFoot*s,color);
    DrawLine(m_pos+Shoulder*s,m_pos+Hip*s,color);
    DrawLine(m_pos+Shoulder*s,m_pos+RHand*s,color);
    DrawLine(m_pos+Shoulder*s,m_pos+LHand*s,color);
    DrawLine(m_pos+Shoulder*s,m_pos+Head*s,color);
    DrawRect(m_pos.x+Head.x*s.x - head_size*s.x,m_pos.y+Head.y*s.y - head_size*s.y,head_size*s.x*2,head_size*s.y*2,color);
}

void    CGuy::Logic(float timestep, float time)
{

    m_timestep = timestep;
    m_time = time;
    m_old_pos = m_pos;                           
    m_acc = Vector2(0,0);                      
    m_max_speed = guy_max_speed;
    m_max_horizontal_speed = guy_max_horizontal_speed;
    
    if (g_pGamepad->button[CGamepad::BTN_SELECT].Pressed())
    {
        m_vel = Vector2(0,0);
        m_pos = Vector2(100,100);
        m_old_pos = m_pos;
        m_state = STATE_AIR;
    }
    
    if (g_pGamepad->button[CGamepad::BTN_Y].Pressed() && m_state != STATE_JETPACK)
    {
        state_transition(STATE_JETPACK);       
    }

    switch  (m_state)
    {
        case STATE_GROUND:
            ground_logic();
            break;
        case STATE_AIR:
            air_logic();  
            break;
        case STATE_JETPACK:
            jetpack_logic();  
            break;
    }
    
}    

void CGuy::state_transition(EState new_state)
{
    if (new_state != m_state)
    {
        m_old_state = m_state;
        m_state = new_state;
        m_state_change_time = m_time;
    }
}

// Apply acceleration and velocity to the position
// this is just some common code I've factored out
void    CGuy::apply_acc_vel()
{
    // Add in gravity to the velocity
    m_vel += m_acc * m_timestep;
    
    // Limit velocity
    if (m_vel.Length() > m_max_speed)
    {
        m_vel = m_max_speed * m_vel.Normal();
    }
    
    // Limit horizontal velocity
    if (m_vel.x > m_max_horizontal_speed) m_vel.x = m_max_horizontal_speed;
    if (m_vel.x < -m_max_horizontal_speed) m_vel.x = -m_max_horizontal_speed;
    
    // add in the velocity to the position    
    m_pos += m_vel * m_timestep + 0.5f * m_acc * m_timestep * m_timestep;
}


float    CGuy::height_above_ground()
{
    CWhisker        down;
    if (down.CheckCollision(Vector2(m_pos.x,m_pos.y-guy_ground_check_offset), Vector2(m_pos.x,m_pos.y+guy_max_ground_check)))
    {
        float height = down.GetPoint().y - m_pos.y;
        return height;
    }
    return 100000.0f;       // can't find the ground
    
}

void   CGuy::snap_to_ground()
{
   float height = height_above_ground();
   m_pos.y += height;         
}


void    CGuy::ground_logic()
{

    // Kill acc and motion perpendicular to the surface we are stood on
    // Which I'm asuming here is just a horizontal surface
    m_acc.y = 0.0f;
    m_vel.y = 0.0f;

    if (!m_crouched)
    {
        // Apply thrust                      
        if (g_pGamepad->button[CGamepad::BTN_L].Pressed())
        {
            m_acc.x -= guy_ground_side_thrust;
        } else if (g_pGamepad->button[CGamepad::BTN_R].Pressed())
        {
            m_acc.x += guy_ground_side_thrust;
        }
        else
        {
            // Left or Right not pressed, so apply constant friction
            if (fabs(m_vel.x) < guy_ground_friction * m_timestep)
            {
                m_vel.x = 0;
            }
            else
            {
                m_acc.x -= guy_ground_friction * (m_vel.x>0?1.0f:-1.0f);
            }
            
        }
    }
    else
    {
        if (fabs(m_vel.x) < guy_crouch_ground_friction * m_timestep)
        {
            m_vel.x = 0;
        }
        else
        {
            m_acc.x -= guy_crouch_ground_friction * (m_vel.x>0?1.0f:-1.0f);
        }
    }

    // Set maximum speed for running
    m_max_speed = guy_max_run_speed;

    apply_acc_vel();

    CWhisker whisker;
    if (whisker.CheckCollision(m_old_pos + Vector2(0,-guy_ground_check_offset), m_pos  + Vector2(0,-guy_ground_check_offset)))
    {
        Vector2 attempted = m_pos;
        m_pos = whisker.GetPoint() + 0.1f * whisker.GetNormal() + Vector2(0,guy_ground_check_offset);
        m_vel = m_vel - 1.1f * DotProduct(m_vel,whisker.GetNormal()) * whisker.GetNormal();
        // Hitting something while on the ground means a wall, so don't both trying to continue velocity
    
    }

    float height = height_above_ground();
    if (height > guy_ground_snap_dist)
    {
        g_pWatchManager->RecordEvent(0, m_time, "Fall", 0xffc00000);             // red
        
        state_transition(STATE_AIR);      
        // down't allow double jumps if we just walked off something      
        m_allow_double_jump = false;       
        m_can_ground_pound = true;        
    }
    else
    {
        snap_to_ground();
    }

    // Button handling 
    if (g_pGamepad->button[CGamepad::BTN_A].Triggered())
    {
        g_pGamepad->button[CGamepad::BTN_A].ClearTrigger();
        // Only do it if the button was triggered recently                                                                               
        if ( m_time - g_pGamepad->button[CGamepad::BTN_A].PressedTime() < guy_early_jump_time)
        {       
            state_transition(STATE_AIR);    
            m_jump_time_start = m_time;
            if (m_crouched)
            {
                // super jump if crouched
                g_pWatchManager->RecordEvent(1, m_time, "SuperJump", 0xff00c000);         // green
                m_vel.y = -guy_super_jump_impulse;
                m_allow_double_jump = false;     
                m_can_continue_jump = false;
                m_can_ground_pound = false;
            }
            else
            {
                g_pWatchManager->RecordEvent(1, m_time, "Jump", 0xff00c000);         // green
                // normal jump if not crouched
                m_vel.y = -guy_initial_jump_impulse;
                m_allow_double_jump = true;     
                m_can_continue_jump = true;
                m_can_ground_pound = true;
                
            }
        }
    }
    
    if (g_pGamepad->button[CGamepad::BTN_R1].Pressed())
    {
        if (!m_crouched)
                g_pWatchManager->RecordEvent(1, m_time, "Crouch", 0xff404040);         // grey
        
        m_crouched = true;
    }
    else
    {
        if (m_crouched)
                g_pWatchManager->RecordEvent(1, m_time, "UnCrouch", 0xff004040);         // cyan
        m_crouched = false;
    }
}

void    CGuy::air_logic()
{

    // Apply thrust                      
    if (g_pGamepad->button[CGamepad::BTN_L].Pressed())
    {
        m_acc.x -= guy_air_side_thrust;
    }
    else if (g_pGamepad->button[CGamepad::BTN_R].Pressed())
    {
        m_acc.x += guy_air_side_thrust;
    }
    else
    {
        // Left or Right not pressed, so apply constant horizontal friction
        // not at all realistic, but that's platform games for you!
        if (fabs(m_vel.x) < guy_air_friction * m_timestep)
        {
            m_vel.x = 0;
        }
        else
        {
            m_acc.x -= guy_air_friction * (m_vel.x>0?1.0f:-1.0f);
        }
        
    }

    m_acc.y += guy_gravity;
    apply_acc_vel();

    CWhisker whisker;
    if (whisker.CheckCollision(m_old_pos, m_pos))
    {
        Vector2 attempted = m_pos;
        m_pos = whisker.GetPoint() + 0.1f * whisker.GetNormal();
        
        float height = height_above_ground();
        // snap to ground if close, and moving towards it
        if (height < guy_ground_snap_dist && m_vel.y > 0.0f)   
        {
            g_pWatchManager->RecordEvent(2, m_time, "Land", 0xff808080);         // grey
            state_transition(STATE_GROUND);    
            snap_to_ground();
            m_vel.y = 0.0f;        
        }
        else
        {
            // Not snapping to the gound, so bounce off the surface
            m_vel = m_vel - 1.1f * DotProduct(m_vel,whisker.GetNormal()) * whisker.GetNormal();
            // and since we've hit something, apply general surface friction
            m_vel -= guy_contact_friction * m_timestep*m_vel.Normal();   
        }
        
        // Since we hit something, see how much we moved of the original movement
        // then try a simple movement of the remaining amount along the new velocity.
        // just ignore if there is a repeated collision
        // distance attmpted to move - distance moved.
        float remaining = (attempted-m_old_pos).Length() - (m_pos-m_old_pos).Length();
		if (remaining > 0.0f && m_vel.Length2() != 0.0f) // mighth be <0 due to to normal movement, if so the remaining will be tiny
        {
            Vector2 newpos = m_pos + remaining * m_vel.Normal();
            if (!whisker.CheckCollision(m_pos,newpos))
            {
                m_pos = newpos;
            }
        }
    }

    if (m_state == STATE_AIR)
    {
        if (   g_pGamepad->button[CGamepad::BTN_A].Pressed()
            && m_can_continue_jump 
            && (m_time - m_jump_time_start) < guy_variable_jump_time)
        {
            // jump button held down 
                m_vel.y -= guy_continued_jump_thrust;
        }
        else
        {
            // If we release the jump button, then no additional thrust
            m_can_continue_jump = false;
        }
    
        if (g_pGamepad->button[CGamepad::BTN_A].Triggered()
         && m_time - g_pGamepad->button[CGamepad::BTN_A].PressedTime() < 0.25f
        )
        {
            // Check for double jumps
            if (m_allow_double_jump && fabs(m_vel.y) < guy_double_jump_speed_window)
            {
                g_pWatchManager->RecordEvent(3, m_time, "Double", 0xffc000c0);   // Magenta
                g_pGamepad->button[CGamepad::BTN_A].ClearTrigger();
                m_vel.y = -guy_double_jump_impulse;
                m_allow_double_jump = false;     
                m_can_ground_pound = true;
                
            }
            
            // check for late jumps
            else if (time_in_this_state() < guy_late_jump_time)
            {       
                g_pWatchManager->RecordEvent(4, m_time, "Lt_Jmp", 0xffc0c000);   // yellow
                g_pGamepad->button[CGamepad::BTN_A].ClearTrigger();
                m_vel.y = -guy_initial_jump_impulse;
                m_allow_double_jump = true;     
                m_can_continue_jump = true;
                m_jump_time_start = m_time;
                m_can_ground_pound = true;
            }
            
        }
        
         if (g_pGamepad->button[CGamepad::BTN_R1].Triggered()
         && m_time - g_pGamepad->button[CGamepad::BTN_R1].PressedTime() < 0.25f
         && m_can_ground_pound)
         {
            g_pWatchManager->RecordEvent(1, m_time, "Gnd_Pnd", 0xff0000c0);         // blue
            m_can_ground_pound = false;
            m_vel.y = guy_ground_pound_speed;
         }

        
    }
}


void    CGuy::jetpack_logic()
{
                           
// This is a simple "Jetpac" model of motion.

    // Apply thrust                      
    if (g_pGamepad->button[CGamepad::BTN_L].Pressed())
    {
        m_acc.x -= guy_jetpack_side_thrust;
    }

    if (g_pGamepad->button[CGamepad::BTN_R].Pressed())
    {
        m_acc.x += guy_jetpack_side_thrust;
    }

    if (g_pGamepad->button[CGamepad::BTN_Y].Pressed())
    {
        m_acc.y -= guy_jetpack_thrust;
    }
    
    m_acc.y += guy_gravity;

    apply_acc_vel();

    CWhisker whisker;
    if (whisker.CheckCollision(m_old_pos, m_pos))
    {
        Vector2 m_attempted = m_pos;
        m_pos = whisker.GetPoint() + 0.1f * whisker.GetNormal();
        m_vel = m_vel - 1.1f * DotProduct(m_vel,whisker.GetNormal()) * whisker.GetNormal();
        // Since we hit something, see how much we moved of the original movement
        // then try a simple movement of the remaining amount along the new velocity.
        // just ignore if there is a repeated collision
        // distance attmpted to move - distance moved.
        float remaining = (m_attempted-m_old_pos).Length() - (m_pos-m_old_pos).Length();
        if (remaining > 0.0f) // mighth be <0 due to to normal movement, if so the remaining will be tiny
        {
            Vector2 newpos = m_pos + remaining * m_vel.Normal();
            if (!whisker.CheckCollision(m_pos,newpos))
            {
                m_pos = newpos;
            }
        }
        // and since we've hit something, apply general surface friction
        m_vel -= guy_contact_friction * m_timestep*m_vel.Normal();   
    }

    if (!g_pGamepad->button[CGamepad::BTN_Y].Pressed())
    {
       state_transition(STATE_AIR);       
    }
}

CGuy                    * g_pGuy;

/////////////////////////////////////////////////////////////////////////////////
// The main MX application

CRITICAL_SECTION	debug_CS;


/*

// problem happend before around 00260

all with no threads
	10000			.0051
	12000			.0062
	14000			.0071
	15000			.0078
	16000			.0083
	17000			.0088
	18000			.0095
	20000			.0104
	40000			.0207
	48000			.0246
	50000			.0255
	50250			.0257
	50400			.0260
	50500			.0269, 0.0266 .0273  <<<< ODD
	50550			.0258
	50600			.0262
	51000			.0264
	51500			.0265
	52000			.0270
	52500			.0273
	53000			.0271
	54000			.0279
	80000			.0414

*/

void MX_Init()
{
	InitializeCriticalSection(&debug_CS);
#ifdef		CRITICAL_SECTION_RND
	InitializeCriticalSection(&rnd_CS);
#endif
	g_pWatchManager = new CWatchManager();
    g_pGamepad = new CGamepad();
    g_pGuy = new CGuy();
	g_ParticleManager.Init(NUM_PARTICLES);
}

char  printf_buffer[1024];

void    debug_log( const char* text, ...)
{
	EnterCriticalSection(&debug_CS);

// Get Text into a printable buffer	 (maybe prepend time)
    va_list args;
	va_start( args, text );
	vsprintf( printf_buffer, text, args);
	va_end( args );
// output as debug text
    OutputDebugString(printf_buffer);

	LeaveCriticalSection(&debug_CS);

}

float game_time = 0.0f;

static bool paused = false;

void MX_Logic(float time)
{
    static float last_time = 0.0f;
    
    float timestep = time - last_time;
    
    if (timestep > 0.25f) timestep = 0.25f;

    last_time = time;
        

    if (paused)
    {
        g_pGamepad->Update(game_time);

        if (g_pGamepad->button[CGamepad::BTN_R1].Triggered())
        {
            g_pGamepad->button[CGamepad::BTN_R1].ClearTrigger();
            g_pWatchManager->ToggleEventDisplay();
        }
    }
    else
    {
        //game_time += timestep;
        game_time = Timer_Seconds();
        g_pGamepad->Update(game_time);
        g_pGuy->Logic(timestep, game_time);
        g_pWatchManager->Update(game_time);
        //  adding the tick event last, as they are drawn in reverse order and we don't want ot overwrite actual events
        g_pWatchManager->RecordEvent(-1, game_time, "Tick", 0xffc00000);  // Special event to record clock ticks, displayed along the bottom
		g_ParticleManager.Update(timestep);
    }
    
    if (g_pGamepad->button[CGamepad::BTN_START].Triggered())
    {
        g_pGamepad->button[CGamepad::BTN_START].ClearTrigger();
        paused = !paused;
        if (paused)
            Timer_Pause();
        else
            Timer_Resume();
    }

}

void MX_Render()
{
    RenderWorld();
    g_pGuy->Render();
 //   g_pWatchManager->Render(game_time, paused);
	g_ParticleManager.Render();
}

void MX_Cleanup()
{
    delete      g_pWatchManager;
    delete      g_pGamepad;
    delete      g_pGuy;
}



//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    	
        case WM_SIZE:
    	{
//    	    resize_db(hWnd);		
//    		InitD3D(hWnd);
            g_resize = true;
            break;
    	}           

		case WM_LBUTTONUP:
		{
			dragging = false;
			break;
		}


		case WM_LBUTTONDOWN:
		{
			dragging = true;
	// DROP THROUGH
	//		break;
		}
		
		case WM_MOUSEMOVE:
		{
			if (dragging)
			{
				mouse_x = LOWORD(lParam);  // X position of cursor 
				mouse_y = HIWORD(lParam);  // Y position of cursor
			}
			break;	
		}

            
    }

    if (g_pWatchManager)  // Might not have been initialized yet
    {
        char win_event[1024]; 
        sprintf(win_event, "%x (%d)", msg, wParam);
    
//        g_pWatchManager->RecordEvent(5, Timer_Seconds(), win_event, 0xffc00000);  // Special event to record clock ticks, displayed along the bottom
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}



//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT UpdateInputState( HWND hDlg )
{
    HRESULT     hr;
    TCHAR       strText[512] = {0}; // Device state text

    if( NULL == g_pJoystick ) 
        return S_OK;

    // Poll the device to read the current state
    hr = g_pJoystick->Poll(); 
    if( FAILED(hr) )  
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = g_pJoystick->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pJoystick->Acquire();

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

    // Get the input's device state
    if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
        return hr; // The device should have been acquired during the Poll()

    // Get the buffered input events

    DWORD     pdwInOut = JOY_BUFFERSIZE;
    g_pJoystick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), g_inputbuffer, &pdwInOut, 0);         
    DWORD system_ticks = GetTickCount();
    float   game_time = Timer_Seconds();
    float system_time = system_ticks/1000.0f;
    if (pdwInOut != 0)
    {

        //debug_log("Have %d buffered from joy", pdwInOut);

        // Go through them and note them as events so we can see timing

        for (unsigned int i=0;i<pdwInOut;i++)
        {
            float timestamp = (float)g_inputbuffer[i].dwTimeStamp/1000.0f;
          //  debug_log("[%d] = 0x%x at Time = %f, system_time = %f, (%f), gametime = %f",(int)g_inputbuffer[i].dwOfs, (int)g_inputbuffer[i].dwData, timestamp, system_time, timestamp-system_time, Timer_Seconds());

            g_pWatchManager->RecordEvent(6, game_time + (timestamp - system_time), g_inputbuffer[i].dwData?"On":"Off", 0xffc00000);  
        }
    }
    return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Simple particle system for testing paralleization schemes

float force_x,force_y;
int force_dir;

void CParticle::Update(float time)
{

#ifndef PRETTY_UP
	
	float a = 1.1f;
#if 0
	// some extra CPU intensive processing
	for (int x=0;x<100;x++)
		a *= 0.999f;
	return;
#endif


	m_old_pos = m_pos;
	Vector2 pos2 = m_pos + m_vel * time;
	
	// acc toward mouse pos
	if (1 || dragging)
	{

		float x = force_x; 
		float y = force_y; 
		if (dragging)
		{
			x = (float)mouse_x;
			y = (float)mouse_y;
		}

		Vector2 to = m_pos - Vector2(x,y);
		float force = 1000.0f/to.Length2();
		if (!dragging)
		{
			m_vel += to * force;
			// friction
			m_vel -= m_vel * 0.03f;
		}
		else
		{
			m_vel -= to * force * 5.0f;
			// friction
			m_vel -= m_vel * 0.002f;
		}

	}
	else
	{
		// bit of randomness
		m_vel += Vector2(rnd(10000)/1000.0f-5.0f,rnd(10000)/1000.0f-5.0f);
		// bit of gravity
		m_vel += Vector2(0.0f,10.0f);
		// friction
		m_vel -= m_vel * 0.02f;
	}

	m_color = 0xff800000 + (int)m_vel.Length() + (int)a;

	CWhisker whisker;
	if (whisker.CheckCollision(m_pos, pos2))
	{
		// Move to the collision point, and raise slightly off the surface
		m_pos = whisker.GetPoint() + 0.01f * whisker.GetNormal();
		// reflect the velocity in the plane of the surface
		m_vel = m_vel - 2.0f * DotProduct((m_vel),whisker.GetNormal()) * whisker.GetNormal();
		//m_vel = -m_vel;
	}
	else
	{
		m_pos = pos2;
	}
#else




	float a = 1.1f;
#if 0
	// some extra CPU intensive processing
	for (int x=0;x<100;x++)
		a *= 0.999f;
	return;
#endif


	m_old_pos = m_pos;
	Vector2 pos2 = m_pos + m_vel * time;
	
	// acc toward mouse pos
	if (1 || dragging)
	{

		float x = force_x; 
		float y = force_y; 
		if (dragging)
		{
			x = (float)mouse_x;
			y = (float)mouse_y;
		}

		Vector2 to = m_pos - Vector2(x,y);
		float force = 1000.0f/to.Length2();
		if (!dragging && force_dir)
		{
			if (force > 1000.0f) force = 1000.0f;
			//m_vel += to.Normal() * force;
			m_vel += to * force;
			// friction
			m_vel -= m_vel * 0.03f;
			m_vel += Vector2(rnd(10000)/1000.0f-5.0f,rnd(10000)/1000.0f-5.0f);
		}
		else
		{
			if (force > 1.0f) force = 1.0f;
			//force = 1.0f;
			m_vel -= to.Normal() * force * 50.0f;
			// friction
			m_vel -= m_vel * 0.02f;
		}

	}
	else
	{
		// bit of randomness
		m_vel += Vector2(rnd(10000)/1000.0f-5.0f,rnd(10000)/1000.0f-5.0f);
		// bit of gravity
		m_vel += Vector2(0.0f,10.0f);
		// friction
		m_vel -= m_vel * 0.002f;
	}

#ifdef	NBODY

		CParticleManager *ppm = &g_ParticleManager;
		int n = ppm->m_num_particles;
		CParticle *pp = ppm->mp_particles; 
		Vector2 my_pos = m_pos;
		for (int i=0;i<n;i++)
		{
			Vector2 to = pp[i].m_pos - my_pos;
			float len = to.Length();
			if (len != 0.0f)
			{
				m_vel += len * to.Normal() * 0.00001f;
				//m_vel += to.Normal() * 0.1;
			}
		}
#endif



	//m_color = 0xffff0000 + (int)m_vel.Length() + ((int)(255.0f*(m_vel.Normal().x))<<8);
	//m_color = 0xffff0000 + ((int)(255.0f*(m_vel.Normal().y))) + ((int)(255.0f*(m_vel.Normal().x))<<8);
	//m_color = 0xffff0000 + ((int)(127.0f*(m_vel.Normal().y+1.0f))) + ((int)(127.0f*(m_vel.Normal().x+1.0f))<<8);
	m_color = 0xff000000 + ((int)(127.0f*(m_vel.Normal().y+1.0f))) + ((int)(127.0f*(m_vel.Normal().x+1.0f))<<8) + ((int)(127.0f*(m_vel.Normal().y * m_vel.Normal().x+1.0f))<<16) ;

	CWhisker whisker;
	if (whisker.CheckCollision(m_pos, pos2))
	{
		// Move to the collision point, and raise slightly off the surface
		m_pos = whisker.GetPoint() + 0.01f * whisker.GetNormal();
		// reflect the velocity in the plane of the surface
		m_vel = m_vel - 2.0f * DotProduct((m_vel),whisker.GetNormal()) * whisker.GetNormal();
		//m_vel = -m_vel;
	}
	else
	{
		m_pos = pos2;
	}

#endif

}


CParticleManager::CParticleManager()
{
	mp_particles = NULL;
	m_num_particles = 0;
}

CParticleManager::~CParticleManager()
{
	if (mp_particles)
		delete [] mp_particles;
}

// Release, forked threads, no physics
// 0		0.0214 0.0331
// 1		0.0226 0.0343
// 2		0.0132 0.0250
// 3		0.0102 0.0218
// 4		0.0096 0.0209

// Release Mode, Worker threads no affinity, no physics
// 0		0.0214 0.0331
// 1		0.0214 0.0331
// 2		0.0117 0.0234
// 3		0.0091 0.0207
// 4		0.0071 0.0189

// Overclocked MB to 3.6 Ghz
// 4		0.0063 0.0177 << YAY!!!

// Release Mode, worker, Dual Core, HT Disabled
// 0		0.0214 0.0333
// 1		0.0211 0.0319
// 2		0.0108 0.0226  <<< best
// 3		0.0136 0.0254
// 4		0.0111 0.0228

// Single Core, HT enabled
// 0		0.0219 0.0340
// 1		0.0215 0.0336
// 2		0.0141 0.0260
// 3		0.0157 0.0276
// 4		0.0144 0.0263

// Single Core, HT disabled  (all times are essentially the same)
// 0		0.0219 0.0341
// 1		0.0223 0.0343
// 2		0.0221 0.0341
// 3		0.0220 0.0339
// 4		0.0221 0.0339

// Release Mode, no affinity, yes physics
// 0		0.0238  0.0266 0.0400
// 1		0.0251  247, 383   (252, 393) (254, 381) <<< slower!! (oscillates)
// 2		158, 301
// 3		119,251
// 4		110,243

// Release Optimized
// Optimizations used:
// Whole program Optimization: Link time code generation
// Floating point Model:  Fast
// 0		0.0182, 0.0282
// 1		0.0121,	0.0221
// 2		0.0069, 0.0168
// 3		0.0056, 0.0156
// 4		0.0046, 0.0144
//
// As above with Enhanced instruction set: streaming Simd2
//
// 0		0.0095, 0.0195
// 1		0.0095, 0.0195
// 2		0.0056, 0.0156
// 3		0.0044, 0.0146
// 4		0.0036, 0.0138
//
// And with inline any suitable
// 1		0.0080, 0.0164
// 2		0.0046, 0.0133
// 3		0.0036,	0.0122
// 4		0.0030, 0.0116
//
// Overclocked MB to 3.6 Ghz
// 4		0.0026, 0.0109

////////////////////////////////////////////////////////////////////////

// Release Mode, set affinity, no physics
// 0		0.0214
// 1		0.0217
// 2		0.0110
// 3		0.0093
// 4		0.0072

// Single core 1.5Ghz machine, worker threads, no physics
// 0		0.0790 0.1019

// 1		0.0454 0.0673
// 2		0.0426 0.0654
// 3		0.0429 0.0663 (highly variable)
// 4		0.0422 0.0646


// Single core, optimized version
// 0		0.0176, 0.0365
// 1		0.0176, 0.0366
// 4		0.0188, 0.0373

// Single Core, optimized, but no SIMD2
// 0		0.1022, 0.1207


// with physics

// 0		0.1565, 0.1997
// 1		0.0832, 0.1118
// 2		0.0664, 0.0929
// 3		0.0554	0.0925
// 4		0.0546	0.0891

// 1.5 Ghz Celeron laptop, worker, no phys
//
// 0		0.0428, 0.0727
// 1		0.0345, 0.0648


///////////////////////////////////////////////////////////////////////
// DEBUG mode
// New system - Setting affinity to 1<<t
// 0		0.0417  100
// 1		0.0417	100
// 2		0.0214  51.3%	Fastest 2 thread, as 
// 3		0.0243  56.1%	<<< slower with 3, task cannot be divided evenly between 3 procs with affinity set
// 4		0.0187	44.8%

// 5		0.0240
// 6		0.0209
// 7		0.0218
// 8		0.0188
// 16		0.0199

// New System - not setting affinity, batched processing
// 0		0.0417  (consistenet)
// 1		0.0417  (consistent)
// 2		0.0248	59.4%	,		0.0239, 0.0272, 0.0250, 0.0243  0.0247, 0.250, 0.240
// 3		0.0225  53.9%	0.0225 
// 4		0.0184  44.1%	(consistent) (fastest because other task are now evenly spread)

// New System, not set affinity, interleaved processing
// 1		0.0417
// 2		0.0256	61.3%
// 3		0.0227	54.4%
// 4		0.0191	45.8%





HANDLE	thStartThreading[MAX_THREAD];
HANDLE	thStopThreading[MAX_THREAD];
HANDLE	thThread[MAX_THREAD];

float update_time;

DWORD WINAPI thParticleUpdate(LPVOID p)
{
	int thread = *(int*)p;
#ifndef	USE_FORKED_THREADS
	while (1)
	{
		WaitForSingleObject(thStartThreading[thread],INFINITE);
#endif
		float time = update_time;
		int n = g_ParticleManager.m_num_particles;
		int step = n / MAX_THREAD;
		int start = step*thread;
		int end = step*(thread+1);
		if (thread == MAX_THREAD-1)
			end = n;

#ifdef MANUAL_OPT
		CParticle *p = &g_ParticleManager.mp_particles[start];
		for (int i=start;i<end;i++)
		{
			p->Update(time);
			p++;
		}
#else
// 
 #ifdef	PROCESS_CHUNKS	// Process in chunks
		for (int i=start;i<end;i++)
 #else	// Process interleaved
		for (int i=thread;i<n;i+=MAX_THREAD)
 #endif
		{
			g_ParticleManager.mp_particles[i].Update(time);
		}

#endif



#ifndef	USE_FORKED_THREADS
		SetEvent(thStopThreading[thread]);
	}
#endif
	return 0;
}


float dummy_a = 1.1f;;
DWORD WINAPI DummyThread(LPVOID p)
{
	while (dummy_a != 0)
		dummy_a *= 1.00001f;
	return 0;
}

DWORD WINAPI NullThread(LPVOID p)
{
	return 0;
}


int	ThreadIndex[MAX_THREAD];

Vector2	drift;
Vector2	driftv;
Vector2	drifta;


void CParticleManager::Init(int n)
{
	drift = Vector2(g_viewport_width/2.0f, g_viewport_height/2.0f);
	driftv = Vector2(0,0);
	drifta = Vector2(0,0);
	// Create the Particles
	if (mp_particles)
		delete [] mp_particles;
	mp_particles = new CParticle[n];
	m_num_particles = n;
	for (int i = 0;i<n;i++)
	{
		mp_particles[i].m_pos = Vector2(g_viewport_width/2.0f, g_viewport_height/2.0f) + Vector2((float) rnd(50)-100, (float)rnd(50)-100);
		mp_particles[i].m_old_pos = mp_particles[i].m_pos + Vector2(30,30);
		mp_particles[i].m_color = 0xff00ff00; // default magenta for un-updated particles
		mp_particles[i].m_vel = Vector2((float)(rnd(10000)-5000),(float)(rnd(10000)-5000));
		mp_particles[i].m_vel = mp_particles[i].m_vel.Normal();
		mp_particles[i].m_vel = mp_particles[i].m_vel * (20.0f + rnd(10000)/10.0f);
	}

#ifdef	USE_WORKER_THREADS
		DWORD	threadId;
	// Create the threads and events
	for (int t=0;t<MAX_THREAD;t++)
	{
		ThreadIndex[t] = t;
		thStartThreading[t] = CreateEvent(NULL, FALSE, FALSE, NULL);
		thStopThreading[t]  = CreateEvent(NULL, FALSE, FALSE, NULL);
		thThread[t] = CreateThread(NULL,0,thParticleUpdate,(LPVOID)&ThreadIndex[t],0,&threadId);
		SetThreadPriority(thThread[t],0);		// 0 = Normal (default)
#ifdef	SET_AFFINITY
		SetThreadAffinityMask(thThread[t],1<<t);
#endif
	}
#endif

#ifdef	USE_DUMMY_PHYSICS
		int	xx;
		DWORD	threadId2;
		HANDLE dt = CreateThread(NULL,0,DummyThread,(LPVOID)&xx,0,&threadId2);
	//	SetThreadPriority(dt,-2);  // below normal

#endif
}



// Call the particle update function, either directly
// or by starting the thread(s) and then waiting for them to finish
void CParticleManager::Update(float time)
{
	#define	SAMPLES 200
	static float avg[SAMPLES];
	static int avgn = 0;
	static float favg[SAMPLES];
	static int favgn = 0;
	static int samples = -50;

	
	if (rnd(10) == 0)
	{
	//	force_x = (float)rnd(g_viewport_width);
	//	force_y = (float)rnd(g_viewport_height);
	//	force_dir = !rnd(10);
	}
	

	force_x = drift.x;
	force_y = drift.y;

	drift += driftv;
	if (drift.x<0.0f && driftv.x<0.0f) driftv.x = -driftv.x; 
	if (drift.y<0.0f && driftv.y<0.0f) driftv.y = -driftv.y; 
	if (drift.x>g_viewport_width && driftv.x>0.0f) driftv.x = -driftv.x; 
	if (drift.y>g_viewport_height && driftv.y>0.0f) driftv.y = -driftv.y; 

	// Occasionally change acceleration
	if (rnd(5) == 0)
	{
		drifta = Vector2((float)(rnd(200)-100)/200.0f,(float)(rnd(200)-100)/200.0f) * 10.0f;
	}
	driftv += drifta;
	driftv *= 0.9f;

	// Occasionally stop
	if (rnd(20) == 0)
	{
		drifta = Vector2(0,0);
		driftv = Vector2(0,0);
	}


	float	duration = Timer_Seconds();
	update_time = time;
	update_time = 0.02f;

#ifndef	USE_WORKER_THREADS
	#ifndef	USE_FORKED_THREADS
		// Don't use worker threads just iterate over the particle list (in the main thread)
		int n = m_num_particles;
		for (int i=0;i<n;i++)
		{
			mp_particles[i].Update(update_time);
		}
	#else
		// Use forked threads, create and start them
		DWORD	threadId;
		for (int t=0;t<MAX_THREAD;t++)
		{
			ThreadIndex[t] = t;
			thThread[t] = CreateThread(NULL,0,thParticleUpdate,(LPVOID)&ThreadIndex[t],0,&threadId);
		}
		// Wait for them to terminate
		WaitForMultipleObjects(MAX_THREAD,&thThread[0],true, INFINITE);
		// and delete the handles
		for (int t=0;t<MAX_THREAD;t++)
		{
			CloseHandle(thThread[t]);
		}
	#endif
#else
// Multithreaded - flag the worker threads to start running
	for (int t=0;t<MAX_THREAD;t++)
		SetEvent(thStartThreading[t]);
// Then wait for them all to finish
	WaitForMultipleObjects(MAX_THREAD,&thStopThreading[0],true, INFINITE);
#endif
	duration -= Timer_Seconds();

#ifdef	USE_FORK_MARKING
	int	xx;
	DWORD	threadId;
	HANDLE nt = CreateThread(NULL,0,NullThread,(LPVOID)&xx,0,&threadId);
	WaitForSingleObject(nt,INFINITE);
	CloseHandle(nt);
#endif

	static float	frame_last_time = 0.0f;
	float	frame_length = Timer_Seconds() - frame_last_time;
	frame_last_time = Timer_Seconds();
	float a=0;
	float fa=0;
	if (samples<SAMPLES) samples++;
	if (samples >0)
	{
		if (avgn < SAMPLES) 
		{
			avg[avgn]=duration;
			favg[avgn]=frame_length;
			avgn++;
		}

		for (int i=0;i<avgn;i++)
		{
			a+=avg[i];
			fa+=favg[i];
		}
		a /= (float) avgn;
		fa /= (float) avgn;
	}
	char buf[512];
	sprintf(buf, "avg = %.6f duration = %.6f   frame avg = %.6f  len = %.6f samples = %d",a,duration,fa,frame_length, samples);
	//debug_log("%s",buf);
	DrawString(0,0,buf,0xffffffff);
}

void CParticleManager::Render()
{
	for (int i=0;i<m_num_particles;i++)
	{
		Vector2 back = mp_particles[i].m_old_pos;
#ifdef	PRETTY_UP
		if ((mp_particles[i].m_pos-back).Length() < 2.5f)
			back += (mp_particles[i].m_pos-back).Normal()*2.5f;
#endif
		DrawLine(back, mp_particles[i].m_pos, mp_particles[i].m_color);		
	}
}

