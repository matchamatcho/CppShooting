#include <windows.h>
#include "Graphics.h"
#include "Game.h"

// --- グローバル変数 ---
// プログラム全体からアクセスできるように、GraphicsとGameのインスタンスをグローバルに保持
Graphics* g_pGraphics = nullptr;
Game* g_pGame = nullptr;

/**
 * @brief ウィンドウプロシージャ
 * ウィンドウメッセージを処理します。
 * @param hWnd メッセージの対象となるウィンドウのハンドル
 * @param message メッセージID
 * @param wParam 追加のメッセージ情報
 * @param lParam 追加のメッセージ情報
 * @return LRESULT メッセージ処理の結果
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY: // ウィンドウが破棄されるメッセージ
        PostQuitMessage(0); // メインループを終了させるためにWM_QUITメッセージをポスト
        break;
    default:
        // 上記以外のメッセージはデフォルトのプロシージャに任せる
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/**
 * @brief アプリケーションのエントリーポイント
 * @param hInstance アプリケーションのインスタンスハンドル
 * @param hPrevInstance 常に0
 * @param lpCmdLine コマンドライン引数
 * @param nCmdShow ウィンドウの表示状態
 * @return int 終了コード
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    // --- ウィンドウクラスの登録 ---
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                      L"SimpleGame", nullptr };
    RegisterClassEx(&wc);

    // --- ウィンドウの作成 ---
    HWND hWnd = CreateWindow(L"SimpleGame", L"DirectX 11 Simple Game (Refactored)",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        nullptr, nullptr, wc.hInstance, nullptr);

    // --- GraphicsとGameクラスのインスタンスを作成 ---
    g_pGraphics = new Graphics();
    g_pGame = new Game();

    // インスタンス作成失敗時は終了
    if (!g_pGraphics || !g_pGame) {
        return -1;
    }

    // --- DirectXの初期化 ---
    if (SUCCEEDED(g_pGraphics->Initialize(hWnd))) {
        // ウィンドウを表示
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // --- メインループ ---
        MSG msg = { 0 };
        while (WM_QUIT != msg.message) {
            // ウィンドウメッセージを処理
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // 'P'キーが押されたらループを抜けて終了
                if (GetAsyncKeyState('P') & 0x8000) {
                    break;
                }

                // ゲームの状態更新と描画
                g_pGame->Update();                  // ゲームのロジックを更新
                g_pGraphics->RenderFrame(*g_pGame); // 更新されたゲームの状態を元に画面を描画
            }
        }
    }

    // --- クリーンアップ処理 ---
    if (g_pGraphics) {
        g_pGraphics->Shutdown();
        delete g_pGraphics;
        g_pGraphics = nullptr;
    }
    if (g_pGame) {
        delete g_pGame;
        g_pGame = nullptr;
    }

    // ウィンドウクラスの登録を解除
    UnregisterClass(L"SimpleGame", wc.hInstance);
    return 0;
}