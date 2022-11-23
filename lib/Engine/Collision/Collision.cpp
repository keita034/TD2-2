#include "Collision.h"

bool Collision::BoxCollision(AliceMathF::Vector3 player, AliceMathF::Vector3 enemy, AliceMathF::Vector3 playerWidth, AliceMathF::Vector3 enemyWidth)
{
	//ÉvÉåÉCÉÑÅ[
	float pXMin = player.x - playerWidth.x;

	float pXMax = player.x + playerWidth.x;

	float pYMin = player.y - playerWidth.y;

	float pYMax = player.y + playerWidth.y;

	float pZMin = player.z - playerWidth.z;

	float pZMax = player.z + playerWidth.z;

	//ìG
	//ïΩñ 
	float eXMin = enemy.x - enemyWidth.x;

	float eXMax = enemy.x + enemyWidth.x;

	float eYMin = enemy.y - enemyWidth.y;

	float eYMax = enemy.y + enemyWidth.y;

	float eZMin = enemy.z - enemyWidth.z;

	float eZMax = enemy.z + enemyWidth.z;

	if ((pXMin <= eXMax && pXMax >= eXMin) && (pYMin <= eYMax && pYMax >= eYMin) && (pZMin <= eZMax && pZMax >= eZMin)) {
		return true;
	}
	return false;
}
