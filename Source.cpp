#include"App.hpp"
#include"Library.cpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Material‚ÌŽg‚¢•û
	Material material(L"Shader.hlsl");
	Drow‚Ì‘O‚Ématerial.Attach();
	*/		


	Camera camera;

	/*Sprite sprite(L"texture/TestTexture.jpg");
	sprite.scale = 2.0f;*/

	Texture texture(L"texture/TestTexture.jpg");
	Mesh mesh;
	mesh.material.SetTexture(0, &texture);

	while (App::Refresh())
	{
		camera.Update();

		/*if (App::GetKey(VK_RETURN))
		sprite.angles.z += 0.1f;
		sprite.Draw();*/


		mesh.angles.y += 0.01f;

		mesh.Draw();
	}
	return 0;
}

