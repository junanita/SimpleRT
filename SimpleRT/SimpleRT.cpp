#include "CoreLib/Basic.h"
#include "CoreLib/Imaging/Bitmap.h"

using namespace CoreLib::Basic;
using namespace CoreLib::Imaging;
using namespace VectorMath;

class Sphere
{
public:
	Vec3 Position;
	float Radius;
};

class CameraDescription
{
public:
	Vec3 Position;
	Vec3 Direction;
	float FOV;
	int Width, Height;
};

class Scene
{
public:
	CameraDescription Camera;
	List<Sphere> Spheres;
};

void ReadScene(Scene & scene, const char * fileName)
{
	scene.Camera.Width = 1024;
	scene.Camera.Height = 768;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		printf("Argument: FileName");

	// read scene file
	Scene scene;
	ReadScene(scene, argv[1]);

	int width = scene.Camera.Width;
	int height = scene.Camera.Height;

	List<Vec4> pixels;
	pixels.SetSize(width * height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// compute pixel(x,y)  (do ray tracing here)
			Vec4 pixel; 
			pixel.x = (sin(y/(float)height * Math::Pi * 2.0f) + 1.0f) * 0.5f; // r
			pixel.y = 0.0f; // g
			pixel.z = 0.0f; // b
			pixel.w = 1.0f; // a

			// store pixel to framebuffer
			pixels[y*width + x] = pixel;
		}
	}

	// write image to file
	ImageRef img;
	img.Height = height;
	img.Width = width;
	img.Pixels = pixels.Buffer();
	img.SaveAsPngFile(L"output.png");
	return 0;
}

