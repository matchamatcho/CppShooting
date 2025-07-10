#pragma once
#include <windows.h>
#include <d3d11.h>
#include "Game.h" // Game�N���X�̏���`��ɗ��p���邽�߃C���N���[�h

// �N���X�̑O���錾 (�z�Q�Ƃ�����邽��)
class Game;

/**
 * @class Graphics
 * @brief DirectX 11 ���g�p�����`�揈���𓝊�����N���X
 */
class Graphics {
public:
    /**
     * @brief �R���X�g���N�^
     */
    Graphics();

    /**
     * @brief �f�X�g���N�^
     */
    ~Graphics();

    /**
     * @brief DirectX 11 �̏������������s���܂��B
     * @param hWnd �`��Ώۂ̃E�B���h�E�n���h��
     * @return HRESULT �����̐���
     */
    HRESULT Initialize(HWND hWnd);

    /**
     * @brief DirectX 11 �̃��\�[�X��������܂��B
     */
    void Shutdown();

    /**
     * @brief 1�t���[�����̕`�揈�����s���܂��B
     * @param game �Q�[���̏�Ԃ��i�[���ꂽGame�I�u�W�F�N�g
     */
    void RenderFrame(const Game& game);

private:
    // --- DirectX 11 �̃R�A�C���^�[�t�F�[�X ---
    IDXGISwapChain* m_pSwapChain;           // �X���b�v�`�F�[��
    ID3D11Device* m_pd3dDevice;           // Direct3D�f�o�C�X
    ID3D11DeviceContext* m_pImmediateContext;    // �f�o�C�X�R���e�L�X�g
    ID3D11RenderTargetView* m_pRenderTargetView;    // �����_�[�^�[�Q�b�g�r���[

    // --- �V�F�[�_�[�I�u�W�F�N�g ---
    ID3D11VertexShader* m_pVertexShader;        // ���_�V�F�[�_�[
    ID3D11PixelShader* m_pPixelShader;         // �s�N�Z���V�F�[�_�[
    ID3D11InputLayout* m_pVertexLayout;        // ���̓��C�A�E�g

    // --- �o�b�t�@�I�u�W�F�N�g ---
    ID3D11Buffer* m_pVertexBuffer;        // �v���C���[�p�̒��_�o�b�t�@
    ID3D11Buffer* m_pConstantBuffer;      // �萔�o�b�t�@ (�I�u�W�F�N�g�̈ʒu�Ȃǂ��V�F�[�_�[�ɓn��)
    ID3D11Buffer* m_pBulletVertexBuffer;  // �e�p�̒��_�o�b�t�@
    ID3D11Buffer* m_pObstacleVertexBuffer;// ��Q���p�̒��_�o�b�t�@

private:
    // --- �`�揈���̃w���p�[�֐� ---
    void RenderPlayer(float x, float y);
    void RenderBullets(const Game& game);
    void RenderObstacles(const Game& game);
};