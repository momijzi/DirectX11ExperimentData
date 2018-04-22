class Camera : public App::Window::Proceedable
{
public:
	enum CameraMoveMode { PLANE, SOLID };
	Float3 position;//カメラの座標
	Float3 angles;	//カメラの現在の見ている角度
	//カメラを向いている方向に移動させる時に使用する変数
	Float3 advance;	//前後
	Float3 side;	//左右

	Camera()
	{
		App::Initialize();

		position = Float3(0.0f, 0.0f, -5.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		mode = PLANE;

		SetPerspective(60.0f, 0.1f, 1000.0f);

		App::AddProcedure(this);

		Create();
	}
	Camera(CameraMoveMode mode)
	{
		App::Initialize();

		position = Float3(0.0f, 0.0f, -5.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		this->mode = mode;

		SetPerspective(60.0f, 0.1f, 1000.0f);
		
		App::AddProcedure(this);

		Create();
	}

	~Camera()
	{
		App::RemoveProcedure(this);
	}

	void ResetCameraPos()
	{
		position = Float3(0.0f, 0.0f, -5.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
	}

	//カメラの設定
	void SetPerspective(float fieldOfView, float nearClip, float farClip)
	{
		this->fieldOfView = fieldOfView;
		this->nearClip = nearClip;
		this->farClip = farClip;

		constant.projection = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixPerspectiveFovLH
			(
				DirectX::XMConvertToRadians(fieldOfView),//視野角
				App::GetWindowSize().x / (float)App::GetWindowSize().y,//ビュー空間のアスペクト比X:Y
				nearClip,//クリッピング面の近面
				farClip//				   遠面
			)
		);
	}

	void SetCameraDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-angles.y + 90)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-angles.y + 90))
				);
				
				side = Float3(
					cos(DirectX::XMConvertToRadians(-angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-angles.y))
				);
				break;
			case SOLID:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-angles.y + 90)) * cos(DirectX::XMConvertToRadians(-angles.x)),
					sin(DirectX::XMConvertToRadians(-angles.x)),
					sin(DirectX::XMConvertToRadians(-angles.y + 90)) * cos(DirectX::XMConvertToRadians(-angles.x))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-angles.y))
				);
				break;
			default:
				break;
		}
	}

	void CameraMoveAdvance(float speed)
	{
		position = position + advance * speed;
	}
	void CameraMoveSide(float speed)
	{
		position = position + side * speed;
	}

	void Update()
	{
		constant.view = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixInverse
			(
				nullptr,
				//並び替えると死にます
				DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x)) *
				DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z)) *
				DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y)) *
				DirectX::XMMatrixTranslation
				(
					position.x,
					position.y, 
					position.z
				)
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

	//平面的挙動（x,z）か立体的挙動か(x,y,z)か
	CameraMoveMode mode;

	float fieldOfView;//視野
	float nearClip;//クリッピング面の近面
	float farClip;//クリッピング面の遠面

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

		//2Dテクスチャの設定
		depthTexture.Release();
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(App::GetWindowSize().x);		//テクスチャの幅（単位　テクセル）
		textureDesc.Height = static_cast<UINT>(App::GetWindowSize().y);		//テクスチャの高さ（単位　テクセル）
		textureDesc.MipLevels = 1;											//テクスチャ内のミップマップレベルの最大数
		textureDesc.ArraySize = 1;											//テクスチャー配列内のテクスチャ―数
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;						//テクスチャフォーマット
		textureDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;		//ピクセル単位のマルチサプリングの数
		textureDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;	//イメージの品質レベル、品質が高いほどパフォーマンスは低下する
		textureDesc.Usage = D3D11_USAGE_DEFAULT;							//テクスチャの読み込み内容また書き込み方法を識別する値
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;					//パイプラインステージへのバインドに関するフラグ
		textureDesc.CPUAccessFlags = 0;										//許可するCPUアクセスの種類を指定するフラグ（論理和可能）
		textureDesc.MiscFlags = 0;											//他の一般性の低いリソースオプションを識別するフラグ
		App::GetGraphicsDevice().CreateTexture2D//3Dテクスチャの配列を作成
		(
			&textureDesc,	//2Dテクスチャの記述へのポインタ
			nullptr,		//サブリソースの記述へのポインタ（マルチサプリングされている場合はNULLにする）
			&depthTexture	//作成するテクスチャへのポインタ
		);

		//深度ステンシルビューからアクセス可能なテクスチャーのサブリソースを設定
		depthStencilView.Release();
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度ステンシルビューのフォーマット
		if (swapChainDesc.SampleDesc.Count == 0)
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;//深度ステンシルリソースへのアクセス方法
			depthStencilViewDesc.Texture2D.MipSlice = 0;//最初に使用するミップマップレベルのインデックス
		}
		else
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		App::GetGraphicsDevice().CreateDepthStencilView
		(
			depthTexture,			//深度ステンシル サーフェスとして機能するリソースへのポインタ
			&depthStencilViewDesc,	//深度ステンシルビューの記述へのポインタ
			&depthStencilView		//ID3D11DepthStencilView へのポインターのアドレス
		);

		//バッファーの作成
		constantBuffer.Release();
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Constant));	//バッファーのサイズ（バイト単位）
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファーで想定される読み込み、書き込みの方法を識別する
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファーをどのようにパイプラインにバインドするかを識別する
		constantBufferDesc.CPUAccessFlags = 0;								//CPUのアクセスフラグ
		//バッファー（頂点バッファー、インデックスバッファー、またはシェーダ定数バッファー）を作成する
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

		App::GetGraphicsContext().Flush();//コマンドバッファー内のコマンドをGPUに送信します

		//スワップチェーンのバックバッファーサイズ、フォーマット、バッファー数を変更する
		App::GetGraphicsMemory().ResizeBuffers
		(
			swapChainDesc.BufferCount,//スワップチェーンのバッファー数
			static_cast<UINT>(App::GetWindowSize().x),//バックバッファ―の新しい幅
			static_cast<UINT>(App::GetWindowSize().y),//バックバッファーの新しい高さ
			swapChainDesc.BufferDesc.Format, swapChainDesc.Flags//スワップチェーンの機能を示すフラグ
		);
		SetPerspective(fieldOfView, nearClip, farClip);
		Create();
	}
};