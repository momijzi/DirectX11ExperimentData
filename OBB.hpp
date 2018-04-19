#pragma once
//�Q�l�ɂ����T�C�g
//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
//�L�����E�{�b�N�X
//OBB�œ����蔻����Ƃ�̂ɕK�v�ȃf�[�^�c���S�A�e���̕����x�N�g���A�������̒���

//�����蔻��̃N���X
class OBB
{
public:
	//�L�����E�{�b�N�X�Ŏg�p����f�[�^
	//struct OBBData
	//{
	//	//�{�b�N�X�̒��S���W
	//	Float3 OBBpos;
	//	//�{�b�N�X�̕����x�N�g��
	//	Float3 OBBvector[3];
	//	//�{�b�N�X�̎������̒���
	//	float OBBlength[3];
	//};

	//�R���X�g���N�^�̒��ŃI�u�W�F�N�g�̃f�[�^�̒��ɍ����OBB�悤�̃f�[�^��ǂݏo��
	OBB()
	{
		App::Initialize();
	}

	~OBB()
	{

	}


	//�x�N�g���̒P�ʃx�N�g����
	Float3 Float3Normalize(float x, float y, float z)
	{
		int num = 0;
		num = 1 / sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		return Float3(x, y, z)*num;
	}

	//��̃x�N�g���̊O�ς̌v�Z
	Float3 Float3Cross(Float3 f1, Float3 f2)
	{
		Float3 f;
		f.x = fabs(f1.z*f2.y) - fabs(f1.y*f2.z);
		f.y = fabs(f1.x*f2.z) - fabs(f1.z*f2.x);
		f.z = fabs(f1.x*f2.y) - fabs(f1.y*f2.x);

		return f;
	}

	float Float3Dot(Float3 f1, Float3 f2)
	{
		return fabs(f1.x*f2.x) + fabs(f1.y * f2.y) + fabs(f1.z*f2.z);
	}

	//OBB�̍X�V
	void OBBUpData(Sprite::OBBData obb)
	{

	}

	//�Փ˔���擾
	bool OBBCheck(Sprite::OBBData obb1, Sprite::OBBData obb2)
	{
		Sprite::OBBData obb[2];
		obb[0] = obb1;
		obb[1] = obb2;
		Float3 L = Float3(fabs(obb[0].OBBpos.x - obb[1].OBBpos.x), fabs(obb[0].OBBpos.y - obb[1].OBBpos.y), fabs(obb[0].OBBpos.z - obb[1].OBBpos.z));
		//float length = sqrt(pow(L.x, 2.0) + pow(L.y, 2.0) + pow(L.z, 2.0));
		float length = 0;
		float obbLength[2];
		Float3 cross;

		//obb1��z����
		obbLength[0] = obb[0].OBBlength[2];
		obbLength[1] = fabs(obb[1].OBBlength[0] * obb[0].OBBvector[2].x) + 
			fabs(obb[1].OBBlength[1] * obb[0].OBBvector[2].y) + 
			fabs(obb[1].OBBlength[2] * obb[0].OBBvector[2].z);
		length = fabs(Float3Dot(obb[1].OBBvector[2], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb1��y����
		obbLength[0] = obb[0].OBBlength[1];
		obbLength[1] = fabs(obb[1].OBBlength[0] * obb[0].OBBvector[1].x) +
			fabs(obb[1].OBBlength[1] * obb[0].OBBvector[1].y) + 
			fabs(obb[1].OBBlength[2] * obb[0].OBBvector[1].z);
		length = fabs(Float3Dot(obb[1].OBBvector[1], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb1��x����
		obbLength[0] = obb[0].OBBlength[0];
		obbLength[1] = fabs(obb[1].OBBlength[0] * obb[0].OBBvector[0].x) + 
			fabs(obb[1].OBBlength[1] * obb[0].OBBvector[0].y) + 
			fabs(obb[1].OBBlength[2] * obb[0].OBBvector[0].z);
		length = fabs(Float3Dot(obb[1].OBBvector[0], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��z����
		obbLength[0] = fabs(obb[0].OBBlength[0] * obb[1].OBBvector[2].x) + 
			fabs(obb[0].OBBlength[1] * obb[1].OBBvector[2].y) + 
			fabs(obb[0].OBBlength[2] * obb[1].OBBvector[2].z);
		obbLength[1] = obb[1].OBBlength[2];
		length = fabs(Float3Dot(obb[0].OBBvector[2], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��y����
		obbLength[0] = fabs(obb[0].OBBlength[0] * obb[1].OBBvector[1].x) + 
			fabs(obb[0].OBBlength[1] * obb[1].OBBvector[1].y) + 
			fabs(obb[0].OBBlength[2] * obb[1].OBBvector[1].z);
		obbLength[1] = obb[1].OBBlength[1];
		length = fabs(Float3Dot(obb[0].OBBvector[1], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��x����
		obbLength[0] = fabs(obb[0].OBBlength[0] * obb[1].OBBvector[0].x) + 
			fabs(obb[0].OBBlength[1] * obb[1].OBBvector[0].y) + 
			fabs(obb[0].OBBlength[2] * obb[1].OBBvector[0].z);
		obbLength[1] = obb[1].OBBlength[0];
		length = fabs(Float3Dot(obb[0].OBBvector[0], L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c00
		cross = Float3Cross(obb[0].OBBvector[0], obb[1].OBBvector[0]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c01
		cross = Float3Cross(obb[0].OBBvector[0], obb[1].OBBvector[1]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c02
		cross = Float3Cross(obb[0].OBBvector[0], obb[1].OBBvector[2]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c10
		cross = Float3Cross(obb[0].OBBvector[1], obb[1].OBBvector[0]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[0] * obb[0].OBBlength[0], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c11
		cross = Float3Cross(obb[0].OBBvector[1], obb[1].OBBvector[1]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[0] * obb[0].OBBlength[0], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c12
		cross = Float3Cross(obb[0].OBBvector[1], obb[1].OBBvector[2]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[0] * obb[0].OBBlength[0], cross) +
			Float3Dot(obb[0].OBBvector[2] * obb[0].OBBlength[2], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c20
		cross = Float3Cross(obb[0].OBBvector[2], obb[1].OBBvector[0]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c21
		cross = Float3Cross(obb[0].OBBvector[2], obb[1].OBBvector[1]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[2] * obb[1].OBBlength[2], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//c22
		cross = Float3Cross(obb[0].OBBvector[2], obb[1].OBBvector[2]);
		obbLength[0] = sqrt(Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross) +
			Float3Dot(obb[0].OBBvector[1] * obb[0].OBBlength[1], cross));
		obbLength[1] = sqrt(Float3Dot(obb[1].OBBvector[0] * obb[1].OBBlength[0], cross) +
			Float3Dot(obb[1].OBBvector[1] * obb[1].OBBlength[1], cross));
		length = fabs(Float3Dot(cross, L));
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		return true;
	}
};