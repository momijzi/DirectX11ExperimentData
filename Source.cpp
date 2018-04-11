#include"App.hpp"
#include"Library.cpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Material�̎g����
	Material material(L"Shader.hlsl");
	Drow�̑O��material.Attach();
	*/		


	Camera camera;

	Texture texture(L"texture/box.jpg");
	Mesh mesh;
	mesh.material.SetTexture(0, &texture);

	while (App::Refresh())
	{
		//ChangeSizeBox();

		camera.Update();

		mesh.angles += 0.1f;
		mesh.Draw();
	}
	return 0;
}

