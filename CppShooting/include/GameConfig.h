#pragma once
#include <windows.h> // ���z�L�[�R�[�h(VK_*)���g�p���邽�߂ɃC���N���[�h

// ============== �Q�[���ݒ�l ================

// --- �v���C���[�֘A ---
const float PLAYER_MOVE_SPEED = 0.02f;      // �v���C���[�̈ړ����x
const float PLAYER_FIRE_COOLDOWN = 1.0f;    // �e�̔��˃N�[���_�E������ (�l���������قǘA�ˑ��x���オ��)

// --- �e�֘A ---
const float BULLET_HALF_WIDTH = 0.005f;     // �e�̕��̔���
const float BULLET_HALF_HEIGHT = 0.02f;    // �e�̍����̔���
const float BULLET_SPEED = 0.05f;           // �e�̔�ԑ��x
const int MAX_BULLETS = 50;
const int MAX_OBSTACLE_BULLETS = 2000; // ��Q���̒e�̍ő吔
const float OBSTACLE_FIRE_COOLDOWN = 1.0f;    // �e�̔��˃N�[���_�E������ (�l���������قǘA�ˑ��x���オ��)

// --- ��Q���֘A ---
const float OBSTACLE_SPAWN_INTERVAL = 3.0f; // ��Q�������ɏo������܂ł̎���
const int   OBSTACLE_DEFAULT_HP = 3;         // ��Q���̏���HP
const float OBSTACLE_COLLISION_RADIUS = 0.05f; // ��Q���Ƃ̓����蔻��̔��a
const float OBSTACLE_SCALE = 1.0f;          // ��Q���̑傫���̔{�� (1.0f���)
const float OBSTACLE_BULLET_SPEED = -0.02f; // ��Q���̒e�̑��x
const float OBSTACLE_BULLET_HALF_WIDTH = 0.01f; // ��Q���̒e�̕��̔���
const float OBSTACLE_BULLET_HALF_HEIGHT = 0.02f; // ��Q���̒e�̍����̔���
const float OBSTACLE_BULLET_SPAWN_INTERVAL = 2.0f; // ��Q���̒e�����ɏo������܂ł̎���
const float OBSTACLE_BULLET_SPAWN_OFFSET = 0.05f; // ��Q���̒e�̔��ˈʒu�I�t�Z�b�g
const float OBSTACLE_BULLET_SPAWN_Y_OFFSET = 0.1f; // ��Q���̒e��Y�ʒu�I�t�Z�b�g
const float OBSTACLE_BULLET_SPAWN_X_OFFSET = 0.1f; // ��Q���̒e��X�ʒu�I�t�Z�b�g
const int MAX_OBSTACLES = 10;
// --- �Q�[���֘A ---
const float GAME_UPDATE_INTERVAL = 0.016f; // �Q�[���̍X�V�Ԋu (��60FPS)
const float GAME_OVER_DELAY = 3.0f;       // �Q�[���I�[�o�[��ʂ̕\������
const float GAME_START_DELAY = 1.0f;      // �Q�[���J�n�O�̑ҋ@����
const float GAME_PAUSE_DELAY = 0.5f;      // �Q�[���ꎞ��~�̑ҋ@����
const float GAME_SCORE_UPDATE_INTERVAL = 1.0f; // �X�R�A�X�V�̊Ԋu





// ============== �L�[�o�C���h�ݒ� ================
// �Q�[���̑���L�[��ݒ肵�܂��B
// �����L�[�� 'W' �̂悤�ɃV���O���N�H�[�g�ň͂݁A
// ����L�[�� VK_SPACE (�X�y�[�X�L�[) �̂悤�Ɏw�肵�܂��B

// --- �Q�[������ ---
const int KEY_QUIT = 'P'; // �Q�[�����I������L�[

// --- �v���C���[���� ---
const int KEY_MOVE_UP = 'W';        // ��ړ�
const int KEY_MOVE_DOWN = 'S';        // ���ړ�
const int KEY_MOVE_LEFT = 'A';        // ���ړ�
const int KEY_MOVE_RIGHT = 'D';        // �E�ړ�
const int KEY_TOGGLE_AUTOFIRE = VK_SPACE;   // �I�[�g�t�@�C�A�؂�ւ�
const int KEY_CHANGE_SHAPE_LEFT = 'Z';        // ����Ԃ̌`��ύX
const int KEY_CHANGE_SHAPE_CENTER = 'X';        // ������Ԃ̌`��ύX
const int KEY_CHANGE_SHAPE_RIGHT = 'C';        // �E��Ԃ̌`��ύX

// ==========================================