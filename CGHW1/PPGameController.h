#pragma once
#include "GameComponent.h"
#include "Collision.h"

class BallComonent;
class RacketComponent;
class Text2DComponent;

class PPGameController : public GameComponent
{
private:
	RacketComponent* playerRacket;
	RacketComponent* enemyRacket;
	BallComonent* ball;
	Text2DComponent* scoreText;

	int scorePlayer;
	int scoreEnemy;

public:
	float MaxBallSpeed;
	float MaxEnemySpeed;
	float MaxPlayerSpeed;

	PPGameController(Game* game, RacketComponent* playerRacket, RacketComponent* enemyRacket, BallComonent* ball, Text2DComponent* scoreText);
	void Update() override;
	void FixedUpdate() override;
	void OnBallCollisionEnter(const CollisionArgs& collisionArgs);

	void MoveBall();
	void MoveRackets();
	void RestartLevel(float startBallDir);
};

