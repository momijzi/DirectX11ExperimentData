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

	bool flagPunch = false;
	bool flagReturn = false;

	Camera camera;

	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.SetDivide(Float2(4.0f, 2.0f));

	textureBox.SetUVNum(Float2(3.0f, 1.0f));
	Sprite box2;
	box2.CreateData(&textureBox, 1);

	textureBox.SetUVNum(Float2(0.0f, 0.0f));
	Sprite box;
	box.CreateData(&textureBox,1);

	box.position.y = 5.0f;
	box2.angles.z = (float)(PI / 180 * 45);

	

	//box.Draw();
	Player player(&textureBox);
	OBB obb;

	while (App::Refresh())
	{
		if (App::GetKey(VK_RIGHT))
		{
			camera.position.z += 0.1f;
			player.PlayerMove(Float3(0.3f,0.0f, 0.0f));
		}
		else if (App::GetKey(VK_LEFT))
		{
			camera.position.z-= 0.1f;
			player.PlayerMove(Float3(-0.3f, 0.0f, 0.0f));
		}
		if (App::GetKey(VK_UP))
		{
			camera.position.y += 0.1f;
			player.PlayerMove(Float3(0.0f, 0.3f, 0.0f));
		}
		else if (App::GetKey(VK_DOWN))
		{
			camera.position.y -= 0.1f;
			player.PlayerMove(Float3(0.0f, -0.3f, 0.0f));
		}
		if (App::GetKey('D'))
			camera.angles.y++;
		else if (App::GetKey('A'))
			camera.angles.y--;
		if (App::GetKey('W'))
			camera.angles.x--;
		else if (App::GetKey('S'))
			camera.angles.x++;
		camera.Update();

		/*if (App::GetKey('L'))
		{
			player.leftArm.angles.x += (float)(PI / 180 * 1);
		}
		if(App::GetKey('P'))
		{
			player.leftArm.angles.z -= (float)(PI / 180 * 10);
		}
		if (App::GetKeyDown('Z') && !flagPunch)
		{
			flagPunch = true;
		}
		
		if (flagPunch)
		{
			if (!flagReturn)
			{
				player.leftArm.position.z += 0.5f;
				if (player.leftArm.position.z > 50.0f)
				{
					flagReturn = true;
				}
			}
			else
			{
				player.leftArm.position.z -= 0.5f;
				if (player.leftArm.position.z <= 0.0f)
				{
					flagReturn = false;
					flagPunch = false;
				}
			}
		}

		player.Update();*/
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
		
		
		box.Draw();
		box2.Draw();
	}
	return 0;
}

