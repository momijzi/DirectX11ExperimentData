

class Mesh
{
public:

	Float3 position;	//座標
	Float3 angles;		//角度
	Float3 scale;		//大きさ

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	Material material;

	Mesh()
	{
		App::Initialize();

		position = Float3(0.0f, 0.0f, 0.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		scale = Float3(1.0f, 1.0f, 1.0f);

		material = Material(L"Shader.hlsl");

		SetCullingMode(D3D11_CULL_BACK);

		CreateCube();
		Apply();
	}
	Mesh(int mode)
	{
		App::Initialize();
		
		position = Float3(0.0f, 0.0f, 0.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		scale = Float3(1.0f, 1.0f, 1.0f);

		material = Material(L"Shader.hlsl");

		SetCullingMode(D3D11_CULL_BACK);

		//作成したい物に合わせて引数を変えること
		switch (mode)
		{
			case 0:
				//軸ずらしについてはまだ
				CreateCube(Float2(0.5f, 0.5f));//Float2(横幅のサイズ、縦幅のサイズ)
				break;
			case 1:
				CreatePlane(Float2(0.5f, 0.5f), Float3(0.0f, 0.0f, -0.5f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f));
				break;
			case 2:

				break;
			default:

				break;
		}
		Apply();
	}

	~Mesh()
	{

	}

	//トライアングルリスト作成
	void CreateTriangle()
	{
		vertices.clear();

		//ベクタに要素を追加
		//ここをいじれば描画する物を変えることが可能だろう
		//できれば引数を使って動的に作りたい
		vertices.push_back(Vertex(Float3(0.0f, 1.0f, 0.0f), Float3(1.0f, 0.0f, 0.0f),Float2(0.0f,0.0f)));
		vertices.push_back(Vertex(Float3(1.0f, -1.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f),Float2(1.0f,0.0f)));
		vertices.push_back(Vertex(Float3(-1.0f, -1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f),Float2(0.0f,1.0f)));
	}

	void CreatePlane(
		Float2 size = Float2(0.5f, 0.5f),
		Float3 offset = Float3(0.0f, 0.0f, 0.0f),
		bool souldclear = true,
		Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
		Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
		Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f)
	)
	{
		if (souldclear)
		{
			vertices.clear();
			indices.clear();
		}
		leftDirection = DirectX::XMVector3Normalize(leftDirection);
		upDirection = DirectX::XMVector3Normalize(upDirection);
		forwardDirection = DirectX::XMVector3Normalize(forwardDirection);

		//
		vertices.push_back(Vertex(
			leftDirection * -size.x + upDirection *  size.y + offset,
			-forwardDirection, 
			Float2(
				0.25f,//x 1をその画像の最大として分割される
				0.0f//y　上と同じ
			)));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection *  size.y + offset,
			-forwardDirection, Float2(0.5f, 0.0f)));
		vertices.push_back(Vertex(leftDirection * -size.x + upDirection * -size.y + offset,
			-forwardDirection, Float2(0.25f, 0.5f)));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection * -size.y + offset,
			-forwardDirection, Float2(0.5f, 0.5f)));

		size_t indexOffset = vertices.size() - 4;
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 1);
	}

	void CreateCube(
		Float2 size = Float2(0.5f, 0.5f),
		bool souldClear = true
	)
	{
		if (souldClear)
		{
			vertices.clear();
			indices.clear();
		}

		CreatePlane(size, Float3( 0.0f, 0.0f,-0.5f), false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f, 1.0f));
		CreatePlane(size, Float3( 0.0f, 0.0f, 0.5f), false,Float3(-1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f,-1.0f));
		CreatePlane(size, Float3( 0.5f, 0.0f, 0.0f), false,Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3(-1.0f, 0.0f, 0.0f));
		CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false,Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 1.0f, 0.0f, 0.0f));
		CreatePlane(size, Float3( 0.0f, 0.5f, 0.0f), false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f,-1.0f, 0.0f));
		CreatePlane(size, Float3( 0.0f,-0.5f, 0.0f), false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f, 1.0f, 0.0f));
	}

	/*
	void CreateTriangleBox(float x, float y, float z)
	{
		vertices.clear();

		//上面
		setVertex[0] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　前
		setVertex[1] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　前
		setVertex[2] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　奥

		setVertex[3] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左上　奥
		setVertex[4] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　前
		setVertex[5] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　奥

		//前面
		setVertex[6] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　前
		setVertex[7] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左上　前
		setVertex[8] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　前

		setVertex[9] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　前
		setVertex[10] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　前
		setVertex[11] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　前

		//奥側
		setVertex[12] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　奥
		setVertex[13] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　奥
		setVertex[14] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　奥

		setVertex[15] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左下　奥
		setVertex[16] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　奥
		setVertex[17] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　奥

		//正面　右	側面
		setVertex[18] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　奥
		setVertex[19] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　前
		setVertex[20] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　奥

		setVertex[21] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　奥
		setVertex[22] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　前
		setVertex[23] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　前

		//正面　左　側面
		setVertex[24] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　前
		setVertex[25] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左上　奥
		setVertex[26] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　前

		setVertex[27] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左下　前
		setVertex[28] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　奥
		setVertex[29] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　奥

		//底
		setVertex[30] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　前
		setVertex[31] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左下　前
		setVertex[32] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　奥

		setVertex[33] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　奥
		setVertex[34] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　前
		setVertex[35] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　奥

		for (int i = 0; i < 36; i++)
		{
			vertices.push_back(setVertex[i]);
		}
	}
	*/

	void SetCullingMode(D3D11_CULL_MODE culingMode)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;//レンダリング時に使用する描画モードを決定する
		rasterizerDesc.CullMode = culingMode;//指定の方向を向いている三角形が描画されないことを示す
		App::GetGraphicsDevice().CreateRasterizerState//ラスタライザーステージに動作を支持するラスタライザステートオブジェクトを作成する
		(
			&rasterizerDesc, 
			&rasterizerState
		);
	}

	void Apply()
	{
		if (vertices.size() > 0)
		{
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
			vertexSubresourceData.pSysMem = vertices.data();
			//バッファの初期化
			vertexBuffer.Release();
			//バッファ作成
			App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData,&vertexBuffer);
		}
		if (indices.size() > 0)
		{
			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
			indexSubresourceData.pSysMem = indices.data();
			//バッファの初期化
			indexBuffer.Release();
			//バッファ作成
			App::GetGraphicsDevice().CreateBuffer(&indexBufferDesc,&indexSubresourceData,&indexBuffer);
		}
		material.SetBuffer(&constant, sizeof(Constant));
	}
	//描画
	void Draw()
	{
		//vertexBufferが正しく作られているかどうかの確認
		if (vertexBuffer == nullptr)
			return;

		material.Attach();

		//軸の角度変更　また座標移動　行列の掛け算をしている　掛け方をかえると変になる
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationY(angles.y) *
			DirectX::XMMatrixRotationX(angles.x) *
			DirectX::XMMatrixRotationZ(angles.z) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		);
		App::GetGraphicsContext().RSSetState(rasterizerState);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p,&stride, &offset);

		if (indexBuffer == nullptr)
		{
			App::GetGraphicsContext().Draw(vertices.size(), 0);
		}
		else
		{
			App::GetGraphicsContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			App::GetGraphicsContext().DrawIndexed(indices.size(), 0, 0);
		}
	}

private:
	struct Constant
	{
		DirectX::XMMATRIX world;
	};
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
};
