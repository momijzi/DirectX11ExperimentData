class Texture
{
public:
	struct TexUVData
	{
		Float2 uv;			//�摜������ 1���番����-1
		Float2 numUV[6];	//���������ꍇ�ǂ���`�悷�邩(�l�p)

		TexUVData()
		{
			uv = Float2(1.0f, 1.0f);
			for (int i = 0; i < 6;i++)
			{
				numUV[i] = Float2(0.0f, 0.0f);
			}
		}

		//�����������������ĕ�����̑傫��������
		void SetDivide(Float2 uv)
		{
			if (uv.x >= 0 && uv.y >= 0)
				this->uv = uv;
		}

		//�`�悵�����ꏊ(�l�p�̏ꍇ6�ʕ���uv���w�肷�邱��)
		void SetUVNum(Float2 numUV[])
		{
			//��������1�ȏ�
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
			CoCreateInstance//�w�肳�ꂽCLSID�Ɋ֘A�t�����ꂽ�N���X�̏���������Ă��Ȃ�obj�̍쐬
			(
				CLSID_WICImagingFactory,//�I�u�W�F�N�g�̍쐬�Ɏg�p�����f�[�^�A�R�[�h�Ɋ֘A�t�����ꂽCLSID
				nullptr,				//NULL�̏ꍇobj���W��̈ꕔ�Ƃ��č쐬����Ȃ����Ƃ������A
										//NULL�ł͂Ȃ��Ƃ��͏W��obj��IUnknowm�C���^�t�F�[�X�ւ̃|�C���^
				CLSCTX_INPROC_SERVER,	//�V�����쐬���ꂽobj���Ǘ�����R�[�h�����s�����R���e�L�X�g
				IID_PPV_ARGS(&factory)	//obj�Ƃ̒ʐM�Ɏg�p�����C���^�[�t�F�[�X�̎��ʎq�ւ̎Q��
			);
		}
			

		ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;

		factory->CreateDecoderFromFilename//IWICBitmapDecoder�N���X�̐V�����C���X�^���X���쐬
		(
			filePath,						//�쐬�܂��̓I�[�v������obj�̖��O���w�肷��k���I��������ւ̃|�C���^
			0,								//�D��f�R�[�_��GUID�B�D��x���_�[�������ꍇ��NULL���g�p
			GENERIC_READ,					//�I�u�W�F�N�g�ւ̃A�N�Z�X�̎d���̒�`
			WICDecodeMetadataCacheOnDemand,	//�f�[�R�[�h�I�v�V�����̎w��
			&decoder						//�����炵��IWICBitmapDecoder�ւ̃|�C���^���󂯎��|�C���^
		);

		ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
		decoder->GetFrame
		(
			0,		//�擾�������̃t���[��
			&frame	//IWICBitmapFrameDecode�ւ̃|�C���^���󂯎��|�C���^
		);
		UINT width, height;
		frame->GetSize//�r�b�g�}�b�v�̃s�N�Z�����ƍ����̎擾
		(
			&width,//�s�N�Z����
			&height//		 ����
		);

		WICPixelFormatGUID pixelFormat = {};
		frame->GetPixelFormat	//�r�b�g�}�b�v�\�[�X�̃s�N�Z���`�����擾���܂�
		(
			&pixelFormat		//�r�b�g�}�b�v���i�[����Ă���s�N�Z���`����GUID���󂯎��
		);
		std::unique_ptr<BYTE[]> buffer//�o�C�g�^�̔z��𓮓I�m��
		(
			new BYTE[width * height * 4]
		);

		if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
		{
			ATL::CComPtr<IWICFormatConverter> formatConverter = nullptr;
			factory->CreateFormatConverter	//IWICFormatConverter�N���X�̐V�����C���X�^���X���쐬����
			(
				&formatConverter//�V����IWICFormatConverter�ւ̃|�C���^���擾����|�C���^
			);

			formatConverter->Initialize//�t�H�[�}�b�g�R���o�[�^������������
			(
				frame,								//�ϊ�������̓r�b�g�}�b�v
				GUID_WICPixelFormat32bppRGBA,		//����s�N�Z���`��GUID
				WICBitmapDitherTypeErrorDiffusion,	//�摜�t�H�[�}�b�g�Ԃŕϊ�����Ƃ��ɓK�p�����f�B�U�A���S���Y���^�C�v�̎w��
				0,									//�ϊ��Ɏg�p����p���b�g
				0,									//�ϊ��Ɏg�p����A���t�@���l
				WICBitmapPaletteTypeCustom			//�ϊ��Ɏg�p����p���b�g�ϊ��^�C�v
			);

			formatConverter->CopyPixels//�r�b�g�}�b�v�̃s�N�Z���f�[�^���R�s�[����
			(
				0,					//�R�s�[����Z�`�BNULL�̏ꍇ�A�S�̂̃r�b�g�}�b�v���w�肷��
				width * 4,			//�r�b�g�}�b�v�̉����iwiki���ƃX�g���C�h�����c�c�j
				width* height * 4,	//�o�b�t�@�[�̃T�C�Y
				buffer.get()		//�o�b�t�@�̃|�C���^
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
		
		//2D�e�N�X�e���ɂ���
		texture.Release();
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;							//�e�N�X�e���̕��i�e�N�Z���P�ʁj
		textureDesc.Height = height;						//�e�N�X�`���̍����i�e�N�Z���P�ʁj
		textureDesc.MipLevels = 1;							//�e�N�X�`�����̃~�b�v�}�b�v�T�C�Y���x���̍ő吔
		textureDesc.ArraySize = 1;							//�e�N�X�`���̔z����̃e�N�X�`���̐�
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�e�N�X�`���̃t�H�[�}�b�g
		textureDesc.SampleDesc.Count = 1;					//�s�N�Z���P�ʂ̃}���`�T�v�����O�̐�
		textureDesc.SampleDesc.Quality = 0;					//�C���[�W�̕i�����x���B�i���������قǃp�t�H�[�}���X���ቺ����
		textureDesc.Usage = D3D11_USAGE_DEFAULT;			//�e�N�X�`���̓ǂݍ��݂܂��������ݕ��@�����ʂ���l
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�p�C�v���C���X�e�[�W�ւ̃o�C���h�Ɋւ���t���O
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;	//������CPU�A�N�Z�X�̎�ނ��w�肷��t���O
		textureDesc.MiscFlags = 0;							//�ق��̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O

		//�T�u���\�[�X�̏������Ɏg�p�����f�[�^�̎w��
		D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
		textureSubresourceData.pSysMem = buffer.get();					//�������f�[�^�ւ̃|�C���^
		textureSubresourceData.SysMemPitch = width * 4;					//�e�N�X�`���ɂ���{�̐��̐�[����ׂ̐��܂ł̋����i�o�C�g�P�ʁj
		textureSubresourceData.SysMemSlicePitch = width * height * 4;	//��̐[�x���x������ׂ̐[�x���x���܂ł̋����i�o�C�g�P�ʁj
		
		texture.Release();
		//��̃f�[�^���g��2D�e�N�X�`���̔z����쐬����
		App::GetGraphicsDevice().CreateTexture2D
		(
			&textureDesc,			//2D�e�N�X�`���L�q�̃|�C���^
			&textureSubresourceData,//�T�u���\�[�X�̋L�q�̔z��̃|�C���^
			&texture				//�쐬�����e�N�X�`���ւ̃|�C���^�ւ̃A�h���X
		);

		//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
		shaderResourceView.Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//�\���t�H�[�}�b�g���w�肷��
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//�r���[�̃��\�[�X�^�C�v
		shaderResourceViewDesc.Texture2D.MipLevels = 1;							//�e�N�X�`�����̃~�b�v�T�C�Y���x���̍ő吔
		App::GetGraphicsDevice().CreateShaderResourceView						//�V�F�[�_�̃��\�[�X�r���[���쐬����
		(
			texture,				//�V�F�[�_�ւ̓��͂Ƃ��ċ@�\���郊�\�[�X�ւ̃|�C���^
			&shaderResourceViewDesc,//�V�F�[�_���\�[�X�r���[�ݒ�̃|�C���^
			&shaderResourceView		//ID3D11ShaderResourceView�ւ̃|�C���^�̃A�h���X
		);

		//�T���v���[�X�e�[�g
		samplerState.Release();
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//�e�N�X�`���̃T���v�����O���Ɏg�p����t�B���^�����O���\�b�h
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//0 �` 1�͈̔͊O�ɂ���u�e�N�X�`���[���W���������邽�߂ɕK�v�ȃ��\�b�h
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//					v
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//					w
		samplerDesc.MipLODBias = 0.0f;							//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g
		samplerDesc.MaxAnisotropy = 1;							//Filter��_ANISOTROPIC�����Ă���Ƃ��Ɏg�p�����N�����v�l
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	//�����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�
		samplerDesc.BorderColor[0] = 0.0f;						//AddressU�AAddressV�AAddressW�� D3D11_TEXTURE_ADDRESS_BORDER
		samplerDesc.BorderColor[1] = 0.0f;						//���w�肳��Ă���Ƃ��Ɏg�p����鋫�E�̐F�A0.0 �` 1.0�͈̔͂Ŏw��
		samplerDesc.BorderColor[2] = 0.0f;						//
		samplerDesc.BorderColor[3] = 0.0f;						//
		samplerDesc.MinLOD = 0.0f;								//�A�N�Z�X���N�����v����~�b�v�}�b�v�T�C�Y�̉���
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;					//											���
		App::GetGraphicsDevice().CreateSamplerState				//�e�N�X�`���[�̃T�v�����O�����J�v�Z��������T���v���[�X�e�[�gobj�̍쐬		
		(
			&samplerDesc,	//�T���v���[�X�e�[�g�̋L�q�ւ̃|�C���^
			&samplerState	//�쐬�����T���v���[�X�e�[�gobj�ւ̃|�C���^�̃A�h���X
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

		App::GetGraphicsContext().PSSetShaderResources//�s�N�Z���V�F�[�_�X�e�[�W�ɃV�F�[�_�̃��\�[�X�̔z����o�C���h����
		(
			slot,					//�f�o�C�X�̔z��̒��ŃV�F�[�_���\�[�X�̐ݒ�J�n�ʒu�́A0����n�܂�C���f�b�N�X
			1,						//�ݒ肷��V�F�[�_���\�[�X�̐�
			&shaderResourceView.p	//�f�o�C�X�ɐݒ肳���V�F�[�_���\�[�X�r���[�C���^�[�t�F�[�X�̔z��
		);
		App::GetGraphicsContext().PSSetSamplers//�T���v���[�X�e�[�g�̔z����s�N�Z���V�F�[�_�̃p�C�v���C���X�e�[�W�ɐݒ肷��
		(
			slot,			//��ɓ���
			1,				//�z����̃T���v���[�̐�
			&samplerState.p	//�T���v���[�X�e�[�g�C���^�t�F�[�X�̔z��ւ̃|�C���^
		);
	}

private:
	DirectX::XMINT2 size;
	
	ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
	ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};