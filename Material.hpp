class Material
{
public:
	Material()
	{
		Initialize();
	}
	Material(char* source)
	{
		Initialize();
		Create(source);
	}
	Material(const wchar_t* const filePath)
	{
		Initialize();
		Load(filePath);
	}

	~Material()
	{
	}
	void Load(const wchar_t* const filePath)
	{
		std::ifstream sourceFile(filePath);
		std::istreambuf_iterator<char> iterator(sourceFile);
		std::istreambuf_iterator<char> last;
		std::string source(iterator, last);
		sourceFile.close();

		Create(source.c_str());
	}

	void SetBuffer(void* cbuffer, size_t size)
	{
		buffer = cbuffer;

		constantBuffer.Release();
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = size;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		
		App::GetGraphicsDevice().CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&constantBuffer
		);
	}
	void SetTexture(int slot, Texture* texture)
	{
		textures[slot] = texture;
	}

	void Attach()
	{
		if(vertexShader != nullptr)
		App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);

		if(pixelShader != nullptr)
		App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);

		if(inputLayout != nullptr)
		App::GetGraphicsContext().IASetInputLayout(inputLayout);

		if (buffer != nullptr)
		{
			App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0,nullptr, buffer, 0, 0);
			App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);
			App::GetGraphicsContext().HSSetConstantBuffers(0, 1, &constantBuffer.p);
			App::GetGraphicsContext().DSSetConstantBuffers(0, 1, &constantBuffer.p);
			App::GetGraphicsContext().GSSetConstantBuffers(0, 1, &constantBuffer.p);
			App::GetGraphicsContext().PSSetConstantBuffers(0, 1, &constantBuffer.p);
		}

		for (int i = 0; i < 10; i++)
		{
			if (textures[i] != nullptr)
			{
				textures[i]->Attach(i);
			}
		}
	}

private:
	void* buffer;
	Texture* textures[10];

	ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;
	ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

	void Initialize()
	{
		App::Initialize();

		for (int i = 0; i < 10; i++)
		{
			textures[i] = nullptr;
		}
	}

	void Create(const char* source)
	{
		vertexShader.Release();
		ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
		CompileShader(source, "VS", "vs_5_0", &vertexShaderBlob);
		App::GetGraphicsDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

		pixelShader.Release();
		ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
		CompileShader(source, "PS", "ps_5_0", &pixelShaderBlob);
		App::GetGraphicsDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

		inputLayout.Release();
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
		inputElementDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		App::GetGraphicsDevice().CreateInputLayout
		(
			inputElementDesc.data(),
			inputElementDesc.size(),
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			&inputLayout
		);
	}
	//void Create(const wchar_t* const filepath)
	//{
	//	vertexShader.Release();
	//	ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
	//	CompileShader(filepath, "VS", "vs_5_0", &vertexShaderBlob);
	//	App::GetGraphicsDevice().CreateVertexShader
	//	(
	//		vertexShaderBlob->GetBufferPointer(),
	//		vertexShaderBlob->GetBufferSize(),
	//		nullptr,
	//		&vertexShader
	//	);

	//	pixelShader.Release();
	//	ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	//	CompileShader(filepath, "PS", "ps_5_0", &pixelShaderBlob);
	//	App::GetGraphicsDevice().CreatePixelShader
	//	(
	//		pixelShaderBlob->GetBufferPointer(),
	//		pixelShaderBlob->GetBufferSize(),
	//		nullptr,
	//		&pixelShader
	//	);

	//	inputLayout.Release();
	//	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
	//	inputElementDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	//	inputElementDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	//	inputElementDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	//	App::GetGraphicsDevice().CreateInputLayout//入力アセンブラーステージで入力される入力バッファーデータを記述するための入力レイアウトobjの作成
	//	(
	//		inputElementDesc.data(),				//入力アセンブラーステージのデータ型配列
	//		inputElementDesc.size(),				//入力要素の配列内の入力データ型の数
	//		vertexShaderBlob->GetBufferPointer(),	//コンパイル済みのシェーダへのポインタ
	//		vertexShaderBlob->GetBufferSize(),		//コンパイル済みのシェーダサイズ
	//		&inputLayout							//作成される入力レイアウトobjへのポインタ
	//	);
	//}

	static void CompileShader(const char* const source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
	{
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		#endif

		ATL::CComPtr<ID3DBlob> errorBlob = nullptr;
		D3DCompile(source, strlen(source), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, shaderFlags, 0, out, &errorBlob);

		if (errorBlob != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			MessageBoxA(App::GetWindowHandle(), static_cast<char*>(errorBlob->GetBufferPointer()), "Shader Error", MB_OK);
		}
	}

	/*void CompileShader(const wchar_t* const filePath, const char* const entryPoint,
		const char* const shaderModel, ID3DBlob** out)
	{
		DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		#endif

		ATL::CComPtr<ID3DBlob> error = nullptr;
		D3DCompileFromFile(
			filePath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			shaderModel,
			flags, 0, out, &error);

		if (error != nullptr)
		{
			OutputDebugStringA((char*)error->GetBufferPointer());
			MessageBoxA(
				App::GetWindowHandle(),
				(char*)error->GetBufferPointer(),
				"Shader Error", MB_OK
			);
		}
			
	}*/
};
