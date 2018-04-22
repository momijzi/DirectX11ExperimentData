#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Materialの使い方
	Material material(L"Shader.hlsl");
	Drowの前にmaterial.Attach();
	*/	
	float num = 0.0f;

	bool flagPunch = false;
	bool flagReturn = false;
	bool flagA = false;

	//textureのuvを設定するためのもの
	struct SetUvData
	{
		Float2 uvData[6] = {};
		//前面同じuvに設定
		void SetAll(Float2 numUV)
		{
			for (int i = 0; i < 6; i++)
			{
				uvData[i] = numUV;
			}
		}
		//上と下以外のuv設定
		void SetAround(Float2 numUV)
		{
			for (int i = 0; i < 4; i++)
			{
				uvData[i] = numUV;
			}
		}
		//前面のuv設定
		void SetFront(Float2 numUV)
		{
			uvData[0] = numUV;
		}
		//後面のuv設定
		void SetBack(Float2 numUV)
		{
			uvData[1] = numUV;
		}
		//左面のuv設定
		void SetLeft(Float2 numUV)
		{
			uvData[2] = numUV;
		}
		//前面のuv設定
		void SetRight(Float2 numUV)
		{
			uvData[3] = numUV;
		}
		//上面のuv設定
		void SetUp(Float2 numUV)
		{
			uvData[4] = numUV;
		}
		//下面のuv設定
		void SetDown(Float2 numUV)
		{
			uvData[5] = numUV;
		}
	};
	SetUvData setUvData;

	Camera::CameraMoveMode cameraMode = Camera::CameraMoveMode::PLANE;

	Camera camera(cameraMode);

	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(4.0f, 2.0f));

	Sprite box;
	setUvData.SetAll(Float2(0.0f,0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box.CreateData(&textureBox, 1);

	Sprite box2;
	setUvData.SetAround(Float2(2.0f, 1.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box2.CreateData(&textureBox, 1);
	
	box.position.y = 5;


	box.Draw();
	box2.Draw();
	box.SetOBBData();
	box2.SetOBBData();
	OBB obb;

	while (App::Refresh())
	{
		//カメラの操作関連
		{
			if (App::GetKey('W') || App::GetKey('A') || App::GetKey('S') || App::GetKey('D'))
			{
				if (App::GetKey('D'))
				{
					camera.angles.y += 1.0f;
				}
				else if (App::GetKey('A'))
				{
					camera.angles.y -= 1.0f;
				}
				if (App::GetKey('W'))
				{
					camera.angles.x -= 1.0f;
				}
				else if (App::GetKey('S'))
				{
					camera.angles.x += 1.0f;
				}
				camera.SetCameraDirection();
			}

			if (App::GetKey(VK_UP))
			{
				camera.CameraMoveAdvance(0.1f);
			}
			else if (App::GetKey(VK_DOWN))
			{
				camera.CameraMoveAdvance(-0.1f);
			}

			if (App::GetKey(VK_RIGHT))
			{
				camera.CameraMoveSide(0.1f);
			}
			else if (App::GetKey(VK_LEFT))
			{
				camera.CameraMoveSide(-0.1f);
			}
			camera.Update();
		}
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

		player.Update();
		*/
		
		//欠点　一回しか無理　任意にフラグを切り替える必要あり
		//複数のobjと接触判定とる場合のフラグがだる死ぬ
		if(!obb.OBBCheck(box.GetOBBData(), box2.GetOBBData()))
		{
			box.position.y -= 0.01f;

			box2.angles.y += 0.01f;
			box2.angles.z += 0.01f;

			box.SetOBBData();
			box2.SetOBBData();
		}
		box.Draw();
		box2.Draw();
	}
	return 0;
}

