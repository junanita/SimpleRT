#include "CoreLib/Basic.h"
#include "CoreLib/Imaging/Bitmap.h"

using namespace CoreLib::Basic;
using namespace CoreLib::Imaging;
using namespace VectorMath;

class Sphere
{
public:
	Vec3 Position;
	Vec3 Color;
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
	//Camera position
	scene.Camera.Width = 1024;
	scene.Camera.Height = 768;
	scene.Camera.Position = Vec3(0.0, 0.0, 0.0);
	scene.Camera.Direction = Vec3(0.0, 0.0, -1.0);
	scene.Camera.FOV = 75.0;
	
	//Sphere1 position
	Sphere sph1;
	sph1.Position = Vec3(0.0, 0.0, -20.0); 
	sph1.Radius = 5.0;
	scene.Spheres.Add(sph1);
	//printf("first sphere %f\n", sph1.Radius);


	//Sphere2 position
	Sphere sph2;
	sph2.Position = Vec3(0.0, 10.0, -30.0);
	sph2.Radius = 20.0;
	scene.Spheres.Add(sph2);
	//printf("second sphere %f\n", sph2.Radius);
}

//calculate coordinate from output image to Image View.
Vec3 ImageView(int x, int y, Scene & scene)//scene->camera
{
	Vec3 view;
	view.x = x - scene.Camera.Width / float(2.0);
	view.y = y - scene.Camera.Height / float(2.0);
	view.z = -scene.Camera.Height / float(2.0) * tan(scene.Camera.FOV / float(2.0));
	return view;
}

float dotProduct(Vec3 s, Vec3 t)
{
	float d = s.x * t.x + s.y * t.y + s.z * s.z;
	return d;
}

bool Intersection(Scene & scene, Vec3 x0, Vec3 x1)
{
	Vec3 d = x1 - x0;
	bool isIntersection = false;

	const List<Sphere> spheres = scene.Spheres;
	//printf("Spheres number %d\n", spheres.Count());
	for (int i = 0; i < spheres.Count(); i++)
	{
		Vec3 sph_co;		
		float r = spheres[i].Radius;
		//printf("Radius: %f\n", r);
		sph_co.x = spheres[i].Position.x;;
		sph_co.y = spheres[i].Position.y;
		sph_co.z = spheres[i].Position.z;

		float A = dotProduct(d, d);
		float B = 2 * dotProduct(d, x0 - sph_co);
		float C = dotProduct(x0 - sph_co, x0 - sph_co) - pow(r, 2);

		if (pow(B, 2) - 4 * A * C >= 0)
		{
			isIntersection = true;
		}
	}
	return isIntersection;
}


int main(int argc, char* argv[])
{
	if (argc < 2)
		printf("Argument: FileName\n");

	//printf("inputFile = %d\n", argv[1]);

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
			Vec3 eye;
			eye.x = 0.0;
			eye.y = 0.0;
			eye.z = 0.0;
			Vec3 view = ImageView(x, y, scene);
			bool isIntersection = Intersection(scene, eye, view);

			Vec4 pixel; 
			if (isIntersection == true)
			{
				//white
				//pixel.x = (sin(y/(float)height * Math::Pi * 2.0f) + 1.0f) * 0.5f; // r
				pixel.x = 255.0f; // r
				pixel.y = 255.0f; // g
				pixel.z = 255.0f; // b
				pixel.w = 255.0f; // a
			}
			else
			{
				//black
				pixel.x = 0.0f; // r
				pixel.y = 0.0f; // g
				pixel.z = 0.0f; // b
				pixel.w = 1.0f; // a
			}

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

