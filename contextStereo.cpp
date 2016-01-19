#include "context.h"
#include "keysinput.h"
#include "shader.h"
#include "rawinput.h"
#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>
#include <il.h>
#include <assert.h>


namespace SCore {

camera *context::Camera;
win32::keyStates *context::keys;

namespace win32 {
typedef struct {
     HINSTANCE hinstance;
     HWND hwnd;
     HDC hdc;
     HGLRC hglrc;
     HANDLE periodicTimer;
} platformHandles;

static bool applicationActive = true;

LRESULT CALLBACK glWndProc( HWND hwnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam  )
{
     switch(uMsg) {

     case WM_KEYDOWN: {
          if (wParam == VK_ESCAPE) {
               PostQuitMessage( 0 );
               return 0;
          }
          break;
     }
     case WM_ACTIVATE: {
          if (wParam == WA_ACTIVE) {
               applicationActive = true;
          } else if (wParam == WA_CLICKACTIVE) {
               applicationActive = true;
          } else if (wParam == WA_INACTIVE) {
               applicationActive = false;
          } else
               break;

          return 0;
          break;
     }
     case WM_QUIT: {
          printf( "WM_QUIT\n" );
          PostQuitMessage( 41 );
          return 0;
          break;
     }
     case WM_CLOSE: {
          printf( "WM_CLOSE\n" );
          PostQuitMessage( 42 );
          return 0;
          break;
     }
     default:
          break;
     }

     return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

bool IsMouseOver( HWND hWnd )
{
     RECT Rect;
     POINT Pt;

     GetWindowRect(hWnd,&Rect);
     GetCursorPos(&Pt);

     return PtInRect(&Rect,Pt) == TRUE;
}
};

bool contextStereo::create( contextConfig* cfg )
{
     WNDCLASSA wc;
     DWORD dwExStyle;
     DWORD dwStyle;
     int PixelFormat;

     if (platformSpecificPrivateData != NULL) return false;
     this->cfg = cfg;

     win32::platformHandles *c = new win32::platformHandles;

     static PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),
          1,
          PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
          PFD_TYPE_RGBA, 32,
          0,0,0,0,0,0,0,0,0,0,0,0,
          32, 0, 0 ,
          PFD_MAIN_PLANE,
          0,0,0,0
     };

     c->hinstance = GetModuleHandle( NULL );

     memset( &wc, 0, sizeof(wc) );
     wc.lpfnWndProc = (WNDPROC) win32::glWndProc;
     wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
     wc.hInstance = c->hinstance;
     //wc.hIcon = LoadIcon();
     wc.hCursor = LoadCursor( NULL, IDC_ARROW );
     wc.lpszClassName = "scream";

     if (!RegisterClassA(&wc))
          return false;

     if (cfg->fullScreen) {
          DEVMODE dmScreenSettings;

          memset(&dmScreenSettings , 0 , sizeof(dmScreenSettings));
          dmScreenSettings.dmSize = sizeof(dmScreenSettings);
          dmScreenSettings.dmPelsWidth = cfg->sx;
          dmScreenSettings.dmPelsHeight = cfg->sy;
		  dmScreenSettings.dmDisplayFrequency = 120;
          dmScreenSettings.dmBitsPerPel = 32;
          dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

          if ( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
               return false;

          dwExStyle = WS_EX_APPWINDOW;
          dwStyle = WS_POPUP;
     } else {
          // windowed version.
          dwExStyle = 0;
          dwStyle = WS_POPUP;
     }

     // center the window.
     int px = (GetSystemMetrics( SM_CXSCREEN ) - int(cfg->sx)) >> 1;
     int py = (GetSystemMetrics( SM_CYSCREEN ) - int(cfg->sy)) >> 1;

     c->hwnd = CreateWindowExA( dwExStyle, "scream", cfg->windowTitle,
                                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
                                px, py, cfg->sx, cfg->sy, NULL, NULL, c->hinstance, NULL );

     if (c->hwnd == NULL)
          return false;

     // for future use, the id number of the window instance.
     SetWindowLong( c->hwnd, GWL_USERDATA, 0 );

     // drawing context and pixel format
     c->hdc = GetDC(c->hwnd);

     if (c->hdc == NULL)
          return false;

     PixelFormat = ChoosePixelFormat(c->hdc, &pfd);

     if (PixelFormat == 0)
          return false;

     if (!SetPixelFormat(c->hdc, PixelFormat, &pfd))
          return false;

     // opengl init.
     HGLRC tempglrc = wglCreateContext(c->hdc);
     if (tempglrc == NULL)
          return false;

     if (!wglMakeCurrent(c->hdc, tempglrc))
          return false;

     assert( glGetError() == GL_NO_ERROR );

	 // stereoscopic 3d initialisation
#if 0
     glewInit();
	 
	 DestroyWindowA( c->hwnd );

     c->hwnd = CreateWindowExA( dwExStyle, "scream", cfg->windowTitle,
                                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
                                px, py, cfg->sx, cfg->sy, NULL, NULL, c->hinstance, NULL );

     if (c->hwnd == NULL)
          return false;

     // for future use, the id number of the window instance.
     SetWindowLong( c->hwnd, GWL_USERDATA, 0 );

     // drawing context and pixel format
     c->hdc = GetDC(c->hwnd);

     if (c->hdc == NULL)
          return false;

     static int stereoQuadGlAttributeList[] = {
          WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
          WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		  WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, 
		  WGL_COLOR_BITS_ARB, 32,
          //WGL_STEREO_ARB, GL_TRUE,
          WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
          0, 0
     };

     static int stereoQuadGlFloatAttributeList[] = {
          0.0f, 0.0f
     };

     if( !wglChoosePixelFormatARB(c->hdc, stereoQuadGlAttributeList, stereoQuadGlFloatAttributeList,  ) )
		return false;

     wglMakeCurrent(c->hdc, c->hglrc);
     wglDeleteContext( tempglrc );

     assert( glGetError() == GL_NO_ERROR );
#else
     c->hglrc = tempglrc;
#endif

     // extensions
     glewInit();

     assert( glGetError() == GL_NO_ERROR );

     // get some extension specific properties
     int maxDrawBuffers;
     glGetIntegerv( GL_MAX_DRAW_BUFFERS_ARB, &maxDrawBuffers );
     int maxTexLayers;
     glGetIntegerv( GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &maxTexLayers );
     int max3DTextureSize;
     glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE_EXT, &max3DTextureSize );
     int maxTextureImageUnits;
     glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTextureImageUnits );
     int maxImageUnits;
     glGetIntegerv( GL_MAX_IMAGE_UNITS_EXT, &maxImageUnits );
     int maxCombinedTextureUnits;
     glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits );
     int maxImageIO;
     glGetIntegerv( GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT, &maxImageIO );
     int maxBindableUniformSz;
     glGetIntegerv( GL_MAX_BINDABLE_UNIFORM_SIZE_EXT, &maxBindableUniformSz );
     int maxVertexBindables;
     glGetIntegerv( GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT, &maxVertexBindables );
     int maxGeomBindables;
     glGetIntegerv( GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT, &maxGeomBindables );
     int maxFragmentBindables;
     glGetIntegerv( GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT, &maxFragmentBindables );

     assert( glGetError() == GL_NO_ERROR );

     // default state
     glViewport( 0, 0, cfg->sx, cfg->sy );
     glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
     glClearDepth( 10000.0f );
     glDepthRange( 0.01f, 10000.0f );
     glDepthFunc( GL_LESS );

     if (GLEW_ARB_seamless_cube_map)
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS_ARB );

     assert( glGetError() == GL_NO_ERROR );

     // window blabla
     ShowWindow( c->hwnd, SW_SHOW );
     SetForegroundWindow( c->hwnd );
     SetFocus( c->hwnd );
     ShowCursor( FALSE );

     // image library
     ilInit();

     // timer
	 /*
     LARGE_INTEGER liPeriod;
     liPeriod.QuadPart = 0;
     c->periodicTimer = CreateWaitableTimerA( NULL, FALSE, NULL );
     SetWaitableTimer( c->periodicTimer, &liPeriod, 15, NULL, NULL, 0 );

     // Camera setup
     Camera = new camera( px+(cfg->sx>>1), py+(cfg->sy>>1) );
     keys = new win32::keyStates;
     SetCursorPos( px+(cfg->sx>>1), py+(cfg->sy>>1) );
	 */

     // set the data
     platformSpecificPrivateData = (void*) c;

     return glGetError() == GL_NO_ERROR;
}

bool context::update()
{
     win32::platformHandles *c = (win32::platformHandles*) platformSpecificPrivateData;
     if (c == NULL) return false;

     SwapBuffers( c->hdc );
     WaitForSingleObject( c->periodicTimer, 15 );

     if (win32::applicationActive) {
          double t = GetTickCount() * 0.001;
          keys->reset( float(t) );
          float keyDx = (keys->getKey( KEY_RIGHT ) - keys->getKey( KEY_LEFT )) * 10.0f;
          float keyDy = (keys->getKey( KEY_DOWN ) - keys->getKey( KEY_UP )) * 10.0f;
          //printf( "dx %4f dy %4f\n", keyDx, keyDy );
          Camera->update( t, keyDx, keyDy );
     }

     /** check the messages */
     MSG msg;
     bool userEnd = false;

     while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) {
          if (msg.message == WM_QUIT) {
               userEnd = true;
          }
          TranslateMessage( &msg );
          DispatchMessage( &msg );
     }

     return !userEnd;
}

void context::destroy()
{
     if (platformSpecificPrivateData==NULL) return;
     platformSpecificPrivateData = NULL;
     ShowCursor( TRUE );
}

float context::getTimerSeconds()
{
     return 0.0f;
}

void context::renderScreen( shader *s )
{
     glUseProgram( s->programID );

     glBegin( GL_QUADS );
     glTexCoord2f( 0.0f, 0.0f );
     glVertex3f( -1.0f, -1.0f, 0.0f );
     glTexCoord2f( 0.0f, 1.0f );
     glVertex3f( -1.0f, 1.0f, 0.0f );
     glTexCoord2f( 1.0f, 1.0f );
     glVertex3f( 1.0f, 1.0f, 0.0f );
     glTexCoord2f( 1.0f, 0.0f );
     glVertex3f( 1.0f, -1.0f, 0.0f );
     glEnd();
}

void context::startCamera()
{
     if (Camera != NULL)
          Camera->start();
}

void context::endCamera()
{
     if (Camera != NULL)
          Camera->end();
}

};
