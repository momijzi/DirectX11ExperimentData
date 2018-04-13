#include"App.hpp"
#include"Library.cpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Materialの使い方
	Material material(L"Shader.hlsl");
	Drowの前にmaterial.Attach();
	*/		


	Camera camera;

	Sprite sprite(L"texture/box.jpg");
	sprite.scale = 2.0f;

	//Texture texture(L"texture/box.jpg");
	//Mesh mesh;
	//mesh.material.SetTexture(0, &texture);

	while (App::Refresh())
	{
		camera.Update();

		if (App::GetKey(VK_RETURN))
		sprite.angles.z += 0.1f;
		sprite.Draw();

		//mesh.angles.x += 0.01f;
		//mesh.angles.y += 0.01f;

		//mesh.Draw();
	}
	return 0;
}

