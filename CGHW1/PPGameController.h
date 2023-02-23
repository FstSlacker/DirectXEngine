#pragma once
#include "GameComponent.h"
#include "Collision.h"

class BallComonent;
class RacketComponent;

class PPGameController : public GameComponent
{
private:
	RacketComponent* playerRacket;
	RacketComponent* enemyRacket;
	BallComonent* ball;

public:
	PPGameController(Game* game, RacketComponent* playerRacket, RacketComponent* enemyRacket, BallComonent* ball);
	void Update() override;
	void FixedUpdate() override;
	void OnBallCollisionEnter(const CollisionArgs& collisionArgs);

	void MoveBall();
	void MoveRackets();
	void RestartLevel(float startBallDir);
};

