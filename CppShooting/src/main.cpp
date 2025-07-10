#include <windows.h>
#include "Graphics.h"
#include "Game.h"

// --- �O���[�o���ϐ� ---
// �v���O�����S�̂���A�N�Z�X�ł���悤�ɁAGraphics��Game�̃C���X�^���X���O���[�o���ɕێ�
Graphics* g_pGraphics = nullptr;
Game* g_pGame = nullptr;

/**
 * @brief �E�B���h�E�v���V�[�W��
 * �E�B���h�E���b�Z�[�W���������܂��B
 * @param hWnd ���b�Z�[�W�̑ΏۂƂȂ�E�B���h�E�̃n���h��
 * @param message ���b�Z�[�WID
 * @param wParam �ǉ��̃��b�Z�[�W���
 * @param lParam �ǉ��̃��b�Z�[�W���
 * @return LRESULT ���b�Z�[�W�����̌���
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY: // �E�B���h�E���j������郁�b�Z�[�W
        PostQuitMessage(0); // ���C�����[�v���I�������邽�߂�WM_QUIT���b�Z�[�W���|�X�g
        break;
    default:
        // ��L�ȊO�̃��b�Z�[�W�̓f�t�H���g�̃v���V�[�W���ɔC����
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/**
 * @brief �A�v���P�[�V�����̃G���g���[�|�C���g
 * @param hInstance �A�v���P�[�V�����̃C���X�^���X�n���h��
 * @param hPrevInstance ���0
 * @param lpCmdLine �R�}���h���C������
 * @param nCmdShow �E�B���h�E�̕\�����
 * @return int �I���R�[�h
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    // --- �E�B���h�E�N���X�̓o�^ ---
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                      L"SimpleGame", nullptr };
    RegisterClassEx(&wc);

    // --- �E�B���h�E�̍쐬 ---
    HWND hWnd = CreateWindow(L"SimpleGame", L"DirectX 11 Simple Game (Refactored)",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        nullptr, nullptr, wc.hInstance, nullptr);

    // --- Graphics��Game�N���X�̃C���X�^���X���쐬 ---
    g_pGraphics = new Graphics();
    g_pGame = new Game();

    // �C���X�^���X�쐬���s���͏I��
    if (!g_pGraphics || !g_pGame) {
        return -1;
    }

    // --- DirectX�̏����� ---
    if (SUCCEEDED(g_pGraphics->Initialize(hWnd))) {
        // �E�B���h�E��\��
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // --- ���C�����[�v ---
        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            // �E�B���h�E���b�Z�[�W������
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // 'P'�L�[�������ꂽ�烋�[�v�𔲂��ďI��
                if (GetAsyncKeyState('P') & 0x8000) {
                    break;
                }

                // �Q�[���̏�ԍX�V�ƕ`��
                g_pGame->Update();                  // �Q�[���̃��W�b�N���X�V
                g_pGraphics->RenderFrame(*g_pGame); // �X�V���ꂽ�Q�[���̏�Ԃ����ɉ�ʂ�`��
            }
        }
    }

    // --- �N���[���A�b�v���� ---
    if (g_pGraphics) {
        g_pGraphics->Shutdown();
        delete g_pGraphics;
        g_pGraphics = nullptr;
    }
    if (g_pGame) {
        delete g_pGame;
        g_pGame = nullptr;
    }

    // �E�B���h�E�N���X�̓o�^������
    UnregisterClass(L"SimpleGame", wc.hInstance);
    return 0;
}