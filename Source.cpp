#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Material‚ÌŽg‚¢•û
	Material material(L"Shader.hlsl");
	Drow‚Ì‘O‚Ématerial.Attach();
	*/	
	float num = 0.0f;

	bool flagPunch = false;
	bool flagReturn = false;

	Camera camera;

	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.SetDivide(Float2(4, 2));

	Sprite box;
	textureBox.SetUVNum(Float2(1, 1));
	box.CreateData(&textureBox, 1);

	OBB obb;

	while (App::Refresh())
	{
		if (App::GetKey(VK_RIGHT))
		{
			camera.position.x += 0.1f;
		}
		else if (App::GetKey(VK_LEFT))
		{
			camera.position.x-= 0.1f;
		}
		if (App::GetKey(VK_UP))
		{
			camera.position.z += 0.1f;
		}
		else if (App::GetKey(VK_DOWN))
		{
			camera.position.z -= 0.1f;
		}
		if (App::GetKey('D'))
			camera.angles.y += PI / 180;
		else if (App::GetKey('A'))
			camera.angles.y -= PI / 180;
		if (App::GetKey('W'))
			camera.angles.x -= PI / 180;
		else if (App::GetKey('S'))
			camera.angles.x += PI / 180;

		camera.Update();

		/*if (App::GetKeyDown(VK_SPACE))
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
		}*/
		
		/*if (!obb.OBBCheck(box.GetOBBData(), box2.GetOBBData()))
		{
			box.position.y -= 0.1f;
			num += 0.1f;
			box.angles.y += (float)(PI / 180 * num);
		}*/
	}
	return 0;
}

