#pragma once
#include <DirectXMath.h>
#include "App.h"

//class CharaClass
//{
//public:
//	CharaClass(uint32_t width, uint32_t height);
//	~CharaClass();
//
//    bool init(ComPtr<ID3D12Device>& m_pDevice, ComPtr<ID3D12DescriptorHeap>& m_pHeapCBV , wchar_t* filename);
//    void update(ComPtr<ID3D12GraphicsCommandList>& m_pCmdList, uint32_t m_FrameIndex);
//
//private:
//    static const uint32_t FrameCount = 2;
//    uint32_t m_Width;
//    uint32_t m_Height;
//
//    std::vector<Mesh> m_Meshes;
//    std::vector<Material> m_Materials;
//
//    ComPtr<ID3D12Resource> m_pVB;
//    ComPtr<ID3D12Resource> m_pIB;
//    ComPtr<ID3D12Resource> m_pCB[FrameCount * 2];
//
//
//    D3D12_VERTEX_BUFFER_VIEW m_VBV;
//    D3D12_INDEX_BUFFER_VIEW m_IBV;
//    ConstantBufferView<Transform> m_CBV[FrameCount * 2];
//};

class Flag {
private:
	char mFlag;
public:
	void set(char flag) { mFlag |= flag; }
	void reset() { mFlag = 0; }
	void off(char flag) { mFlag &= ~flag; }
	bool is(char flag) { return (mFlag & flag)!=0; }
};
class State {
private:
	Flag mFlag;
public:
	State() :mFlag() {};
	struct FLAGS {
		enum {
			WALK = 1 << 0,//00000001
			ATTACK=1<<1,//00000010
			CLDED=1<<2,//00000100
			BLIGHTED=1<<3,//00001000

		};
	};
	void set(char flag) { mFlag.set(flag); }
	void reset() { mFlag.reset(); }
	void off(char flag) { mFlag.off(flag); }
	bool is(char flag) { return mFlag.is(flag); }
};

class MouseState {
private:
	Flag mFlag;
public:
	MouseState() :mFlag() {};
	struct FLAGS {
		enum {
			LCLICK=1<<0,
			RCLICK=1<<1,
			HOLD=1<<2,
			NOTCLICK=1<<3,
		};
	};
	void set(char flag) { mFlag.set(flag); }
	void reset() { mFlag.reset(); }
	void off(char flag) { mFlag.off(flag); }
	bool is(char flag) { return mFlag.is(flag); }
};

float sigm(float x);