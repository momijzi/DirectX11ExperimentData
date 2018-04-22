class Texture
{
public:
	struct TexUVData
	{
		Float2 uv;			//画像分割数 1から分割数-1
		Float2 numUV[6];	//分割した場合どこを描画するか(四角)

		TexUVData()
		{
			uv = Float2(1.0f, 1.0f);
			for (int i = 0; i < 6;i++)
			{
				numUV[i] = Float2(0.0f, 0.0f);
			}
		}

		//引数分だけ分割して分割一つの大きさを入れる
		void SetDivide(Float2 uv)
		{
			if (uv.x >= 0 && uv.y >= 0)
				this->uv = uv;
		}

		//描画したい場所(四角の場合6面分のuvを指定すること)
		void SetUVNum(Float2 numUV[])
		{
			//分割数は1以上
			for (int i = 0; i < 6; i++)
			{
				if (numUV[i].x >= 0 && numUV[i].y >= 0 && numUV[i].x < uv.x && numUV[i].y < uv.y)
					this->numUV[i] = numUV[i];
			}
		}
	};
	TexUVData texUVData;

	Texture()
	{
		App::Initialize();
	}
	Texture(const wchar_t* const filePath)
	{
		App::Initialize();
		Load(filePath);
	}
	~Texture()
	{

	}
	
	
	TexUVData GetTexUVData()
	{
		return texUVData;
	}

	void Load(const wchar_t* const filePath)
	{
		static ATL::CComPtr<IWICImagingFactory> factory = nullptr;
		if (factory == nullptr)
		{
			CoCreateInstance//指定されたCLSIDに関連付けされたクラスの初期化されていないobjの作成
			(
				CLSID_WICImagingFactory,//オブジェクトの作成に使用されるデータ、コードに関連付けされたCLSID
				nullptr,				//NULLの場合objが集約の一部として作成されないことを示す、
										//NULLではないときは集約objのIUnknowmインタフェースへのポインタ
				CLSCTX_INPROC_SERVER,	//新しく作成されたobjを管理するコードが実行されるコンテキスト
				IID_PPV_ARGS(&factory)	//objとの通信に使用されるインターフェースの識別子への参照
			);
		}
			

		ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;

		factory->CreateDecoderFromFilename//IWICBitmapDecoderクラスの新しいインスタンスを作成
		(
			filePath,						//作成またはオープンするobjの名前を指定するヌル終了文字列へのポインタ
			0,								//優先デコーダのGUID。優先ベンダーが無い場合はNULLを使用
			GENERIC_READ,					//オブジェクトへのアクセスの仕方の定義
			WICDecodeMetadataCacheOnDemand,	//デーコードオプションの指定
			&decoder						//あたらしいIWICBitmapDecoderへのポインタを受け取るポインタ
		);

		ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
		decoder->GetFrame
		(
			0,		//取得する特定のフレーム
			&frame	//IWICBitmapFrameDecodeへのポインタを受け取るポインタ
		);
		UINT width, height;
		frame->GetSize//ビットマップのピクセル幅と高さの取得
		(
			&width,//ピクセル幅
			&height//		 高さ
		);

		WICPixelFormatGUID pixelFormat = {};
		frame->GetPixelFormat	//ビットマップソースのピクセル形式を取得します
		(
			&pixelFormat		//ビットマップが格納されているピクセル形式のGUIDを受け取る
		);
		std::unique_ptr<BYTE[]> buffer//バイト型の配列を動的確保
		(
			new BYTE[width * height * 4]
		);

		if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
		{
			ATL::CComPtr<IWICFormatConverter> formatConverter = nullptr;
			factory->CreateFormatConverter	//IWICFormatConverterクラスの新しいインスタンスを作成する
			(
				&formatConverter//新しいIWICFormatConverterへのポインタを取得するポインタ
			);

			formatConverter->Initialize//フォーマットコンバータを初期化する
			(
				frame,								//変換する入力ビットマップ
				GUID_WICPixelFormat32bppRGBA,		//宛先ピクセル形式GUID
				WICBitmapDitherTypeErrorDiffusion,	//画像フォーマット間で変換するときに適用されるディザアルゴリズムタイプの指定
				0,									//変換に使用するパレット
				0,									//変換に使用するアルファ闘値
				WICBitmapPaletteTypeCustom			//変換に使用するパレット変換タイプ
			);

			formatConverter->CopyPixels//ビットマップのピクセルデータをコピーする
			(
				0,					//コピーする短形。NULLの場合、全体のビットマップを指定する
				width * 4,			//ビットマップの横幅（wikiだとストライドだが……）
				width* height * 4,	//バッファーのサイズ
				buffer.get()		//バッファのポインタ
			);
		}
		else
		{
			frame->CopyPixels
			(
				0,
				width * 4,
				width * height * 4,
				buffer.get()
			);
		}
		size = DirectX::XMINT2(width, height);
		
		//2Dテクステャについて
		texture.Release();
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;							//テクステャの幅（テクセル単位）
		textureDesc.Height = height;						//テクスチャの高さ（テクセル単位）
		textureDesc.MipLevels = 1;							//テクスチャ内のミップマップサイズレベルの最大数
		textureDesc.ArraySize = 1;							//テクスチャの配列内のテクスチャの数
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//テクスチャのフォーマット
		textureDesc.SampleDesc.Count = 1;					//ピクセル単位のマルチサプリングの数
		textureDesc.SampleDesc.Quality = 0;					//イメージの品質レベル。品質が高いほどパフォーマンスが低下する
		textureDesc.Usage = D3D11_USAGE_DEFAULT;			//テクスチャの読み込みまた書き込み方法を識別する値
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//パイプラインステージへのバインドに関するフラグ
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;	//許可するCPUアクセスの種類を指定するフラグ
		textureDesc.MiscFlags = 0;							//ほかの一般性の低いリソースオプションを識別するフラグ

		//サブリソースの初期化に使用されるデータの指定
		D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
		textureSubresourceData.pSysMem = buffer.get();					//初期化データへのポインタ
		textureSubresourceData.SysMemPitch = width * 4;					//テクスチャによる一本の線の先端から隣の線までの距離（バイト単位）
		textureSubresourceData.SysMemSlicePitch = width * height * 4;	//一つの深度レベルから隣の深度レベルまでの距離（バイト単位）
		
		texture.Release();
		//上のデータを使い2Dテクスチャの配列を作成する
		App::GetGraphicsDevice().CreateTexture2D
		(
			&textureDesc,			//2Dテクスチャ記述のポインタ
			&textureSubresourceData,//サブリソースの記述の配列のポインタ
			&texture				//作成されるテクスチャへのポインタへのアドレス
		);

		//シェーダーリソースビューの設定
		shaderResourceView.Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//表示フォーマットを指定する
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//ビューのリソースタイプ
		shaderResourceViewDesc.Texture2D.MipLevels = 1;							//テクスチャ内のミップサイズレベルの最大数
		App::GetGraphicsDevice().CreateShaderResourceView						//シェーダのリソースビューを作成する
		(
			texture,				//シェーダへの入力として機能するリソースへのポインタ
			&shaderResourceViewDesc,//シェーダリソースビュー設定のポインタ
			&shaderResourceView		//ID3D11ShaderResourceViewへのポインタのアドレス
		);

		//サンプラーステート
		samplerState.Release();
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//テクスチャのサンプリング時に使用するフィルタリングメソッド
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0 ～ 1の範囲外にあるuテクスチャー座標を解決するために必要なメソッド
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//					v
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//					w
		samplerDesc.MipLODBias = 0.0f;							//計算されたミップマップレベルからのオフセット
		samplerDesc.MaxAnisotropy = 1;							//Filterに_ANISOTROPICがついているときに使用されるクランプ値
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//既存のサンプリングデータに対してデータを比較する関数
		samplerDesc.BorderColor[0] = 0.0f;						//AddressU、AddressV、AddressWに D3D11_TEXTURE_ADDRESS_BORDER
		samplerDesc.BorderColor[1] = 0.0f;						//が指定されているときに使用される境界の色、0.0 ～ 1.0の範囲で指定
		samplerDesc.BorderColor[2] = 0.0f;						//
		samplerDesc.BorderColor[3] = 0.0f;						//
		samplerDesc.MinLOD = 0.0f;								//アクセスをクランプするミップマップサイズの下限
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;					//											上限
		App::GetGraphicsDevice().CreateSamplerState				//テクスチャーのサプリング情報をカプセル化するサンプラーステートobjの作成		
		(
			&samplerDesc,	//サンプラーステートの記述へのポインタ
			&samplerState	//作成されるサンプラーステートobjへのポインタのアドレス
		);
	} 

	DirectX::XMINT2 GetSize() const
	{
		return size;
	}

	void Attach(int slot)
	{
		if (texture == nullptr)
			return;

		App::GetGraphicsContext().PSSetShaderResources//ピクセルシェーダステージにシェーダのリソースの配列をバインドする
		(
			slot,					//デバイスの配列の中でシェーダリソースの設定開始位置の、0から始まるインデックス
			1,						//設定するシェーダリソースの数
			&shaderResourceView.p	//デバイスに設定されるシェーダリソースビューインターフェースの配列
		);
		App::GetGraphicsContext().PSSetSamplers//サンプラーステートの配列をピクセルシェーダのパイプラインステージに設定する
		(
			slot,			//上に同じ
			1,				//配列内のサンプラーの数
			&samplerState.p	//サンプラーステートインタフェースの配列へのポインタ
		);
	}

private:
	DirectX::XMINT2 size;
	
	ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
	ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};