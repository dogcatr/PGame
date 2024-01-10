//-----------------------------------------------------------------------------
// File : App.h
// Desc : Application Module.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Mesh.h"

#include "GameUpdate.h"
#include <cmath>
#include <iostream>
#include <random>

#include <DirectXCollision.h>
#include <DirectXMath.h>

constexpr float PI = 3.1415;
constexpr float NP = 2.7182;

//-----------------------------------------------------------------------------
// Linker
//-----------------------------------------------------------------------------
#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

//-----------------------------------------------------------------------------
// Type Alias
//-----------------------------------------------------------------------------
template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct alignas(256) Transform
{
    DirectX::XMMATRIX   World;      // ワールド行列です.
    DirectX::XMMATRIX   View;       // ビュー行列です.
    DirectX::XMMATRIX   Proj;       // 射影行列です.
    //chg1
    /*DirectX::XMFLOAT3 LightPosition;
    DirectX::XMFLOAT3 LightColor;*/
    DirectX::XMMATRIX Light;
};

template<typename T>
struct ConstantBufferView
{
    D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;               // 定数バッファの構成設定です.
    D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;          // CPUディスクリプタハンドルです.
    D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;          // GPUディスクリプタハンドルです.
    T*                              pBuffer;            // バッファ先頭へのポインタです.
};

struct Texture
{
    ComPtr<ID3D12Resource>          pResource;      // リソースです.
    D3D12_CPU_DESCRIPTOR_HANDLE     HandleCPU;      // CPUディスクリプタハンドルです.
    D3D12_GPU_DESCRIPTOR_HANDLE     HandleGPU;      // GPUディスクリプタハンドルです.
};


class App
{
public:
    App(uint32_t width, uint32_t height);
    ~App();
    void Run();

    float m_LightRA;
    float pos_x = 0.0f;//eye position
    float pos_y = 9.0f;
    float pos_z = 6.0f;
    DirectX::XMVECTOR eyePos = { pos_x, pos_y, pos_z, 0.0f };
    DirectX::XMVECTOR targetPos = DirectX::XMVectorZero();
    void setEP();

    class CharaClass
    {
    public:
        CharaClass(uint32_t width, uint32_t height);
        ~CharaClass();

        bool init(ComPtr<ID3D12Device>& m_pDevice, ComPtr<ID3D12DescriptorHeap>& m_pHeapCBV, wchar_t* filename);
        bool initbb(ComPtr<ID3D12Device>& m_pDevice, ComPtr<ID3D12DescriptorHeap>& m_pHeapCBV, wchar_t* filename);
        void update(ComPtr<ID3D12GraphicsCommandList>& m_pCmdList, uint32_t m_FrameIndex);
        void updatebb(ComPtr<ID3D12GraphicsCommandList>& m_pCmdList, uint32_t m_FrameIndex);//bounding box

        //DirectX::XMMATRIX cm_World = { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} };
        DirectX::XMMATRIX cm_World;//キャラの位置
        /*void chWld(DirectX::XMMATRIX world, uint32_t m_FrameIndex);*/

        float cm_LightRA;
        DirectX::XMMATRIX LightM = { {0.0f,100.0f,-100.0f,0.0f},{0.9f,0.9f,0.9f,0.0f},{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f} };
        float C_Angle;
        float pos_x;//world pos
        float pos_y;
        float pos_z;
        DirectX::XMVECTOR eyePos;
        DirectX::XMVECTOR targetPos = DirectX::XMVectorZero();

        //std::vector<DirectX::XMFLOAT4> bflt = { { 1.0f, 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f, 0.0f } };//単位ベクトル

        State state;
        float bltedTime;

        void setPos();

        void setWorldBB();

        float Getbox(int i, int j);

        void setEyePos(DirectX::XMVECTOR& eyepos);

    private:
        static const uint32_t cFrameCount = 2;
        uint32_t cm_Width;
        uint32_t cm_Height;

        std::vector<Mesh> cm_Meshes;
        std::vector<Material> cm_Materials;

        ComPtr<ID3D12Resource> cm_pVB;
        ComPtr<ID3D12Resource> cm_pIB;
        ComPtr<ID3D12Resource> cm_pCB[cFrameCount];


        D3D12_VERTEX_BUFFER_VIEW cm_VBV;
        D3D12_INDEX_BUFFER_VIEW cm_IBV;
        ConstantBufferView<Transform> cm_CBV[cFrameCount];

        //DirectX::XMMATRIX cm_World = { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} };

        //Bounding Box
        std::vector<Mesh> bb_Meshes;
        std::vector<Material> bb_Materials;

        ComPtr<ID3D12Resource> bb_pVB;
        ComPtr<ID3D12Resource> bb_pIB;
        ComPtr<ID3D12Resource> bb_pCB[cFrameCount];


        D3D12_VERTEX_BUFFER_VIEW bb_VBV;
        D3D12_INDEX_BUFFER_VIEW bb_IBV;
        ConstantBufferView<Transform> bb_CBV[cFrameCount];

        DirectX::XMMATRIX bbox;//Bounding box
        DirectX::XMMATRIX wbbox;//Bounding box, world
        //DirectX::XMMATRIX bbox = { {0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f} };

        //DirectX::BoundingBox box;
        //XM_CALLCONV.CreateFromPoint();
    };

private:

    CharaClass m_CharaA;
    CharaClass m_CharaB;
    CharaClass m_CharaC;
    CharaClass m_Target;

    HINSTANCE   m_hInst;        // インスタンスハンドルです.
    HWND        m_hWnd;         // ウィンドウハンドルです.
    uint32_t    m_Width;        // ウィンドウの横幅です.
    uint32_t    m_Height;       // ウィンドウの縦幅です.

    static const uint32_t FrameCount = 2;
    ComPtr<ID3D12Device>                   m_pDevice;                      // デバイスです.
    ComPtr<ID3D12CommandQueue>             m_pQueue;                       // コマンドキューです.
    ComPtr<IDXGISwapChain3>                m_pSwapChain;                   // スワップチェインです.
    ComPtr<ID3D12Resource>                 m_pColorBuffer[FrameCount];     // カラーバッファです.
    ComPtr<ID3D12Resource>                 m_pDepthBuffer;                 // 深度ステンシルバッファです.
    ComPtr<ID3D12CommandAllocator>         m_pCmdAllocator[FrameCount];    // コマンドアロケータです.
    ComPtr<ID3D12GraphicsCommandList>      m_pCmdList;                     // コマンドリストです.
    ComPtr<ID3D12DescriptorHeap>           m_pHeapRTV;                     // ディスクリプタヒープです(レンダーターゲットビュー).
    ComPtr<ID3D12Fence>                    m_pFence;                       // フェンスです.
    ComPtr<ID3D12DescriptorHeap>           m_pHeapDSV;                     // ディスクリプタヒープです(深度ステンシルビュー)
    ComPtr<ID3D12DescriptorHeap>           m_pHeapCBV;                     // ディスクリプタヒープです(定数バッファビュー・シェーダリソースビュー・アンオーダードアクセスビュー)
    ComPtr<ID3D12Resource>                 m_pVB;                          // 頂点バッファです.
    ComPtr<ID3D12Resource>                 m_pIB;                          // インデックスバッファです.
    ComPtr<ID3D12Resource>                 m_pCB[FrameCount * 2];          // 定数バッファです.
    ComPtr<ID3D12RootSignature>            m_pRootSignature;               // ルートシグニチャです.
    ComPtr<ID3D12PipelineState>            m_pPSO;                         // パイプラインステートです.

    HANDLE                          m_FenceEvent;                   // フェンスイベントです.
    uint64_t                        m_FenceCounter[FrameCount];     // フェンスカウンターです.
    uint32_t                        m_FrameIndex;                   // フレーム番号です.
    D3D12_CPU_DESCRIPTOR_HANDLE     m_HandleRTV[FrameCount];        // CPUディスクリプタ(レンダーターゲットビュー)です.
    D3D12_CPU_DESCRIPTOR_HANDLE     m_HandleDSV;                    // CPUディスクリプタ(深度ステンシルビュー)です.
    D3D12_VERTEX_BUFFER_VIEW        m_VBV;                          // 頂点バッファビューです.
    D3D12_INDEX_BUFFER_VIEW         m_IBV;                          // インデックスバッファビューです.
    D3D12_VIEWPORT                  m_Viewport;                     // ビューポートです.
    D3D12_RECT                      m_Scissor;                      // シザー矩形です.
    ConstantBufferView<Transform>   m_CBV[FrameCount * 2];          // 定数バッファビューです.
    float                           m_RotateAngle;                  // 回転角です.
    Texture                         m_Texture;                      // テクスチャです.
    std::vector<Mesh>               m_Meshes;                       // ★追加：メッシュです.
    std::vector<Material>           m_Materials;                    // ★追加：マテリアルです.
    
    //=========================================================================
    // private methods.
    //=========================================================================
    bool InitApp();
    void TermApp();
    bool InitWnd();
    void TermWnd();
    void MainLoop();
    bool InitD3D();
    void TermD3D();
    void Render();
    void WaitGpu();
    void Present(uint32_t interval);
    bool OnInit();
    void OnTerm();


    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

    //mouse
    void mousemove();
    long centerX;
    long centerY;
    float diffX;
    float diffY;
    float diffD;

    float dftspeed = 0.03f;//キャラBとカメラの速さ
    float speed=dftspeed;

    float edftspeed = 0.00f;
    float espeed = edftspeed;

    MouseState state;
    MSG m_msg = {};

    std::random_device rd;

    //collision
    bool Colli();
    //blighted
    void knockBack();
    void knockBackA();
    void updMOVE();

    void updmatome();
};