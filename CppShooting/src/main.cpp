#include <windows.h>
#include "Graphics.h"
#include "Game.h"

// �O���[�o���ϐ�
Graphics* g_pGraphics = nullptr;
Game* g_pGame = nullptr;

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
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                      L"SimpleGame", nullptr };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(L"SimpleGame", L"DirectX 11 Simple Game (Refactored)",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        nullptr, nullptr, wc.hInstance, nullptr);

    // Graphics��Game�N���X�̃C���X�^���X���쐬
    g_pGraphics = new Graphics();
    g_pGame = new Game();

    if (!g_pGraphics || !g_pGame) {
        return -1;
    }

    if (SUCCEEDED(g_pGraphics->Initialize(hWnd))) {
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                if (GetAsyncKeyState('P') & 0x8000) {
                    break;
                }

                // ��Ԃ̍X�V�ƕ`��
                g_pGame->Update();                  // �Q�[���̏�Ԃ��X�V
                g_pGraphics->RenderFrame(*g_pGame); // �Q�[���̏�Ԃ����ɕ`��
            }
        }
    }

    // �N���[���A�b�v
    if (g_pGraphics) {
        g_pGraphics->Shutdown();
        delete g_pGraphics;
        g_pGraphics = nullptr;
    }
    if (g_pGame) {
        delete g_pGame;
        g_pGame = nullptr;
    }


    UnregisterClass(L"SimpleGame", wc.hInstance);
    return 0;
}