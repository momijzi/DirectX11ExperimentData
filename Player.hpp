class Player
{
public:
	Sprite head;//ì™
	Sprite body;//ëÃ
	Sprite leftArm;//ç∂òr
	Sprite rightArm;//âEòr
	Sprite leftLeg;//ç∂ãr
	Sprite rightLeg;//âEãr

	Player()
	{
		
	}
	Player(Texture* tex)
	{
		head.CreateData(tex, 1);
		body.CreateData(tex, 1);
		leftArm.CreateData(tex, 1);
		rightArm.CreateData(tex, 1);
		leftLeg.CreateData(tex, 1);
		rightLeg.CreateData(tex, 1);

		head.position.y = 2.0f;
		
		body.position.y = -0.5f;
		body.scale.x = 2.0f;
		body.scale.y = 3.0f;

		leftArm.position.x = -2.0f;
		leftArm.scale.y = 2.0f;

		rightArm.position.x = 2.0f;
		rightArm.scale.y = 2.0f;

		leftLeg.position.x = -1.0f;
		leftLeg.position.y = -3.0f;

		rightLeg.position.x =  1.0f;
		rightLeg.position.y = -3.0f;
	}
	~Player()
	{

	}
	void Release()
	{

	}

	void PlayerMove(Float3 position)
	{
		head.position += position;
		body.position += position;
		leftArm.position += position;
		rightArm.position += position;
		leftLeg.position += position;
		rightLeg.position += position;
	}

	void Update()
	{
		head.Draw();
		body.Draw();
		leftArm.Draw();
		rightArm.Draw();
		leftLeg.Draw();
		rightLeg.Draw();
	}

private:
	
};