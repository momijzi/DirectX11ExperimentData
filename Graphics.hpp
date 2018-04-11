class Graphics : public App::Window::Proceedable
{
public:
	Graphics()
	{
		App::Initialize();

		UINT flags = 0;
		#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		//�h���C�o�[�̃I�v�V����
		std::vector<D3D_DRIVER_TYPE> driverTypes
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_SOFTWARE,
		};
		//Direct3D�f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�̋L�q
		std::vector<D3D_FEATURE_LEVEL> featureLevels
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		//�X���b�v�`�F�C���̐ݒ�
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = App::GetWindowSize().x;
		swapChainDesc.BufferDesc.Height = App::GetWindowSize().y;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = App::GetWindowHandle();
		swapChainDesc.Windowed = true;

		for (size_t i = 0; i < driverTypes.size(); i++)
		{
			HRESULT r =
				D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i],
					nullptr, flags, featureLevels.data(), featureLevels.size(),
					D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device,
					nullptr, &context);

			if (SUCCEEDED(r))
				break;
		}
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
		//�����_�[�^�[�Q�b�g�̂̃u�����f�B���O�X�e�[�g�̋L�q������
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = true;					//�u�����f�B���O��L���i�܂��͖����j�ɂ���
																		//�u�����f�B���O�I�v�V�����̐ݒ�
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//�ŏ���RGB�f�[�^�\�[�X���w�肷��
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//��Ԗڂ�RGB�f�[�^�\�[�X���w�肷��
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//�u�����f�B���O������RGB�f�[�^�̑g�ݍ��킹���̒�`
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//�ŏ��̃A���t�@�f�[�^�\�[�X���w�肷��
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;		//��Ԗڂ̃A���t�@�f�[�^�\�[�X���w�肷��
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//�u�����f�B���O�����̃A���t�@�f�[�^�\�[�X�̑g�ݍ��킹���@���`����
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//�������݃}�X�N
	
		//�o�b�t�@�[���\�[�X�̃f�[�^�̐ݒ�
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(Constant);
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

		App::AddProcedure(this);

		CreateRenderTarget();
	}

	~Graphics()
	{

	}
	ID3D11Device& GetDevice() const
	{
		return *device;
	}
	ID3D11DeviceContext& GetContext() const
	{
		return *context;
	}
	IDXGISwapChain& GetMemory() const
	{
		return *swapChain;
	}

	void Update()
	{
		swapChain->Present(1, 0);
		context->UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);//���\�[�X�̃T�u���\�[�X���̃f�[�^�X�V
		
		context->VSSetConstantBuffers(1, 1, &constantBuffer.p);//�萔�o�b�t�@�̐ݒ�
		context->HSSetConstantBuffers(1, 1, &constantBuffer.p);//�n���V�F�[�_�[�X�e�[�W�Ŏg�p�����萔�o�b�t�@�̐ݒ�
		context->DSSetConstantBuffers(1, 1, &constantBuffer.p);//�h���C���V�F�[�_�[�X�e�[�W�Ŏg�p�����萔�o�b�t�@�̐ݒ�
		context->GSSetConstantBuffers(1, 1, &constantBuffer.p);//�W�I���g���V�F�[�_�̃p�C�v���C���X�e�[�W�Ŏg�p�����萔�o�b�t�@�̐ݒ�
		context->PSSetConstantBuffers(1, 1, &constantBuffer.p);//�s�N�Z���V�F�[�_�[�̃p�C�v���C���X�e�[�W�Ŏg�p�����萔�o�b�t�@�̐ݒ�
		context->OMSetRenderTargets//�����_�[�^�[�Q�b�g�̐ݒ�
		(
			1,
			&renderTargetView.p, 
			depthStencilView
		);

		static float color[4] = { 1.0f,1.0f,1.0f ,1.0f };
		//�����_�[�^�[�Q�b�g�̂��ׂĂ���̐F�ɂ���
		context->ClearRenderTargetView(renderTargetView, color);
	}

private:
	struct Constant
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	Constant constant;
	ATL::CComPtr<ID3D11Device> device = nullptr;
	ATL::CComPtr<IDXGISwapChain> swapChain = nullptr;
	ATL::CComPtr<ID3D11DeviceContext> context = nullptr;
	ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
	ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;
	ATL::CComPtr<ID3D11BlendState> blendState = nullptr;

	void CreateRenderTarget()
	{
		D3D11_VIEWPORT viewPort = {};
		viewPort.Width = static_cast<float>(App::GetWindowSize().x);
		viewPort.Height = static_cast<float>(App::GetWindowSize().y);
		context->RSSetViewports(1, &viewPort);

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTexture));
		device->CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

		constant.view = DirectX::XMMatrixIdentity();
		constant.projection = DirectX::XMMatrixOrthographicLH(static_cast<float>(App::GetWindowSize().x), static_cast<float>(App::GetWindowSize().y), -10000.0f, 10000.0f);
	}
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override
	{
		if (message != WM_SIZE)
		{
			return;
		}
		//�E�B���h�E������Ă��邩�ǂ����̊m�F
		if (App::GetWindowSize().x <= 0.0f || App::GetWindowSize().y <= 0.0f)
		{
			return;
		}
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChain->GetDesc(&swapChainDesc);

		ATL::CComPtr<ID3D11RenderTargetView> nullRenderTarget = nullptr;
		context->OMSetRenderTargets(1, &nullRenderTarget, nullptr);
		renderTargetView.Release();
		renderTexture.Release();
		context->Flush();
		swapChain->ResizeBuffers(swapChainDesc.BufferCount, App::GetWindowSize().x, App::GetWindowSize().y, swapChainDesc.BufferDesc.Format, 0);
	}
};
