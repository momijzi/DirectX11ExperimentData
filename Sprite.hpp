class Sprite
{
public:
	Float3 position;	//�`�悷��ʒu
	Float3 angles;		//�`�悷�镨�̊p�x//���W�A���p
	Float3 scale;		//�`�悷�镨�̃T�C�Y
	Float3 axis;		//�������̕��̂̂ǂꂾ�����炵�Ă��邩

	Float4 BaseDir[3]
	{
		Float4(0,0,1,1),
		Float4(1,0,0,1),
		Float4(0,1,0,1)
	};

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
				mesh.CreatePlane(texSize / 2.0f);
				break;
			case 1://�l�p�`	��ɓ���
				mesh.CreateCube(tex->GetTexUVData());
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
	}
	Float4 Float4Transform(Float4 f4, DirectX::XMMATRIX *mat)
	{
		Float4 f;
		f.x = f4.x*DirectX::XMVectorGetX(mat->r[0]) + f4.y*DirectX::XMVectorGetY(mat->r[0]) + f4.z*DirectX::XMVectorGetZ(mat->r[0]) + f4.w*DirectX::XMVectorGetW(mat->r[0]);
		f.y = f4.x*DirectX::XMVectorGetX(mat->r[1]) + f4.y*DirectX::XMVectorGetY(mat->r[1]) + f4.z*DirectX::XMVectorGetZ(mat->r[1]) + f4.w*DirectX::XMVectorGetW(mat->r[1]);
		f.z = f4.x*DirectX::XMVectorGetX(mat->r[2]) + f4.y*DirectX::XMVectorGetY(mat->r[2]) + f4.z*DirectX::XMVectorGetZ(mat->r[2]) + f4.w*DirectX::XMVectorGetW(mat->r[2]);
		f.w = f4.x*DirectX::XMVectorGetX(mat->r[3]) + f4.y*DirectX::XMVectorGetY(mat->r[3]) + f4.z*DirectX::XMVectorGetZ(mat->r[3]) + f4.w*DirectX::XMVectorGetW(mat->r[3]);
		return f;
	}

	//-----------------------------------------------
	void SetOBBData()
	{
		//mat1 2�͉�]�s��
		DirectX::XMMATRIX mat1, mat2, mat3;
		mat1 = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x));
		mat2 = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y));
		mat3 = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z));
		Float4 v4[3];//��{�̌����̃x�N�g���ɍs��1,2�̉�]��K�p�������
					 //���v�Z���đ��
					 //vector4�Ȃ͍̂s��̌^��4x4�Ȃ̂ł���ɍ��킹��
		Float3 v3[3];

		for (int i = 0; i < 3; i++)
		{
			//v4[i] = DirectX::XMVector4Transform(BaseDir[i], mat1);//�s��1�̉�]�ŕϊ�
			//
			//v4[i] = DirectX::XMVector4Transform(v4[i], mat2);

			//v4[i] = DirectX::XMVector4Transform(v4[i], mat3);

			v4[i] = Float4Transform(BaseDir[i], &mat1);
			v4[i] = Float4Transform(v4[i], &mat2);
			v4[i] = Float4Transform(v4[i], &mat3);

			//vec4����vec3�Ƀf�[�^���ڂ�
			v3[i] = Float3(v4[i].x, v4[i].y, v4[i].z);
		}

		//OBB�̒��S���W�ݒ�
		obbData.OBBpos = position;

		//OBB�̕ӂ̒�����ݒ�
		obbData.OBBlength[0] = scale.x / 2;
		obbData.OBBlength[1] = scale.y / 2;
		obbData.OBBlength[2] = scale.z / 2;

		obbData.OBBvector[0] = Float3(DirectX::XMVector3Normalize(v3[0]));
		obbData.OBBvector[1] = Float3(DirectX::XMVector3Normalize(v3[1]));
		obbData.OBBvector[2] = Float3(DirectX::XMVector3Normalize(v3[2]));

	}
	//------------------------------------------�v�ۓc_04_10

	void UpdateOBBData()
	{
		SetOBBData();
	}

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
			"    return texture0.Sample(sampler0, pixel.uv);"
			"}"
		);
	}
};