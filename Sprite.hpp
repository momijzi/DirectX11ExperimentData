class Sprite
{
public:
	Float3 position;	//描画する位置
	Float3 angles;		//描画する物の角度//ラジアン角
	Float3 scale;		//描画する物のサイズ
	Float3 axis;		//軸をその物体のどれだけずらしているか

	//久保田_04_10------------------------------------------
	//有向境界ボックスで使用するデータ
	struct OBBData
	{
		//ボックスの中心座標
		Float3 OBBpos;
		//ボックスの方向ベクトル
		Float3 OBBvector[3];
		//ボックスの軸方向の長さ
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
	//スプライトの描画
	//filePathはテクスチャのファイルを渡す　例.(L"Texture/texture.jpg");
	//modeは描画したい形の設定
	void CreateData(Texture* tex,int mode)
	{
		mesh.material.SetTexture(0, tex);

		Float2 texSize(tex->GetSize().x, tex->GetSize().y);
		
		switch (mode)
		{
			case 0://UI		座標を追加しろー
				mesh.CreatePlane(tex->uv_x,tex->uv_y, tex->numUV_x,tex->numUV_y);
				break;
			case 1://四角形	上に同じ
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
		//久保田_04_10------------------------------------------
		//現在はデバッグ用に必ず処理されるDraw関数の中に記述
		//CreateCubeの中に記述するべき？
		SetOBBData();
		//------------------------------------------久保田_04_10
	}

	//-----------------------------------------------
	void SetOBBData()
	{
		//OBBの中心座標設定
		obbData.OBBpos = position;

		//OBBの辺の長さを設定
		obbData.OBBlength[0] = scale.x / 2;
		obbData.OBBlength[1] = scale.y / 2;
		obbData.OBBlength[2] = scale.z / 2;

		//オイラー角から方向ベクトルを求める
		float sinX = 0, sinY = 0, sinZ = 0;
		float cosX = 0, cosY = 0, cosZ = 0;
		float Nx = 0, Ny = 0, Nz = 0;
		sinX = sin(angles.x * 180.0 / PI);
		sinY = sin(angles.y * 180.0 / PI);
		sinZ = sin(angles.z * 180.0 / PI);
		cosX = cos(angles.x * 180.0 / PI);
		cosY = cos(angles.y * 180.0 / PI);
		cosZ = cos(angles.z * 180.0 / PI);
		//D3DXVec3Normalizeを使った方がいいが今回は処理を分かりやすくするため使用せず

		Nx = 1 / sqrt(pow(sinX, 2.0) + pow(cosX, 2.0));
		Ny = 1 / sqrt(pow(sinY, 2.0) + pow(cosY, 2.0));
		Nz = 1 / sqrt(pow(sinZ, 2.0) + pow(cosZ, 2.0));

		//OBBの方向ベクトル(単位ベクトル)を設定
		obbData.OBBvector[0] = Float3(0, Nx*sinX, Nx*cosX);
		obbData.OBBvector[1] = Float3(Ny*cosY, 0, Ny*sinY);
		obbData.OBBvector[2] = Float3(Nz*sinZ, Nz*cosZ, 0);
	}
	//------------------------------------------久保田_04_10

	OBBData GetOBBData()
	{
		return obbData;
	}
private:
	Mesh mesh;
	//久保田_04_10------------------------------------------
	OBBData obbData;
	//------------------------------------------久保田_04_10
	void Initialize() 
	{
		App::Initialize();

		//初期化
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