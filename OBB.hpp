//参考にしたサイト
//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
//有向境界ボックス
//OBBで当たり判定をとるのに必要なデータ…中心、各軸の方向ベクトル、軸方向の長さ

//当たり判定のクラス
class OBB
{
public:
	//有向境界ボックスで使用するデータ
	//struct OBBData
	//{
	//	//ボックスの中心座標
	//	Float3 OBBpos;
	//	//ボックスの方向ベクトル
	//	Float3 OBBvector[3];
	//	//ボックスの軸方向の長さ
	//	float OBBlength[3];
	//};

	//コンストラクタの中でオブジェクトのデータの中に作ったOBBようのデータを読み出す
	OBB()
	{
		App::Initialize();
	}

	~OBB()
	{

	}


	//ベクトルの単位ベクトル化
	Float3 Float3Normalize(float x, float y, float z)
	{
		int num = 0;
		num = 1 / sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		return Float3(x, y, z)*num;
	}

	//二つのベクトルの外積の計算
	Float3 Float3Cross(Float3 f1, Float3 f2)
	{
		Float3 f;
		f.x = f1.z*f2.y - f1.y*f2.z;
		f.y = f1.x*f2.z - f1.z*f2.x;
		f.z = f1.x*f2.y - f1.y*f2.x;

		return f;
	}

	//内積計算
	float Float3Dot(Float3 f1, Float3 f2)
	{
		return f1.x*f2.x + f1.y * f2.y + f1.z*f2.z;
	}

	float Float3Length(Float3 f1)
	{
		return sqrt(pow(f1.x, 2.0f) + pow(f1.y, 2.0f) + pow(f1.z, 2.0f));
	}

	//衝突判定取得
	bool OBBCheck(Sprite::OBBData obb1, Sprite::OBBData obb2)
	{
		Sprite::OBBData obb[2];
		obb[0] = obb1;
		obb[1] = obb2;
		Float3 L = obb[0].OBBpos - obb[1].OBBpos;
		float length = 0;
		float obbLength[2];
		Float3 cross;
		Float3 Ae[3];
		Float3 Be[3];
		for (int i = 0; i < 3; i++)
		{
			Ae[i] = obb[0].OBBvector[i] * obb[0].OBBlength[i] * 0.5f;
			Be[i] = obb[1].OBBvector[i] * obb[1].OBBlength[i] * 0.5f;
		}

		//obb1のx方向
		obbLength[0] = obb[0].OBBlength[0] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[0], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb1のy方向
		obbLength[0] = obb[0].OBBlength[1] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[1], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb1のz方向
		obbLength[0] = obb[0].OBBlength[2] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[2], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のx方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[0], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[0] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のy方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[1], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[1] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のz方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[2], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[2] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cross = Float3Cross(obb[0].OBBvector[i], obb[1].OBBvector[j]);

				obbLength[0] = LenSegOnSeparateAxis(&cross, &Ae[(i + 1) % 3], &Ae[(i + 2) % 3]);
				obbLength[1] = LenSegOnSeparateAxis(&cross, &Be[(j + 1) % 3], &Be[(j + 2) % 3]);
				length = fabs(Float3Dot(cross, L));
				if (length > obbLength[0] + obbLength[1])
				{
					//衝突していない
					return false;
				}
			}
		}

		return true;
	}



	bool ColOBBs(Sprite::OBBData &obb1, Sprite::OBBData &obb2)
	{
		// 各方向ベクトルの確保
		// （N***:標準化方向ベクトル）
		Float3 NAe1 = obb1.OBBvector[0], Ae1 = NAe1 * obb1.OBBlength[0];
		Float3 NAe2 = obb1.OBBvector[1], Ae2 = NAe2 * obb1.OBBlength[1];
		Float3 NAe3 = obb1.OBBvector[2], Ae3 = NAe3 * obb1.OBBlength[2];
		Float3 NBe1 = obb2.OBBvector[0], Be1 = NBe1 * obb2.OBBlength[0];
		Float3 NBe2 = obb2.OBBvector[1], Be2 = NBe2 * obb2.OBBlength[1];
		Float3 NBe3 = obb2.OBBvector[2], Be3 = NBe3 * obb2.OBBlength[2];
		Float3 Interval = obb1.OBBpos - obb2.OBBpos;

		// 分離軸 : Ae1
		FLOAT rA = Float3Length(Ae1);
		FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
		FLOAT L = fabs(Float3Dot(Interval, NAe1));
		if (L > rA + rB)
			return false; // 衝突していない

						  // 分離軸 : Ae2
		rA = Float3Length(Ae2);
		rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
		L = fabs(Float3Dot(Interval, NAe2));
		if (L > rA + rB)
			return false;

		// 分離軸 : Ae3
		rA = Float3Length(Ae3);
		rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
		L = fabs(Float3Dot(Interval, NAe3));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be1
		rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
		rB = Float3Length(Be1);
		L = fabs(Float3Dot(Interval, NBe1));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be2
		rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
		rB = Float3Length(Be2);
		L = fabs(Float3Dot(Interval, NBe2));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be3
		rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
		rB = Float3Length(Be3);
		L = fabs(Float3Dot(Interval, NBe3));
		if (L > rA + rB)
			return false;

		// 分離軸 : C11
		Float3 Cross;
		Cross = Float3Cross(NAe1, NBe1);
		rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C12
		Cross = Float3Cross(NAe1, NBe2);
		rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C13
		Cross = Float3Cross(NAe1, NBe3);
		rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C21
		Cross = Float3Cross(NAe2, NBe1);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C22
		Cross = Float3Cross(NAe2, NBe2);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C23
		Cross = Float3Cross(NAe2, NBe3);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C31
		Cross = Float3Cross(NAe3, NBe1);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C32
		Cross = Float3Cross(NAe3, NBe2);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C33
		Cross = Float3Cross(NAe3, NBe3);
		rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
		rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
		L = fabs(Float3Dot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離平面が存在しないので「衝突している」
		return true;
	}




	// 分離軸に投影された軸成分から投影線分長を算出
	FLOAT LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3 = 0)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		FLOAT r1 = fabs(Float3Dot(*Sep, *e1));
		FLOAT r2 = fabs(Float3Dot(*Sep, *e2));
		FLOAT r3 = e3 ? (fabs(Float3Dot(*Sep, *e3))) : 0;
		return r1 + r2 + r3;
	}
};