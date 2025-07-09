#pragma once
#include <windows.h>
#include <d3d11.h>

class Graphics {
public:
    Graphics();
    ~Graphics();

    // �������ƃV���b�g�_�E��
    HRESULT Initialize(HWND hWnd);
    void Shutdown();

    // �t���[���̃����_�����O
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
};