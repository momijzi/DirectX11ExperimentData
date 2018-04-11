class Material
{
public:
	Material()
	{
		App::Initialize();
	}

	Material(const wchar_t* const filepath)
	{
		App::Initialize();
		Create(filepath);
	}

	~Material()
	{
		App::Initialize();

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
		if (buffer != nullptr)
		{
			App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0,
				nullptr, buffer, 0, 0);
			App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);
		}

		if(vertexShader != nullptr)
		App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);

		if(pixelShader != nullptr)
		App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);

		if(inputLayout != nullptr)
		App::GetGraphicsContext().IASetInputLayout(inputLayout);
	}

private:
	void* buffer;
	Texture* textures[10];

	ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;
	ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

	void Create(const wchar_t* const filepath)
	{
		vertexShader.Release();
		ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
		CompileShader(filepath, "VS", "vs_5_0", &vertexShaderBlob);
		App::GetGraphicsDevice().CreateVertexShader
		(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			&vertexShader
		);

		pixelShader.Release();
		ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
		CompileShader(filepath, "PS", "ps_5_0", &pixelShaderBlob);
		App::GetGraphicsDevice().CreatePixelShader
		(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			&pixelShader
		);

		inputLayout.Release();
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
		inputElementDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		App::GetGraphicsDevice().CreateInputLayout//���̓A�Z���u���[�X�e�[�W�œ��͂������̓o�b�t�@�[�f�[�^���L�q���邽�߂̓��̓��C�A�E�gobj�̍쐬
		(
			inputElementDesc.data(),				//���̓A�Z���u���[�X�e�[�W�̃f�[�^�^�z��
			inputElementDesc.size(),				//���͗v�f�̔z����̓��̓f�[�^�^�̐�
			vertexShaderBlob->GetBufferPointer(),	//�R���p�C���ς݂̃V�F�[�_�ւ̃|�C���^
			vertexShaderBlob->GetBufferSize(),		//�R���p�C���ς݂̃V�F�[�_�T�C�Y
			&inputLayout							//�쐬�������̓��C�A�E�gobj�ւ̃|�C���^
		);
	}

	void CompileShader(const wchar_t* const filePath, const char* const entryPoint,
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
			
	}
};