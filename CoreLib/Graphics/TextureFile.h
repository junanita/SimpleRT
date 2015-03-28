#ifndef CORELIB_GRAPHICS_TEXTURE_FILE_H
#define CORELIB_GRAPHICS_TEXTURE_FILE_H

#include "../Basic.h"
#include "../LibIO.h"

namespace CoreLib
{
	namespace Graphics
	{
		enum class TextureType : short
		{
			Texture2D
		};
		enum class TextureStorageFormat : short
		{
			R8, RG8, RGB8, RGBA8,
			R_F32, RG_F32, RGB_F32, RGBA_F32
		};
		class TextureFileHeader
		{
		public:
			TextureType Type;
			TextureStorageFormat Format;
			int Width, Height;
		};

		class TextureFile
		{
		private:
			CoreLib::Basic::List<unsigned char> buffer;
			TextureStorageFormat format;
			int width, height;
			void LoadFromStream(CoreLib::IO::Stream * stream);
		public:
			TextureFile()
			{
				width = height = 0;
				format = TextureStorageFormat::RGBA8;
			}
			TextureFile(CoreLib::Basic::String fileName);
			TextureFile(CoreLib::IO::Stream * stream);
			TextureStorageFormat GetFormat()
			{
				return format;
			}
			int GetWidth()
			{
				return width;
			}
			int GetHeight()
			{
				return height;
			}
			void SaveToFile(CoreLib::Basic::String fileName);
			void SaveToStream(CoreLib::IO::Stream * stream);
			void SetData(TextureStorageFormat format, int width, int height, CoreLib::Basic::ArrayView<unsigned char> data);
			CoreLib::Basic::ArrayView<unsigned char> GetData()
			{
				return buffer.GetArrayView();
			}
		};
	}
}

#endif