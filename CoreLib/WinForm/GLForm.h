#ifndef CORE_LIB_GL_FORM_H
#define CORE_LIB_GL_FORM_H

#include "WinApp.h"
#include "WinForm.h"
#include "../GL/gl.h"

namespace CoreLib
{
	namespace WinForm
	{
		class GLForm : public BaseForm
		{
		protected:
			virtual void Create()
			{
				handle = ::CreateWindow(Application::GLFormClassName, 0, WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, Application::GetHandle(), NULL);
				if (!handle)
				{
					throw "Failed to create window.";
				}
				Application::RegisterComponent(this);
				SubClass();
				InitGL();
			}
		protected:
			HGLRC  hRC;
			HDC hDC;
			void InitGL()
			{
				PIXELFORMATDESCRIPTOR pf=				
				{
					sizeof(PIXELFORMATDESCRIPTOR),		
						1,								
						PFD_DRAW_TO_WINDOW |			
						PFD_SUPPORT_OPENGL |			
						PFD_DOUBLEBUFFER,				
						PFD_TYPE_RGBA,					
						24,								
						0, 0, 0, 0, 0, 0,				
						0,								
						0,								
						0,								
						0, 0, 0, 0,						
						32,								
						0,								
						0,								
						PFD_MAIN_PLANE,					
						0,								
						0, 0, 0							
				};
				hDC = GetDC(GetHandle());
				int idx= ChoosePixelFormat(hDC,&pf);
				SetPixelFormat(hDC,idx,&pf);

				hRC = wglCreateContext(hDC);
				wglMakeCurrent(hDC,hRC);

				glViewport(0,0, GetClientWidth(), GetClientHeight());
			}
			void FinalizeGL()
			{
				wglDeleteContext(hRC);
			}
			bool ProcessMessage(WinMessage & msg)
			{
				bool proc = BaseForm::ProcessMessage(msg);
				if (msg.message == WM_SIZE)
				{
					BeginGLDrawing();
					glViewport(0,0, GetClientWidth(), GetClientHeight());
				}
				return proc;
			}
		public:
			void BeginGLDrawing()
			{
				int rs = wglMakeCurrent(hDC,hRC);
				if (rs == FALSE)
					throw 0;
			}
			void EndGLDrawing()
			{
				SwapBuffers(hDC);
				wglMakeCurrent( NULL, NULL);
			}
			GLForm()
			{
				Create();
			}
			~GLForm()
			{
				Application::UnRegisterComponent(this);
				FinalizeGL();
			}
		};
	}
}

#endif