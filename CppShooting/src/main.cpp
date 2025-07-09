#include <windows.h>
#include "Graphics.h"

// グローバル変数
Graphics* g_pGraphics = nullptr;

// ウィンドウプロシージャ
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

// メイン関数
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    // ウィンドウクラスの登録
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                      L"SimpleGame", nullptr };
    RegisterClassEx(&wc);

    // ウィンドウの作成
    HWND hWnd = CreateWindow(L"SimpleGame", L"DirectX 11 Simple Game (Split Files)",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        nullptr, nullptr, wc.hInstance, nullptr);

    // Graphicsクラスのインスタンスを作成
    g_pGraphics = new Graphics();
    if (!g_pGraphics) {
        return -1;
    }

    // DirectXの初期化
    if (SUCCEEDED(g_pGraphics->Initialize(hWnd))) {
        // ウィンドウの表示
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // メインループ
        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // 描画処理
                g_pGraphics->RenderFrame();
            }
        }
    }

    // クリーンアップ
    if (g_pGraphics) {
        g_pGraphics->Shutdown();
        delete g_pGraphics;
        g_pGraphics = nullptr;
    }

    UnregisterClass(L"SimpleGame", wc.hInstance);
    return 0;
}