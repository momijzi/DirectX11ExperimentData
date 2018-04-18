class Sprite
{
public:
	Float3 position;	//描画する位置
	Float3 angles;		//描画する画像の角度
	Float3 scale;		//画像のサイズ

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

		switch (mode)
		{
			case 0://UI		座標を追加しろー
				mesh.CreatePlane(Float2(0.5f, 0.5f),tex->GetNumUV(), tex->GetUV());
				break;
			case 1://四角形	上に同じ
				mesh.CreateCube(Float2(0.5f, 0.5f),tex->GetNumUV(), tex->GetUV());
				break;
		}
		mesh.Apply();
	}
	void Draw()
	{
		mesh.position = position;
		mesh.angles = angles;
		mesh.scale = scale;
		mesh.Draw();
	}

private:
	Mesh mesh;

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
			"    return texture0.Sample(sampler0, pixel.uv);"
			"}"
		);
	}
};