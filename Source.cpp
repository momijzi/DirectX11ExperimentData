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
	float num = 0.0f;
	float num2 = 0.0f;
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.SetDivide(Float2(4.0f, 2.0f));
	textureBox.SetUVNum(Float2(0.0f,0.0f));

	Sprite box;
	box.CreateData(&textureBox,1);

	Camera camera;

	while (App::Refresh())
	{
		if (App::GetKey(VK_RIGHT))
			camera.position.x+= 0.1;
		else if (App::GetKey(VK_LEFT))
			camera.position.x-= 0.1;
		if (App::GetKey(VK_UP))
			camera.position.z+= 0.1;
		else if (App::GetKey(VK_DOWN))
			camera.position.z-= 0.1;
		if (App::GetKey('D'))
			camera.angles.y++;
		else if (App::GetKey('A'))
			camera.angles.y--;
		if (App::GetKey('W'))
			camera.angles.x--;
		else if (App::GetKey('S'))
			camera.angles.x++;
		camera.Update();

		if (App::GetKeyDown(VK_SPACE))
		{
			num+= 1.0f;
			if (num >= textureBox.GetUV().x)
			{
				if (num2 == 0.0f)
				{
					num2 = 1.0f;
				}
				else
				{
					num2 = 0.0f;
				}
				num = 0.0f;
			}
			textureBox.SetUVNum(Float2(num,num2));
			box.CreateData(&textureBox, 1);
		}
			
		box.Draw();
	}
	return 0;
}

