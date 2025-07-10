#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Bullet.h"
#include "Obstacle.h" // Obstacle�N���X�̃w�b�_�[���C���N���[�h

// �O���錾
class Bullet;
class Obstacle;

class Graphics {
public:
    Graphics();
    ~Graphics();

    // �������ƃV���b�g�_�E��
    HRESULT Initialize(HWND hWnd);
    void Shutdown();

    // �t���[���̍X�V�ƃ����_�����O
    void Update();
    void RenderFrame();

private:
    // DirectX�C���^�[�t�F�[�X
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11VertexShader* m_pVertexShader;
    ID3D11PixelShader* m_pPixelShader;
    ID3D11InputLayout* m_pVertexLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pConstantBuffer;
    ID3D11Buffer* m_pBulletVertexBuffer;
    ID3D11Buffer* m_pObstacleVertexBuffer; // ��Q���p�̒��_�o�b�t�@

    // �Q�[���I�u�W�F�N�g
    float m_triangleX;
    float m_triangleY;

    // �e�̊Ǘ�
    static const int MAX_BULLETS = 50;
    Bullet m_bullets[MAX_BULLETS];
    float m_fireCooldown;

    // ��Q���̊Ǘ�
    static const int MAX_OBSTACLES = 10;
    Obstacle m_obstacles[MAX_OBSTACLES];
    float m_obstacleSpawnTimer;

private:
    // �e�̍X�V����
    void UpdateBullets();
    // �e�̕`�揈��
    void RenderBullets();

    // ��Q���̍X�V����
    void UpdateObstacles();
    // ��Q���̕`�揈��
    void RenderObstacles();
};