#include "context.h"
#include "shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
//#include <wglew.h>
#include <il.h>
#include <assert.h>


namespace SCore {

	camera *context::Camera;

	namespace win32 {

		typedef struct {
			HINSTANCE hinstance;
			HWND hwnd;
			HDC hdc;
			HGLRC hglrc;
			HANDLE periodicTimer;
		} platformHandles;

		static bool applicationActive = true;
		int mouseX=0, mouseY=0;
		static int mouseCenterX = 100;
		static int mouseCenterY = 100;
		static int mouseOffsetX = 0;
		static int mouseOffsetY = 0;

		LRESULT CALLBACK glWndProc( HWND hwnd,
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam  )
		{
			switch(uMsg) {

		case WM_CREATE:
			{
				SetCursorPos( mouseCenterX, mouseCenterY );

				return 0;
				break;
			}
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
		case WM_MOUSEMOVE:
		 {
			 int px = lParam & 0xFFFF;
			 int py = lParam>>16;

			 int dst = abs(px-mouseCenterX) + abs(py-mouseCenterY);

			 if (dst > 100)
			 {
				 SetCursorPos( mouseCenterX, mouseCenterY );
				 mouseOffsetX += px-mouseCenterX;
				 mouseOffsetY += py-mouseCenterY;
				 px = 0;
				 py = 0;
			 }
			 else
			 {
				 px -= mouseCenterX;
				 py -= mouseCenterY;
			 }

			 mouseX = mouseOffsetX + px;
			 mouseY = mouseOffsetY + py;

			 return 0;
			 break;
		 }
		case WM_QUIT: {
			PostQuitMessage( 41 );
			return 0;
			break;
					  }
		case WM_CLOSE: {
			PostQuitMessage( 42 );
			return 0;
			break;
					   }
		default:
			break;
		 }

			return DefWindowProc(hwnd,uMsg,wParam,lParam);
		}

	};

	bool context::create( contextConfig* cfg )
	{
		WNDCLASSA wc;
		DWORD dwExStyle;
		DWORD dwStyle;
		int PixelFormat;

		if (platformSpecificPrivateData != NULL) return false;
		context::cfg = cfg;

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
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

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

#if 0
		// opengl 3+ init.
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttrARB =
			(PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress( "wglCreateContextAttribsARB" );

		if (wglCreateContextAttrARB == NULL)
			return false;

		static int gl3AttributeList[8] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, cfg->majorVer,
			WGL_CONTEXT_MINOR_VERSION_ARB, cfg->minorVer,
			WGL_CONTEXT_FLAGS_ARB,
			(WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB*(cfg->useCoreProfile?1:0)) +
			(WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB*(cfg->useCoreProfile?0:1)) +
			(WGL_CONTEXT_DEBUG_BIT_ARB*(cfg->debugContext?1:0)),
			0, 0
		};

		if (gl3AttributeList[5] == 0) gl3AttributeList[4] = 0;

		HGLRC hglrcnew = wglCreateContextAttrARB(c->hdc, NULL, gl3AttributeList);

		if (hglrcnew == NULL)
			return false;

		c->hglrc = hglrcnew;
		wglMakeCurrent(c->hdc, c->hglrc);
		//wglDeleteContext( tempglrc );

		assert( glGetError() == GL_NO_ERROR );
#else
		c->hglrc = tempglrc;
#endif

		// extensions
		glewInit();

		// default state
		glViewport( 0, 0, cfg->sx, cfg->sy );
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClearDepth( 10000.0f );
		glDepthRange( 0.01f, 10000.0f );
		glDepthFunc( GL_LESS );

		if (GLEW_ARB_seamless_cube_map)
			glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS_ARB );

		if (GLEW_ARB_color_buffer_float)
			glClampColorARB( GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FIXED_ONLY_ARB );

		assert( glGetError() == GL_NO_ERROR );

		// window blabla
		ShowWindow( c->hwnd, SW_SHOW );
		SetForegroundWindow( c->hwnd );
		SetFocus( c->hwnd );
		ShowCursor( FALSE );

		// image library
		ilInit();

		// Camera setup
		Camera = new camera( float(px+(cfg->sx>>1)), float(py+(cfg->sy>>1)) );
		SetCursorPos( 100, 100 );

		// Song playback
		if (cfg->audioName)
		{
			oggPlay = new oggPlayer;
			oggPlay->decodeDiskStream( cfg->audioName );
		}

		// set the data
		platformSpecificPrivateData = (void*) c;

		return glGetError() == GL_NO_ERROR;
	}

	bool context::update()
	{
		win32::platformHandles *c = (win32::platformHandles*) platformSpecificPrivateData;
		if (c == NULL) return false;

		/** start music playback */
		static bool playbackStarted = false;
		if (oggPlay && !playbackStarted)
		{
			oggPlay->startPlaying();
			playbackStarted = true;
		}

		SwapBuffers( c->hdc );

		if (win32::applicationActive)
		{
			// TODO: user interaction rewrite for freedir camera.
			Camera->update( getTimerSeconds(), 0.0f, 0.0f, 0.0f, 0.0f );
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

	double context::getTimerSeconds()
	{
		if (oggPlay)
			return oggPlay->getPlayedTimeSeconds();
		else
			return 0.0;
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

		glUseProgram( 0 );
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
