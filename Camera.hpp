class Camera : public App::Window::Proceedable
{
public:
	Float3 position;//�J�����̍��W
	Float3 angles;//�J�����̌��݂̌��Ă���p�x

	Camera()
	{
		App::Initialize();

		position = Float3(0.0f, 0.0f,-5.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);

		SetPerspective(60.0f, 0.1f, 1000.0f);

		App::AddProcedure(this);

		Create();
	}

	~Camera()
	{
		App::RemoveProcedure(this);
	}
	//�J�����̐ݒ�
	void SetPerspective(float fieldOfView, float nearClip, float farClip)
	{
		this->fieldOfView = fieldOfView;
		this->nearClip = nearClip;
		this->farClip = farClip;

		constant.projection = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixPerspectiveFovLH
			(
				DirectX::XMConvertToRadians(fieldOfView),//����p
				App::GetWindowSize().x / (float)App::GetWindowSize().y,//�r���[��Ԃ̃A�X�y�N�g��X:Y
				nearClip,//�N���b�s���O�ʂ̋ߖ�
				farClip//				   ����
			)
		);
	}
	void Update()
	{
		//�����ō��W�Ɗp�x��ς��
		constant.view = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixInverse
			(
				nullptr,
				DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z)) *
				DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y)) *
				DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x)) *
				DirectX::XMMatrixTranslation(position.x, position.y, position.z)
			)
		);

		App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
		App::GetGraphicsContext().VSSetConstantBuffers(1, 1, &constantBuffer.p);
		App::GetGraphicsContext().HSSetConstantBuffers(1, 1, &constantBuffer.p);
		App::GetGraphicsContext().DSSetConstantBuffers(1, 1, &constantBuffer.p);
		App::GetGraphicsContext().GSSetConstantBuffers(1, 1, &constantBuffer.p);
		App::GetGraphicsContext().PSSetConstantBuffers(1, 1, &constantBuffer.p);

		App::GetGraphicsContext().OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

		static float color[4] = { 1.0f,1.0f,1.0f ,1.0f };
		App::GetGraphicsContext().ClearRenderTargetView
		(
			renderTargetView, color
		);
		App::GetGraphicsContext().ClearDepthStencilView
		(
			depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0
		);
	}

private:
	struct Constant
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	float fieldOfView;//����
	float nearClip;//�N���b�s���O�ʂ̋ߖ�
	float farClip;//�N���b�s���O�ʂ̉���

	Constant constant;
	ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
	ATL::CComPtr<ID3D11Texture2D> depthTexture = nullptr;
	ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

	void Create()
	{
		renderTexture.Release();
		App::GetGraphicsMemory().GetBuffer
		(
			0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&renderTexture)
		);
		renderTargetView.Release();
		App::GetGraphicsDevice().CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		App::GetGraphicsMemory().GetDesc(&swapChainDesc);

		//2D�e�N�X�`���̐ݒ�
		depthTexture.Release();
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(App::GetWindowSize().x);		//�e�N�X�`���̕��i�P�ʁ@�e�N�Z���j
		textureDesc.Height = static_cast<UINT>(App::GetWindowSize().y);		//�e�N�X�`���̍����i�P�ʁ@�e�N�Z���j
		textureDesc.MipLevels = 1;											//�e�N�X�`�����̃~�b�v�}�b�v���x���̍ő吔
		textureDesc.ArraySize = 1;											//�e�N�X�`���[�z����̃e�N�X�`���\��
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;						//�e�N�X�`���t�H�[�}�b�g
		textureDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;		//�s�N�Z���P�ʂ̃}���`�T�v�����O�̐�
		textureDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;	//�C���[�W�̕i�����x���A�i���������قǃp�t�H�[�}���X�͒ቺ����
		textureDesc.Usage = D3D11_USAGE_DEFAULT;							//�e�N�X�`���̓ǂݍ��ݓ��e�܂��������ݕ��@�����ʂ���l
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;					//�p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
		textureDesc.CPUAccessFlags = 0;										//������CPU�A�N�Z�X�̎�ނ��w�肷��t���O�i�_���a�\�j
		textureDesc.MiscFlags = 0;											//���̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O
		App::GetGraphicsDevice().CreateTexture2D//3D�e�N�X�`���̔z����쐬
		(
			&textureDesc,	//2D�e�N�X�`���̋L�q�ւ̃|�C���^
			nullptr,		//�T�u���\�[�X�̋L�q�ւ̃|�C���^�i�}���`�T�v�����O����Ă���ꍇ��NULL�ɂ���j
			&depthTexture	//�쐬����e�N�X�`���ւ̃|�C���^
		);

		//�[�x�X�e���V���r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X��ݒ�
		depthStencilView.Release();
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�X�e���V���r���[�̃t�H�[�}�b�g
		if (swapChainDesc.SampleDesc.Count == 0)
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;//�[�x�X�e���V�����\�[�X�ւ̃A�N�Z�X���@
			depthStencilViewDesc.Texture2D.MipSlice = 0;//�ŏ��Ɏg�p����~�b�v�}�b�v���x���̃C���f�b�N�X
		}
		else
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		App::GetGraphicsDevice().CreateDepthStencilView
		(
			depthTexture,			//�[�x�X�e���V�� �T�[�t�F�X�Ƃ��ċ@�\���郊�\�[�X�ւ̃|�C���^
			&depthStencilViewDesc,	//�[�x�X�e���V���r���[�̋L�q�ւ̃|�C���^
			&depthStencilView		//ID3D11DepthStencilView �ւ̃|�C���^�[�̃A�h���X
		);

		//�o�b�t�@�[�̍쐬
		constantBuffer.Release();
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Constant));	//�o�b�t�@�[�̃T�C�Y�i�o�C�g�P�ʁj
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�[�őz�肳���ǂݍ��݁A�������݂̕��@�����ʂ���
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@�[���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩�����ʂ���
		constantBufferDesc.CPUAccessFlags = 0;								//CPU�̃A�N�Z�X�t���O
		//�o�b�t�@�[�i���_�o�b�t�@�[�A�C���f�b�N�X�o�b�t�@�[�A�܂��̓V�F�[�_�萔�o�b�t�@�[�j���쐬����
		App::GetGraphicsDevice().CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	}
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override
	{
		if (message != WM_SIZE)
		{
			return;
		}
		if (App::GetWindowSize().x <= 0.0f || App::GetWindowSize().y <= 0.0f)
		{
			return;
		}
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		App::GetGraphicsMemory().GetDesc(&swapChainDesc);

		ATL::CComPtr<ID3D11RenderTargetView> nullRenderTarget = nullptr;
		ATL::CComPtr<ID3D11DepthStencilView> nullDepthStencil = nullptr;

		App::GetGraphicsContext().OMSetRenderTargets
		(
			1,
			&nullRenderTarget,
			nullDepthStencil
		);

		renderTargetView.Release();
		depthStencilView.Release();
		renderTexture.Release();
		depthTexture.Release();

		App::GetGraphicsContext().Flush();//�R�}���h�o�b�t�@�[���̃R�}���h��GPU�ɑ��M���܂�

		//�X���b�v�`�F�[���̃o�b�N�o�b�t�@�[�T�C�Y�A�t�H�[�}�b�g�A�o�b�t�@�[����ύX����
		App::GetGraphicsMemory().ResizeBuffers
		(
			swapChainDesc.BufferCount,//�X���b�v�`�F�[���̃o�b�t�@�[��
			static_cast<UINT>(App::GetWindowSize().x),//�o�b�N�o�b�t�@�\�̐V������
			static_cast<UINT>(App::GetWindowSize().y),//�o�b�N�o�b�t�@�[�̐V��������
			swapChainDesc.BufferDesc.Format, swapChainDesc.Flags//�X���b�v�`�F�[���̋@�\�������t���O
		);
		SetPerspective(fieldOfView, nearClip, farClip);
		Create();
	}
};