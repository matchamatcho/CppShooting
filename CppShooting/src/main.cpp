#include <windows.h>
#include "Graphics.h"

// �O���[�o���ϐ�
Graphics* g_pGraphics = nullptr;

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���C���֐�
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    // �E�B���h�E�N���X�̓o�^
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                      L"SimpleGame", nullptr };
    RegisterClassEx(&wc);

    // �E�B���h�E�̍쐬
    HWND hWnd = CreateWindow(L"SimpleGame", L"DirectX 11 Simple Game (Split Files)",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        nullptr, nullptr, wc.hInstance, nullptr);

    // Graphics�N���X�̃C���X�^���X���쐬
    g_pGraphics = new Graphics();
    if (!g_pGraphics) {
        return -1;
    }

    // DirectX�̏�����
    if (SUCCEEDED(g_pGraphics->Initialize(hWnd))) {
        // �E�B���h�E�̕\��
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // ���C�����[�v
        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // �`�揈��
                g_pGraphics->RenderFrame();
            }
        }
    }

    // �N���[���A�b�v
    if (g_pGraphics) {
        g_pGraphics->Shutdown();
        delete g_pGraphics;
        g_pGraphics = nullptr;
    }

    UnregisterClass(L"SimpleGame", wc.hInstance);
    return 0;
}