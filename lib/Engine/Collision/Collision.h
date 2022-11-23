#pragma once
#include "Transform.h"
#include "Model.h"
#include <cassert>
#include "Input.h"


class Collision {
public:
    //プレイヤーとエネミーの座標を入る
    //プレイヤーの半径をx,y,zの順に入れる
    //エネミーの半径をx,y,zの順に入れる
    static bool BoxCollision(AliceMathF::Vector3 player, AliceMathF::Vector3 enemy, AliceMathF::Vector3 playerWidth, AliceMathF::Vector3 enemyWidth);

private:
};
