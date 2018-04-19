class Sprite
{
public:
	Float3 position;	//�`�悷��ʒu
	Float3 angles;		//�`�悷�镨�̊p�x//���W�A���p
	Float3 scale;		//�`�悷�镨�̃T�C�Y
	Float3 axis;		//�������̕��̂̂ǂꂾ�����炵�Ă��邩

	//�v�ۓc_04_10------------------------------------------
	//�L�����E�{�b�N�X�Ŏg�p����f�[�^
	struct OBBData
	{
		//�{�b�N�X�̒��S���W
		Float3 OBBpos;
		//�{�b�N�X�̕����x�N�g��
		Float3 OBBvector[3];
		//�{�b�N�X�̎������̒���
		float OBBlength[3];
	};
	//------------------------------------------------------

	Sprite()
	{
		Initialize();
	}
	/*Sprite(const wchar_t* const filePath)
	{
		Initialize();
		Load(filePath,0);
	}*/
	~Sprite()
	{
	}
	//�X�v���C�g�̕`��
	//filePath�̓e�N�X�`���̃t�@�C����n���@��.(L"Texture/texture.jpg");
	//mode�͕`�悵�����`�̐ݒ�
	void CreateData(Texture* tex,int mode)
	{
		mesh.material.SetTexture(0, tex);

		Float2 texSize(tex->GetSize().x, tex->GetSize().y);
		
		switch (mode)
		{
			case 0://UI		���W��ǉ�����[
				mesh.CreatePlane(tex->uv_x,tex->uv_y, tex->numUV_x,tex->numUV_y);
				break;
			case 1://�l�p�`	��ɓ���
				mesh.CreateCube(tex->uv_x, tex->uv_y, tex->numUV_x, tex->numUV_y);
				break;
		}
		mesh.Apply();
	}
	void Draw()
	{
		mesh.position = position;
		mesh.angles = angles;
		mesh.scale = scale;
		mesh.axis = axis;
		mesh.Draw();
		//�v�ۓc_04_10------------------------------------------
		//���݂̓f�o�b�O�p�ɕK�����������Draw�֐��̒��ɋL�q
		//CreateCube�̒��ɋL�q����ׂ��H
		SetOBBData();
		//------------------------------------------�v�ۓc_04_10
	}

	//-----------------------------------------------
	void SetOBBData()
	{
		//OBB�̒��S���W�ݒ�
		obbData.OBBpos = position;

		//OBB�̕ӂ̒�����ݒ�
		obbData.OBBlength[0] = scale.x / 2;
		obbData.OBBlength[1] = scale.y / 2;
		obbData.OBBlength[2] = scale.z / 2;

		//�I�C���[�p��������x�N�g�������߂�
		float sinX = 0, sinY = 0, sinZ = 0;
		float cosX = 0, cosY = 0, cosZ = 0;
		float Nx = 0, Ny = 0, Nz = 0;
		sinX = sin(angles.x * 180.0 / PI);
		sinY = sin(angles.y * 180.0 / PI);
		sinZ = sin(angles.z * 180.0 / PI);
		cosX = cos(angles.x * 180.0 / PI);
		cosY = cos(angles.y * 180.0 / PI);
		cosZ = cos(angles.z * 180.0 / PI);
		//D3DXVec3Normalize���g������������������͏����𕪂���₷�����邽�ߎg�p����

		Nx = 1 / sqrt(pow(sinX, 2.0) + pow(cosX, 2.0));
		Ny = 1 / sqrt(pow(sinY, 2.0) + pow(cosY, 2.0));
		Nz = 1 / sqrt(pow(sinZ, 2.0) + pow(cosZ, 2.0));

		//OBB�̕����x�N�g��(�P�ʃx�N�g��)��ݒ�
		obbData.OBBvector[0] = Float3(0, Nx*sinX, Nx*cosX);
		obbData.OBBvector[1] = Float3(Ny*cosY, 0, Ny*sinY);
		obbData.OBBvector[2] = Float3(Nz*sinZ, Nz*cosZ, 0);
	}
	//------------------------------------------�v�ۓc_04_10

	OBBData GetOBBData()
	{
		return obbData;
	}
private:
	Mesh mesh;
	//�v�ۓc_04_10------------------------------------------
	OBBData obbData;
	//------------------------------------------�v�ۓc_04_10
	void Initialize() 
	{
		App::Initialize();

		//������
		position = Float3(0.0f, 0.0f, 0.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		scale = Float3(1.0f, 1.0f, 1.0f);
		
		mesh.material = Material(
			"cbuffer Object : register(b0)"
			"{"
			"    matrix world;"
			"};"
			"cbuffer Camera : register(b1)"
			"{"
			"    matrix view;"
			"    matrix projection;"
			"};"
			"Texture2D texture0 : register(t0);"
			"SamplerState sampler0 : register(s0);"
			"struct Vertex"
			"{"
			"    float4 position : POSITION;"
			"    float2 uv : TEXCOORD;"
			"};"
			"struct Pixel"
			"{"
			"    float4 position : SV_POSITION;"
			"    float2 uv : TEXCOORD;"
			"};"
			"Pixel VS(Vertex vertex)"
			"{"
			"    Pixel output;"
			"    output.position = mul(vertex.position, world);"
			"    output.position = mul(output.position, view);"
			"    output.position = mul(output.position, projection);"
			"    output.uv = vertex.uv;"
			"    return output;"
			"}"
			"float4 PS(Pixel pixel) : SV_TARGET"
			"{"
			"    return float4(texture0.Sample(sampler0, pixel.uv).rgb,1);"
			"}"
		);
	}
};