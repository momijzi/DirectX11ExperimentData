class Player
{
public:
	Sprite head;//“ª
	Sprite body;//‘Ì
	Sprite leftArm;//¶˜r
	Sprite rightArm;//‰E˜r
	Sprite leftLeg;//¶‹r
	Sprite rightLeg;//‰E‹r

	Player()
	{
		
	}
	Player(Texture* tex)
	{
		tex->SetUVNum(0,0);
		head.CreateData(tex, 1);
		tex->SetUVNum(1,0);
		body.CreateData(tex, 1);
		tex->SetUVNum(2,0);
		leftArm.CreateData(tex, 1);
		tex->SetUVNum(3,0);
		rightArm.CreateData(tex, 1);
		tex->SetUVNum(0,1);
		leftLeg.CreateData(tex, 1);
		tex->SetUVNum(1,1);
		rightLeg.CreateData(tex, 1);

		head.position.y = 2.0f;
		
		body.position.y = -0.5f;
		body.scale.x = 2.0f;
		body.scale.y = 3.0f;

		leftArm.position.x = -2.0f;
		leftArm.scale.y = 2.0f;
		leftArm.axis.y = -leftArm.scale.y / 4;
		leftArm.position.y -= leftArm.axis.y;

		rightArm.position.x = 2.0f;
		rightArm.scale.y = 2.0f;

		leftLeg.position.x = -1.0f;
		leftLeg.position.y = -2.0f;

		rightLeg.position.x =  1.0f;
		rightLeg.position.y = -2.0f;
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