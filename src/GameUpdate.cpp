#include "App.h"
#include "GameUpdate.h"
#include "iostream"
#include "FileUtil.h"

//void App::update(){
//    RECT rec;
//    POINT MousePoint;
//    GetWindowRect(m_hWnd, &rec);
//    if (GetCursorPos(&MousePoint)) {
//        if (MousePoint.x < (rec.left + rec.right) / 2) {
//            f_a = -0.025f;
//        }
//        else {
//            f_a = 0.025f;
//        }
//    }
//
//    m_RotateAngle += f_a;
//    //m_CBV[m_FrameIndex].pBuffer->World = DirectX::XMMatrixRotationY(m_RotateAngle);
//    DirectX::XMMATRIX heko = { {1.0f,0.0f, 0.0f, 0.0f}, {0.0f,1.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f,0.0f}, {0.0f + 0.5f * sin(m_LightRA),0.0f + 0.5f * cos(m_LightRA),0.0f,1.0f} };
//    //m_CBV[m_FrameIndex].pBuffer->World = DirectX::XMMatrixRotationY(m_RotateAngle) * heko;
//    m_CBV[m_FrameIndex].pBuffer->World = DirectX::XMMatrixRotationY(m_RotateAngle);
//    //chg1
//    //m_CBV[m_FrameIndex].pBuffer->LightPosition = DirectX::XMFLOAT3(5.0f * sin(m_LightRA), 10.0f + 5.0f * cos(m_LightRA) , 10.0f);
//
//    /*auto eyePos = DirectX::XMVectorSet(0.0f, 1.0f , 2.0f, 0.0f);
//    auto targetPos = DirectX::XMVectorSet(0.0f , 0.0f , 0.0f, 0.0f);
//    auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//    m_CBV[m_FrameIndex].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);*/
//    //chg1
//}

void chmin(float& a, const float& b) {
    if (a > b) {
        a = b;
    }
}

void chmax(float& a, const float& b) {
    if (a < b) {
        a = b;
    }
}

float sigm(float x) {
    return 0.1f / (1.0f + pow(NP, x));
}

//Charcter Class
//.objのパスの設定から、Vertex Buffer, ... , Command Listまでupdate関数で行う
//Appの参照？を渡す

App::CharaClass::CharaClass(uint32_t width, uint32_t height):cm_Width(width),cm_Height(height)
{

}
App::CharaClass::~CharaClass() {

}
bool App::CharaClass::init(ComPtr<ID3D12Device>& m_pDevice , ComPtr<ID3D12DescriptorHeap>& m_pHeapCBV, wchar_t* filename) {
    {
        std::wstring path;
        if (!SearchFilePath(filename, path))
        {
            return false;
        }
        if (!LoadMesh(path.c_str(), cm_Meshes, cm_Materials))
        {
            return false;
        }
        assert(cm_Meshes.size() == 1);
    }

    //vertex
    {
        //vertex
        auto size = sizeof(MeshVertex) * cm_Meshes[0].Vertices.size();
        auto vertices = cm_Meshes[0].Vertices.data();

        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(cm_pVB.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        void* ptr = nullptr;
        hr = cm_pVB->Map(0, nullptr, &ptr);
        if (FAILED(hr))
        {
            return false;
        }
        memcpy(ptr, vertices, size);
        cm_pVB->Unmap(0, nullptr);

        cm_VBV.BufferLocation = cm_pVB->GetGPUVirtualAddress();
        cm_VBV.SizeInBytes = static_cast<UINT>(size);
        cm_VBV.StrideInBytes = static_cast<UINT>(sizeof(MeshVertex));
    }

    //index
    {
        //index
        auto size = sizeof(uint32_t) * cm_Meshes[0].Indices.size();
        auto indices = cm_Meshes[0].Indices.data();

        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(cm_pIB.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        void* ptr = nullptr;
        hr = cm_pIB->Map(0, nullptr, &ptr);
        if (FAILED(hr))
        {
            return false;
        }

        memcpy(ptr, indices, size);
        cm_pIB->Unmap(0, nullptr);

        cm_IBV.BufferLocation = cm_pIB->GetGPUVirtualAddress();
        cm_IBV.Format = DXGI_FORMAT_R32_UINT;
        cm_IBV.SizeInBytes = static_cast<UINT>(size);
    }

    //constant
    {
        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        // リソースの設定.
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(Transform);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0; i < FrameCount; ++i)
        {
            // リソース生成.
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(cm_pCB[i].GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            auto address = cm_pCB[i]->GetGPUVirtualAddress();
            auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
            auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

            handleCPU.ptr += incrementSize * i;
            handleGPU.ptr += incrementSize * i;

            // 定数バッファビューの設定.
            cm_CBV[i].HandleCPU = handleCPU;
            cm_CBV[i].HandleGPU = handleGPU;
            cm_CBV[i].Desc.BufferLocation = address;
            cm_CBV[i].Desc.SizeInBytes = sizeof(Transform);

            // 定数バッファビューを生成.
            m_pDevice->CreateConstantBufferView(&cm_CBV[i].Desc, handleCPU);

            // マッピング.
            hr = cm_pCB[i]->Map(0, nullptr, reinterpret_cast<void**>(&cm_CBV[i].pBuffer));
            if (FAILED(hr))
            {
                return false;
            }

            auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            auto fovY = DirectX::XMConvertToRadians(37.5f);
            auto aspect = static_cast<float>(cm_Width) / static_cast<float>(cm_Height);

            // 変換行列の設定.
            //cm_CBV[i].pBuffer->World = DirectX::XMMatrixIdentity();
            cm_CBV[i].pBuffer->World = cm_World;
            cm_CBV[i].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
            cm_CBV[i].pBuffer->Proj = DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, 1.0f, 1000.0f);
            //cm_CBV[i].pBuffer->Light = { DirectX::XMFLOAT3(0.0f, 100.0f, -100.0f), DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f), DirectX::XMFLOAT3(0.0f,0.0f,0.0f) };//Appのメンバにする
            cm_CBV[i].pBuffer->Light = LightM;
        }
    }
}

bool App::CharaClass::initbb(ComPtr<ID3D12Device>& m_pDevice, ComPtr<ID3D12DescriptorHeap>& m_pHeapCBV, wchar_t* filename) {
    {
        std::wstring path;
        if (!SearchFilePath(filename, path))
        {
            return false;
        }
        if (!LoadMesh(path.c_str(), bb_Meshes, bb_Materials))
        {
            return false;
        }
        std::vector<std::vector<float>> bev = { {INFINITY,INFINITY,INFINITY,0.0f}, {-INFINITY,-INFINITY,-INFINITY,0.0f} };//minus plus
        assert(bb_Meshes.size() == 1);
        for (const auto &t : bb_Meshes[0].Vertices) {
            chmin(bev[0][0], t.Position.x);
            chmin(bev[0][1], t.Position.y);
            chmin(bev[0][2], t.Position.z);
            chmax(bev[1][0], t.Position.x);
            chmax(bev[1][1], t.Position.y);
            chmax(bev[1][2], t.Position.z);
        }
       /* for (int i = 0; i < 6; i++) {
            std::cout << bev[i / 3][i % 3] << " " << std::endl;
        }*/
        bbox = { {bev[0][0],bev[0][1],bev[0][2],1.0f},{bev[1][0],bev[1][1],bev[1][2],1.0f},{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f} };
    }
    //vertex
    {
        //vertex
        auto size = sizeof(MeshVertex) * bb_Meshes[0].Vertices.size();
        auto vertices = bb_Meshes[0].Vertices.data();

        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(bb_pVB.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        void* ptr = nullptr;
        hr = bb_pVB->Map(0, nullptr, &ptr);
        if (FAILED(hr))
        {
            return false;
        }
        memcpy(ptr, vertices, size);
        bb_pVB->Unmap(0, nullptr);

        bb_VBV.BufferLocation = bb_pVB->GetGPUVirtualAddress();
        bb_VBV.SizeInBytes = static_cast<UINT>(size);
        bb_VBV.StrideInBytes = static_cast<UINT>(sizeof(MeshVertex));
    }

    {
        //index
        auto size = sizeof(uint32_t) * bb_Meshes[0].Indices.size();
        auto indices = bb_Meshes[0].Indices.data();

        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(bb_pIB.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        void* ptr = nullptr;
        hr = bb_pIB->Map(0, nullptr, &ptr);
        if (FAILED(hr))
        {
            return false;
        }

        memcpy(ptr, indices, size);
        bb_pIB->Unmap(0, nullptr);

        bb_IBV.BufferLocation = bb_pIB->GetGPUVirtualAddress();
        bb_IBV.Format = DXGI_FORMAT_R32_UINT;
        bb_IBV.SizeInBytes = static_cast<UINT>(size);
    }

    {
        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        // リソースの設定.
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(Transform);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0; i < FrameCount; ++i)
        {
            // リソース生成.
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(bb_pCB[i].GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            auto address = bb_pCB[i]->GetGPUVirtualAddress();
            auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
            auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

            handleCPU.ptr += incrementSize * i;
            handleGPU.ptr += incrementSize * i;

            // 定数バッファビューの設定.
            bb_CBV[i].HandleCPU = handleCPU;
            bb_CBV[i].HandleGPU = handleGPU;
            bb_CBV[i].Desc.BufferLocation = address;
            bb_CBV[i].Desc.SizeInBytes = sizeof(Transform);

            // 定数バッファビューを生成.
            m_pDevice->CreateConstantBufferView(&bb_CBV[i].Desc, handleCPU);

            // マッピング.
            hr = bb_pCB[i]->Map(0, nullptr, reinterpret_cast<void**>(&bb_CBV[i].pBuffer));
            if (FAILED(hr))
            {
                return false;
            }

            auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            auto fovY = DirectX::XMConvertToRadians(37.5f);
            auto aspect = static_cast<float>(cm_Width) / static_cast<float>(cm_Height);

            // 変換行列の設定.
            //cm_CBV[i].pBuffer->World = DirectX::XMMatrixIdentity();
            bb_CBV[i].pBuffer->World = cm_World;
            bb_CBV[i].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
            bb_CBV[i].pBuffer->Proj = DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, 1.0f, 1000.0f);
            cm_CBV[i].pBuffer->Light = LightM;

            //bb_CBV[i].pBuffer->LightPosition = DirectX::XMFLOAT3(0.0f, 100.0f, -100.0f);//Appのメンバにする
            //bb_CBV[i].pBuffer->LightColor = DirectX::XMFLOAT3(0.9f, 0.9f, 0.9f);
        }
    }
}

void App::CharaClass::update(ComPtr<ID3D12GraphicsCommandList>& m_pCmdList , uint32_t m_FrameIndex) {
    cm_CBV[m_FrameIndex].pBuffer->World = DirectX::XMMatrixRotationY(C_Angle) * cm_World;
    cm_CBV[m_FrameIndex].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    cm_CBV[m_FrameIndex].pBuffer->Light = LightM;

    //cm_CBV[m_FrameIndex].pBuffer->LightPosition = DirectX::XMFLOAT3(0.0f + 1.5f * sin(cm_LightRA) , 10.0f, -10.0f);//App mem
    m_pCmdList->SetGraphicsRootConstantBufferView(0, cm_CBV[m_FrameIndex].Desc.BufferLocation);

    m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    m_pCmdList->IASetVertexBuffers(0, 1, &cm_VBV);
    m_pCmdList->IASetIndexBuffer(&cm_IBV);

    auto count = static_cast<uint32_t>(cm_Meshes[0].Indices.size());
    m_pCmdList->DrawIndexedInstanced(count, 1, 0, 0, 0);
}

void App::CharaClass::updatebb(ComPtr<ID3D12GraphicsCommandList>& m_pCmdList, uint32_t m_FrameIndex) {
    bb_CBV[m_FrameIndex].pBuffer->World = cm_World;
    bb_CBV[m_FrameIndex].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    bb_CBV[m_FrameIndex].pBuffer->Light = LightM;

    //bb_CBV[m_FrameIndex].pBuffer->LightPosition = DirectX::XMFLOAT3(0.0f + 1.5f * sin(cm_LightRA), 10.0f, -10.0f);//App mem
    m_pCmdList->SetGraphicsRootConstantBufferView(0, bb_CBV[m_FrameIndex].Desc.BufferLocation);

    //m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    m_pCmdList->IASetVertexBuffers(0, 1, &bb_VBV);
    m_pCmdList->IASetIndexBuffer(&bb_IBV);

    auto count = static_cast<uint32_t>(bb_Meshes[0].Indices.size());
    m_pCmdList->DrawIndexedInstanced(count, 1, 0, 0, 0);
}

//void App::CharaClass::chWld(DirectX::XMMATRIX world, uint32_t m_FrameIndex) {
//    cm_CBV[m_FrameIndex].pBuffer->World = world;
//    bb_CBV[m_FrameIndex].pBuffer->World = world;
//}

void App::CharaClass::setPos() {
    cm_World = { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {pos_x, pos_y, pos_z, 1.0f} };
}

void App::CharaClass::setWorldBB() {
    //wbbox = cm_World * bbox;
    wbbox = bbox * cm_World;
}

float App::CharaClass::Getbox(int i, int j) {
    if (j == 0) {
        return DirectX::XMVectorGetX(wbbox.r[i]);
    }
    else if (j == 1) {
        return DirectX::XMVectorGetY(wbbox.r[i]);
    }
    else{
        return DirectX::XMVectorGetZ(wbbox.r[i]);
    }
 /*   else{
        return DirectX::XMVectorGetX(wbbox.r[i]);
    }*/
}

void App::CharaClass::setEyePos(DirectX::XMVECTOR& eyepos) {
    eyePos = eyepos;
}


void App::setEP() {
    eyePos = { pos_x, pos_y, pos_z, 0.0f };
    m_CharaA.setEyePos(eyePos);
    m_CharaB.setEyePos(eyePos);
    m_CharaC.setEyePos(eyePos);
    m_Target.setEyePos(eyePos);
}

//App Method
//マウスでBとカメラを移動, xyz



bool App::Colli() {
    //collision
    m_CharaA.setWorldBB();
    m_CharaB.setWorldBB();
    if (m_CharaA.Getbox(0, 0) <= m_CharaB.Getbox(1, 0) && m_CharaB.Getbox(0, 0) <= m_CharaA.Getbox(1, 0)) {
        if (m_CharaA.Getbox(0, 1) <= m_CharaB.Getbox(1, 1) && m_CharaB.Getbox(0, 1) <= m_CharaA.Getbox(1, 1)) {
            if (m_CharaA.Getbox(0, 2) <= m_CharaB.Getbox(1, 2) && m_CharaB.Getbox(0, 2) <= m_CharaA.Getbox(1, 2)) {
                return true;
            }
        }
    }
    return false;
}

void App::knockBack() {
    diffX = m_CharaA.pos_x - m_CharaB.pos_x;
    diffY = -(m_CharaA.pos_z - m_CharaB.pos_z);
    float angle = atan(diffY / diffX);
    if (diffX < 0) angle += PI;
    diffX = 3.0f * sigm(m_CharaB.bltedTime) * cos(angle);
    diffY = 3.0f * sigm(m_CharaB.bltedTime) * sin(angle);
    m_CharaB.pos_x -= diffX;
    pos_x -= diffX;
    m_CharaB.pos_z += diffY;
    pos_z += diffY;
}

void App::knockBackA() {
    diffX = m_CharaB.pos_x - m_CharaA.pos_x;
    diffY = -(m_CharaB.pos_z - m_CharaA.pos_z);
    float angle = atan(diffY / diffX);
    if (diffX < 0) angle += PI;
    diffX = 3.0f * sigm(m_CharaA.bltedTime) * cos(angle);
    diffY = 3.0f * sigm(m_CharaA.bltedTime) * sin(angle);
    m_CharaA.pos_x -= diffX;
    m_CharaA.pos_z += diffY;
}

void App::updMOVE() {
    //world更新, matrix
    m_CharaA.setPos();
    m_CharaB.setPos();
    m_CharaC.setPos();
    m_Target.setPos();

    //カメラの視点更新, vector
    targetPos = { m_CharaB.pos_x, m_CharaB.pos_y, m_CharaB.pos_z, 0.0f };
    m_CharaA.targetPos = targetPos;
    m_CharaB.targetPos = targetPos;
    m_CharaC.targetPos = targetPos;
    m_Target.targetPos = targetPos;

    //カメラ移動, vector
    eyePos = { pos_x, pos_y, pos_z, 0.0f };
    m_CharaA.setEyePos(eyePos);
    m_CharaB.setEyePos(eyePos);
    m_CharaC.setEyePos(eyePos);
    m_Target.setEyePos(eyePos);

    //LightPos Color, CameraPos
    DirectX::XMMATRIX lightM = { {0.0f,3.0f,3.0f,0.0f},{0.7f, 0.7f, 0.7f, 0.0f},eyePos,{0.0f,0.0f,0.0f,0.0f} };
    m_CharaA.LightM = lightM;
    m_CharaB.LightM = lightM;
    m_CharaC.LightM = lightM;
    lightM = { {0.0f,3.0f,3.0f,0.0f},{1.0f, 0.2f, 0.2f, 0.0f},eyePos,{0.0f,0.0f,0.0f,0.0f} };
    m_Target.LightM = lightM;
}

void App::updmatome() {//キャラのアップデートまとめ
    //キャラ更新
    //マウスでBとカメラを移動, xyz
    mousemove();//App.cppにある

    //FLAGで行動を変える
    if (state.is(MouseState::FLAGS::LCLICK) && !m_CharaB.state.is(State::FLAGS::ATTACK)) {
        speed = 0.20f;
        m_CharaB.state.set(State::FLAGS::ATTACK);
    }
    /*else if (state.is(MouseState::FLAGS::RCLICK)) {
        speed = 0.00f;
    }*/

    if (m_CharaB.state.is(State::FLAGS::ATTACK)) {
        speed -= 0.001f;
        if (speed < dftspeed) {
            speed = dftspeed;
            m_CharaB.state.off(State::FLAGS::ATTACK);
        }
        
    }
    //ターゲットマークが自キャラに近づく
    if (!m_Target.state.is(State::FLAGS::ATTACK) && !m_CharaA.state.is(State::FLAGS::ATTACK) && rd()%200<2) {
        //m_CharaA.state.set(State::FLAGS::ATTACK);
        m_Target.state.set(State::FLAGS::ATTACK);
        espeed = 0.35f;
        /*m_CharaA.pos_x = m_CharaB.pos_x;
        m_CharaA.pos_y = m_CharaB.pos_y;
        m_CharaA.pos_z = m_CharaB.pos_z;*/
    }

    if (m_Target.state.is(State::FLAGS::ATTACK)) {
        m_Target.pos_x += (0.05f * (m_CharaB.pos_x - m_Target.pos_x));
        m_Target.pos_y += (0.05f * (m_CharaB.pos_y - m_Target.pos_y));
        m_Target.pos_z += (0.05f * (m_CharaB.pos_z - m_Target.pos_z));
        if (abs(m_CharaB.pos_x - m_Target.pos_x) + abs(m_CharaB.pos_z - m_Target.pos_z)<1.0f) {
            m_CharaA.state.set(State::FLAGS::ATTACK);
            m_Target.state.off(State::FLAGS::ATTACK);
        }
    }
    //ターゲットマークが十分近づいたら敵キャラが攻撃
    if (m_CharaA.state.is(State::FLAGS::ATTACK)) {
        espeed -= 0.001f;
        diffX = m_Target.pos_x - m_CharaA.pos_x;
        diffY = m_Target.pos_z - m_CharaA.pos_z;
        m_CharaA.C_Angle = atan(diffY / diffX);
        if (diffX < 0) {
            m_CharaA.C_Angle += PI;
        }
        diffX = espeed * cos(m_CharaA.C_Angle);
        diffY = espeed * sin(m_CharaA.C_Angle);
        m_CharaA.pos_x += diffX;
        m_CharaA.pos_z += diffY;
        if (espeed < edftspeed || (abs(m_CharaA.pos_x - m_Target.pos_x) + abs(m_CharaA.pos_z - m_Target.pos_z) < 0.5f)) {
            espeed = edftspeed;
            m_CharaA.state.off(State::FLAGS::ATTACK);
            //m_Target.state.off(State::FLAGS::ATTACK);
        }
    }

    //collision, xyz
    if (Colli()) {
        if (!m_CharaB.state.is(State::FLAGS::ATTACK) || m_CharaA.state.is(State::FLAGS::ATTACK)) {//攻撃状態ではないとき
            m_CharaB.state.set(State::FLAGS::BLIGHTED);
            m_CharaB.bltedTime = 0.0f;
        }
        if (!m_CharaA.state.is(State::FLAGS::ATTACK) || m_CharaB.state.is(State::FLAGS::ATTACK)) {//攻撃状態ではないとき
            m_CharaA.state.set(State::FLAGS::BLIGHTED);
            m_CharaA.bltedTime = 0.0f;
        }
    }
    /*if (m_CharaB.state.is(State::FLAGS::BLIGHTED)) {
        m_CharaA.bltedTime += 0.1f;
        m_CharaB.bltedTime += 0.1f;
        knockBackA();
        knockBack();
    }*/
    if (m_CharaA.state.is(State::FLAGS::BLIGHTED)) {//BLIGHTEDならノックバック
        m_CharaA.bltedTime += 0.1f;
        knockBackA();
    }
    if (m_CharaB.state.is(State::FLAGS::BLIGHTED)) {
        m_CharaB.bltedTime += 0.1f;
        knockBack();
    }

    if (sigm(m_CharaA.bltedTime) < 0.01f) {//BLIGHTEDになって時間がたったらBLIGHTEDではなくなる
        m_CharaA.state.off(State::FLAGS::BLIGHTED);
    }
    if (sigm(m_CharaB.bltedTime) < 0.01f) {
        m_CharaB.state.off(State::FLAGS::BLIGHTED);
    }

    //world等 更新, matrix
    updMOVE();

    //描画
    {

        m_CharaA.update(m_pCmdList, m_FrameIndex);
        m_CharaB.update(m_pCmdList, m_FrameIndex);
        m_CharaC.update(m_pCmdList, m_FrameIndex);//plane
        m_Target.update(m_pCmdList, m_FrameIndex);//target

        m_CharaA.updatebb(m_pCmdList, m_FrameIndex);
        m_CharaB.updatebb(m_pCmdList, m_FrameIndex);
        //m_CharaC.updatebb(m_pCmdList, m_FrameIndex);//plane
    }
    //GetMouseMovePointsEx
}