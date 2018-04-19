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

	textureBox.SetUVNum(Float2(3.0f, 0.0f));
	Sprite box2;
	box2.CreateData(&textureBox, 1);

	Camera camera;
	OBB obb;
	box.position.y = -10;
	box.position.y = 10;
	box.Draw();
	box2.Draw();

	while (App::Refresh())
	{
		if (App::GetKey(VK_RIGHT))
			camera.position.x+= 0.1f;
		else if (App::GetKey(VK_LEFT))
			camera.position.x-= 0.1f;
		if (App::GetKey(VK_UP))
			camera.position.y+= 0.1f;
		else if (App::GetKey(VK_DOWN))
			camera.position.y-= 0.1f;
		if (App::GetKey('D'))
			camera.angles.y++;
		else if (App::GetKey('A'))
			camera.angles.y--;
		if (App::GetKey('W'))
			camera.angles.x--;
		else if (App::GetKey('S'))
			camera.angles.x++;
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
		if (!obb.OBBCheck(box.GetOBBData(), box2.GetOBBData()))
		{
			box.position.y -= 0.01f;
			box.angles += 0.1f;
			box2.position.y+= 0.01f;
			box2.angles += 0.1f;
		}
		
		box.Draw();
		box2.Draw();
	}
	return 0;
}

