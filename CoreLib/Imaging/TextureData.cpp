#include "TextureData.h"

#ifdef TEXTURE_ACCESS_DUMP
bool EnableTextureAccessDump;
TextWriter * TextureAccessDebugWriter;
#endif

using namespace CoreLib::Basic;
using namespace CoreLib::Imaging;

namespace CoreLib
{
	namespace Imaging
	{
		void CreateDefaultTextureData(TextureData<Color> & tex)
		{
			tex.Levels.SetSize(2);
			tex.Levels[0].Width = 2;
			tex.Levels[0].Height = 2;
			tex.Levels[0].Pixels.SetSize(4);
			tex.Levels[0].Pixels[2] = tex.Levels[0].Pixels[0] = Color(255, 0, 255, 255);
			tex.Levels[0].Pixels[1] = tex.Levels[0].Pixels[3] = Color(0, 255, 0, 255);
			tex.Levels[1].Width = 1;
			tex.Levels[1].Height = 1;
			tex.Levels[1].Pixels.SetSize(1);
			tex.Levels[1].Pixels[0] = Color(127, 127, 127, 255);
			tex.Width = tex.Height = 2;
			tex.InvHeight = tex.InvWidth = 0.5f;
			tex.IsTransparent = false;
		}

		void CreateTextureDataFromBitmap(TextureData<Color4F> & tex, Bitmap & image)
		{
			tex.Levels.SetSize(CeilLog2(Math::Max(image.GetWidth(), image.GetHeight())) + 1);
			tex.Levels[0].Pixels.Reserve(image.GetWidth()*image.GetHeight());
			for (int i = image.GetHeight() - 1; i >= 0; i--)
				for (int j = 0; j < image.GetWidth(); j++)
				{
					auto color = *((Color*)image.GetPixels() + i * image.GetWidth() + j);
					Color4F cf;
					cf.x = color.R / 255.0f;
					cf.y = color.G / 255.0f;
					cf.z = color.B / 255.0f;
					cf.w = color.A / 255.0f;
					tex.Levels[0].Pixels.Add(cf);

				}
			tex.Levels[0].Width = image.GetWidth();
			tex.Levels[0].Height = image.GetHeight();
			tex.Width = image.GetWidth();
			tex.Height = image.GetHeight();
			tex.IsTransparent = image.GetIsTransparent();
			tex.GenerateMipmaps();
		}

		void CreateTextureDataFromBitmap(TextureData<Color> & tex, Bitmap & image)
		{
			tex.Levels.SetSize(CeilLog2(Math::Max(image.GetWidth(), image.GetHeight())) + 1);
			tex.Levels[0].Pixels.Reserve(image.GetWidth()*image.GetHeight());
			for (int i = image.GetHeight() - 1; i >= 0; i--)
				tex.Levels[0].Pixels.AddRange((Color*)image.GetPixels() + i *image.GetWidth(), image.GetWidth());
			tex.Levels[0].Width = image.GetWidth();
			tex.Levels[0].Height = image.GetHeight();
			tex.Width = image.GetWidth();
			tex.Height = image.GetHeight();
			tex.IsTransparent = image.GetIsTransparent();
			tex.GenerateMipmaps();
		}


		void CreateTextureDataFromFile(TextureData<Color> & tex, const Basic::String & fileName)
		{
			// Read file to Levels[0]
			Bitmap image(fileName);
			CreateTextureDataFromBitmap(tex, image);
		}

		
	}
}