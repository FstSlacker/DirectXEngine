#include "PPGameController.h"
#include "BallComonent.h"
#include "RacketComponent.h"
#include "Text2DComponent.h"
#include <random>
#include <math.h>


PPGameController::PPGameController(Game* game, RacketComponent* playerRacket, RacketComponent* enemyRacket, BallComonent* ball, Text2DComponent* scoreText) : GameComponent(game, Transform3D())
{
	this->playerRacket = playerRacket;
	this->enemyRacket = enemyRacket;
	this->scoreText = scoreText;
	this->MaxBallSpeed = 2.0f;
	this->ball = ball;
	this->MaxEnemySpeed = 1.0f;
	this->MaxPlayerSpeed = 0.75f;
	this->ball->Collider->OnCollisionEnter.AddRaw(this, &PPGameController::OnBallCollisionEnter);

	playerRacket->MoveSpeed = 0.5f;
	enemyRacket->MoveSpeed = 0.5f;

	scorePlayer = 0;
	scoreEnemy = 0;
	this->scoreText->Text = std::to_wstring(scoreEnemy) + L"\n" + std::to_wstring(scorePlayer);
}

void PPGameController::Update()
{
	GameComponent::Update();
	MoveRackets();

	Vector3 ballPos = ball->Transform.GetPosition();
	float ballRadius = ball->Transform.GetScale().y * 0.5f;
	if (ballPos.y + ballRadius > 1.0f)
	{
		scorePlayer++;
		RestartLevel(-1.0f);
	}
	else if (ballPos.y - ballRadius < -1.0f)
	{
		scoreEnemy++;
		RestartLevel(1.0f);
	}
}

void PPGameController::FixedUpdate()
{
	GameComponent::FixedUpdate();
	MoveBall();
}

void PPGameController::OnBallCollisionEnter(const CollisionArgs& collisionArgs)
{
	Vector3 racketPos = collisionArgs.CollidedComponent->Transform.GetPosition();
	Vector3 racketScale = collisionArgs.CollidedComponent->Transform.GetScale();
	Vector3 ballPos = ball->Transform.GetPosition();

	float radius = ball->Transform.GetScale().x * 0.5f;
	float ySign = racketPos.y >= 0.0f ? 1.0f : -1.0f;

	ballPos.y = racketPos.y - (racketScale.y * 0.5f + radius) * ySign;

	ball->MoveDirection = (ballPos - racketPos);
	ball->MoveDirection.Normalize();

	ball->Transform.SetPosition(ballPos);

	//game->Pause();
}

void PPGameController::MoveBall()
{
	ball->Move();
}

void PPGameController::MoveRackets()
{
	//Move player racket
	if (game->Input->IsKeyDown(Keys::A)) {
		playerRacket->MoveByX(-1.0f);
	}
	if (game->Input->IsKeyDown(Keys::D)) {
		playerRacket->MoveByX(1.0f);
	}

	//Move enemy racket
	Vector3 ballPos = ball->Transform.GetPosition();
	Vector3 eRacketPos = enemyRacket->Transform.GetPosition();

	float distToBall = ballPos.x - eRacketPos.x;
	float moveDir = distToBall > 0.0f ? 1.0f : -1.0f;

	if(distToBall * moveDir > game->DeltaTime * enemyRacket->MoveSpeed)
		enemyRacket->MoveByX(moveDir);
}

void PPGameController::RestartLevel(float startBallDir)
{
	Vector3 pPos = playerRacket->Transform.GetPosition();
	Vector3 ePos = enemyRacket->Transform.GetPosition();
	Vector3 bPos = ball->Transform.GetPosition();

	pPos.x = 0.0f;
	ePos.x = 0.0f;
	bPos.x = 0.0f;
	bPos.y = 0.0f;

	playerRacket->Transform.SetPosition(pPos);
	enemyRacket->Transform.SetPosition(ePos);
	ball->Transform.SetPosition(bPos);

	const float pi = 3.14159265358979323846f;
	float rndAngle = pi * 0.5f + (rand() % 100) * 0.001f;
	

	ball->MoveDirection = Vector3(cos(rndAngle), sin(rndAngle), 0.0f) * startBallDir;
	ball->MoveSpeed += 0.1f;
	ball->MoveSpeed = ball->MoveSpeed > MaxBallSpeed ? MaxBallSpeed : ball->MoveSpeed;
	enemyRacket->MoveSpeed = enemyRacket->MoveSpeed + 0.05f > MaxEnemySpeed ? MaxEnemySpeed : enemyRacket->MoveSpeed + 0.1f;
	playerRacket->MoveSpeed = playerRacket->MoveSpeed + 0.025f > MaxPlayerSpeed ? MaxPlayerSpeed : playerRacket->MoveSpeed + 0.025f;

	scoreText->Text = std::to_wstring(scoreEnemy) + L"\n" + std::to_wstring(scorePlayer);
}
