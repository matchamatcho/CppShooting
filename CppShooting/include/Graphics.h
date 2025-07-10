#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Bullet.h" // Bullet�N���X�̃w�b�_�[���C���N���[�h

// �O���錾
class Bullet;

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
    ID3D11Buffer* m_pVertexBuffer;     // �O�p�`�p�̒��_�o�b�t�@
    ID3D11Buffer* m_pConstantBuffer;   // �V�F�[�_�[�Ƀf�[�^��n�����߂̃o�b�t�@

    // �������������������������������������������� �ǉ� ��������������������������������������������
    ID3D11Buffer* m_pBulletVertexBuffer; // �e�p�̒��_�o�b�t�@

    // �Q�[���I�u�W�F�N�g
    float m_triangleX; // �O�p�`��X���W
    float m_triangleY; // �O�p�`��Y���W

    // �e�̊Ǘ�
    static const int MAX_BULLETS = 50; // ��ʂɕ\���ł���e�̍ő吔
    Bullet m_bullets[MAX_BULLETS];     // �e�̔z��
    float m_fireCooldown;              // �e�̔��ˊԊu�𐧌䂷��^�C�}�[
    // �������������������������������������������� �ǉ� ��������������������������������������������

private:
    // �������������������������������������������� �ǉ� ��������������������������������������������
    // �e�̍X�V����
    void UpdateBullets();
    // �e�̕`�揈��
    void RenderBullets();
    // �������������������������������������������� �ǉ� ��������������������������������������������
};