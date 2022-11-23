#pragma once
#include "Transform.h"
#include "Model.h"
#include <cassert>
#include "Input.h"


class Collision {
public:
    //�v���C���[�ƃG�l�~�[�̍��W�����
    //�v���C���[�̔��a��x,y,z�̏��ɓ����
    //�G�l�~�[�̔��a��x,y,z�̏��ɓ����
    static bool BoxCollision(AliceMathF::Vector3 player, AliceMathF::Vector3 enemy, AliceMathF::Vector3 playerWidth, AliceMathF::Vector3 enemyWidth);

private:
};
