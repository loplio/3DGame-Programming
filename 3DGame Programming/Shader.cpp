#include "Shader.h"
#include "Method.h"
#include "stdafx.h"

CShader::CShader()
{
}

CShader::~CShader()
{
    if (m_ppd3dPipelineStates)
    {
        for (int i = 0; i < m_nPipelineStates; i++) if (m_ppd3dPipelineStates[i])
            m_ppd3dPipelineStates[i]->Release();
        delete[] m_ppd3dPipelineStates;
    }
}

void CShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
    XMFLOAT4X4 xmf4x4World;
    XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
    pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CShader::ReleaseShaderVariables()
{
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = NULL;
    d3dInputLayoutDesc.NumElements = 0;
    return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
    D3D12_RASTERIZER_DESC d3dRasterizerDesc;
    ::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    //d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;      // ���̾� ���
    d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;      // �ָ��� ���
    //d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;         // NONE
    //d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;         // FRONT
    d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;         // BACK
    d3dRasterizerDesc.FrontCounterClockwise = FALSE;
    d3dRasterizerDesc.DepthBias = 0;
    d3dRasterizerDesc.DepthBiasClamp = 0.0f;
    d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
    d3dRasterizerDesc.DepthClipEnable = TRUE;
    d3dRasterizerDesc.MultisampleEnable = FALSE;
    d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
    d3dRasterizerDesc.ForcedSampleCount = 0;
    d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    return(d3dRasterizerDesc);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader()
{
    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = 0;
    d3dShaderByteCode.pShaderBytecode = NULL;
    return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader()
{
    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = 0;
    d3dShaderByteCode.pShaderBytecode = NULL;
    return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = 0;
    d3dShaderByteCode.pShaderBytecode = NULL;
    return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = 0;
    d3dShaderByteCode.pShaderBytecode = NULL;
    return(d3dShaderByteCode);
}

D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
    ::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
    d3dDepthStencilDesc.DepthEnable = TRUE;
    d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    d3dDepthStencilDesc.StencilEnable = FALSE;
    d3dDepthStencilDesc.StencilReadMask = 0x00;
    d3dDepthStencilDesc.StencilWriteMask = 0x00;
    d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
    d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
    return(d3dDepthStencilDesc);
}

D3D12_BLEND_DESC CShader::CreateBlendState()
{
    D3D12_BLEND_DESC d3dBlendDesc;
    ::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
    d3dBlendDesc.AlphaToCoverageEnable = FALSE;
    d3dBlendDesc.IndependentBlendEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
    d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
    d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
    d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR* pszFileName, LPCSTR
    pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob)
{
    UINT nCompileFlags = 0;
#if defined(_DEBUG)
    nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile,
        nCompileFlags, 0, ppd3dShaderBlob, NULL);
    D3D12_SHADER_BYTECODE d3dShaderByteCode;
    d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
    d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
    return(d3dShaderByteCode);
}

void CShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
    ::ZeroMemory(&m_d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    m_d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
    m_d3dPipelineStateDesc.VS = CreateVertexShader();
    m_d3dPipelineStateDesc.PS = CreatePixelShader();
    m_d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
    m_d3dPipelineStateDesc.BlendState = CreateBlendState();
    m_d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
    m_d3dPipelineStateDesc.InputLayout = CreateInputLayout();
    m_d3dPipelineStateDesc.SampleMask = UINT_MAX;
    m_d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    m_d3dPipelineStateDesc.NumRenderTargets = 1;
    m_d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    m_d3dPipelineStateDesc.SampleDesc.Count = 1;
    m_d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);
}

//�׷��Ƚ� ���������� ���� ��ü�� �����Ѵ�.
void CShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
    ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
    ::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
    d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
    d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
    d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
    d3dPipelineStateDesc.BlendState = CreateBlendState();
    d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
    d3dPipelineStateDesc.InputLayout = CreateInputLayout();
    d3dPipelineStateDesc.SampleMask = UINT_MAX;
    d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    d3dPipelineStateDesc.NumRenderTargets = 1;
    d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    d3dPipelineStateDesc.SampleDesc.Count = 1;
    d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
        __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);

    if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
    if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
    if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
        d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState)
{
    //���������ο� �׷��Ƚ� ���� ��ü�� �����Ѵ�.
    std::cout << "������ " << m_ppd3dPipelineStates[nPipelineState] << std::endl;

    pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[nPipelineState]);
    std::cout << "�����? " << m_ppd3dPipelineStates[nPipelineState] << std::endl;
}

void CShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
    OnPrepareRender(pd3dCommandList);
}

void CShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
    OnPrepareRender(pd3dCommandList);
    for (int j = 0; j < m_nObjects; j++)
    {
        if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList);
    }
    OnPrepareRender(pd3dCommandList);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
    UINT nInputElementDescs = 2;
    D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
        D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
    pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
    d3dInputLayoutDesc.NumElements = nInputElementDescs;
    return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "VSDiffused", "vs_5_1",
        ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "PSDiffused", "ps_5_1",
        ppd3dShaderBlob));
}

void CPlayerShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
    m_nPipelineStates = 1;
    m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
    CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CObjectsShader::CObjectsShader()
{
}
CObjectsShader::~CObjectsShader()
{
}
D3D12_INPUT_LAYOUT_DESC CObjectsShader::CreateInputLayout()
{
    UINT nInputElementDescs = 2;
    D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
    pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
    d3dInputLayoutDesc.NumElements = nInputElementDescs;
    return(d3dInputLayoutDesc);
}
D3D12_SHADER_BYTECODE CObjectsShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "VSDiffused", "vs_5_1",
        ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CObjectsShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "PSDiffused", "ps_5_1",
        ppd3dShaderBlob));
}
void CObjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
    * pd3dGraphicsRootSignature)
{
    m_nPipelineStates = 1;
    m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
    CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

    // BuildObject
}
void CObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
    * pd3dCommandList)
{
    //����x����x���̰� 12x12x12�� ������ü �޽��� �����Ѵ�.
    CCubeMeshDiffused* pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
        12.0f, 12.0f, 12.0f);
    /*x-��, y-��, z-�� ���� ������ ��ü �����̴�. �� ���� 1�� �ø��ų� ���̸鼭 ������ �� ������ ����Ʈ�� ���
    ���ϴ� ���� ���캸�� �ٶ���.*/
    int xObjects = 10, yObjects = 10, zObjects = 10, i = 0;
    //x-��, y-��, z-������ 21���� �� 21 x 21 x 21 = 9261���� ������ü�� �����ϰ� ��ġ�Ѵ�.
    m_nObjects = (xObjects * 2 + 1) * (yObjects * 2 + 1) * (zObjects * 2 + 1);
    m_ppObjects = new CGameObject * [m_nObjects];
    float fxPitch = 12.0f * 2.5f;
    float fyPitch = 12.0f * 2.5f;
    float fzPitch = 12.0f * 2.5f;
    CRotatingObject* pRotatingObject = NULL;
    for (int z = +zObjects; z >= -zObjects; --z)
    {
        for (int y = -yObjects; y <= yObjects; ++y)
        {
            for (int x = -xObjects; x <= xObjects; ++x)
            {
                pRotatingObject = new CRotatingObject();
                pRotatingObject->SetMesh(pCubeMesh);
                //�� ������ü ��ü�� ��ġ�� �����Ѵ�.
                pRotatingObject->SetPosition(fxPitch * x, fyPitch * y, fzPitch * z);
                pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
                pRotatingObject->SetRotationSpeed(10.0f * (i % 10) + 3.0f);
                m_ppObjects[i++] = pRotatingObject;
            }
        }
    }
    CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
void CObjectsShader::ReleaseObjects()
{
    if (m_ppObjects)
    {
        for (int j = 0; j < m_nObjects; j++)
        {
            if (m_ppObjects[j]) delete m_ppObjects[j];
        }
        delete[] m_ppObjects;
    }
}
void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
    for (int j = 0; j < m_nObjects; j++)
    {
        m_ppObjects[j]->Animate(fTimeElapsed);
    }
}
void CObjectsShader::ReleaseUploadBuffers()
{
    if (m_ppObjects)
    {
        for (int j = 0; j < m_nObjects; j++) {
            std::cout << "j = " << j << std::endl;
            m_ppObjects[j]->ReleaseUploadBuffers();
        }
    }
}
void CObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, const WCHAR* Method)
{
    CShader::Render(pd3dCommandList, pCamera);
    for (int j = 0; j < m_nObjects; ++j)
    {
        if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera, Method);
    }
    std::cout << "�������ε�?" << std::endl;
}


//void CObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
//   * pd3dCommandList)
//{
//}
CInstancingShader::CInstancingShader()
{
}
CInstancingShader::~CInstancingShader()
{
}
D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
{
    UINT nInputElementDescs = 7;
    D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
        D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
    //���� ������ ���� �Է� �����̴�.
    pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };
    pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    //�ν��Ͻ� ������ ���� �Է� �����̴�.
    pd3dInputElementDescs[2] = { "WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
    pd3dInputElementDescs[3] = { "WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
    pd3dInputElementDescs[4] = { "WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,
    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
    pd3dInputElementDescs[5] = { "WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,
    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
    pd3dInputElementDescs[6] = { "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
    64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 };
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
    d3dInputLayoutDesc.NumElements = nInputElementDescs;
    return(d3dInputLayoutDesc);
}
D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "VSInstancing", "vs_5_1",
        ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "PSInstancing", "ps_5_1",
        ppd3dShaderBlob));
}
void CInstancingShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
    * pd3dGraphicsRootSignature)
{
    m_nPipelineStates = 1;
    m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
    CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}
void CInstancingShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
    //�ν��Ͻ� ������ ������ ���� ���۸� ���ε� �� �������� �����Ѵ�. 
    m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL,
        sizeof(VS_VB_INSTANCE) * m_nObjects, D3D12_HEAP_TYPE_UPLOAD,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
    //���� ����(���ε� ��)�� ���� �����͸� �����Ѵ�. 
    m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);
    //���� ���ۿ� ���� �並 �����Ѵ�.
    m_d3dInstancingBufferView.BufferLocation =
        m_pd3dcbGameObjects->GetGPUVirtualAddress();
    m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
    m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE) * m_nObjects;
}
void CInstancingShader::ReleaseShaderVariables()
{
    if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Unmap(0, NULL);
    if (m_pd3dcbGameObjects) m_pd3dcbGameObjects->Release();
}
//�ν��Ͻ� ����(��ü�� ���� ��ȯ ��İ� ����)�� ���� ���ۿ� �����Ѵ�.
void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList
    * pd3dCommandList)
{
    for (int j = 0; j < m_nObjects; j++)
    {
        m_pcbMappedGameObjects[j].m_xmcColor = (j % 2) ? XMFLOAT4(0.5f, 0.0f, 0.0f, 0.0f) :
            XMFLOAT4(0.0f, 0.0f, 0.5f, 0.0f);
        XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform,
            XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
    }
}
void CInstancingShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
    * pd3dCommandList)
{
    int xObjects = 10, yObjects = 10, zObjects = 10, i = 0;
    m_nObjects = (xObjects * 2 + 1) * (yObjects * 2 + 1) * (zObjects * 2 + 1);
    m_ppObjects = new CGameObject * [m_nObjects];
    float fxPitch = 12.0f * 2.5f;
    float fyPitch = 12.0f * 2.5f;
    float fzPitch = 12.0f * 2.5f;
    CRotatingObject* pRotatingObject = NULL;
    for (int x = -xObjects; x <= xObjects; x++)
    {
        for (int y = -yObjects; y <= yObjects; y++)
        {
            for (int z = -zObjects; z <= zObjects; z++)
            {
                pRotatingObject = new CRotatingObject();
                pRotatingObject->SetPosition(fxPitch * x, fyPitch * y, fzPitch * z);
                pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
                pRotatingObject->SetRotationSpeed(10.0f * (i % 10));
                m_ppObjects[i++] = pRotatingObject;
            }
        }
    }
    //�ν��Ͻ��� ����Ͽ� �������ϱ� ���Ͽ� �ϳ��� ���� ��ü�� �޽��� ������.
    CCubeMeshDiffused* pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
        12.0f, 12.0f, 12.0f);
    m_ppObjects[0]->SetMesh(pCubeMesh);
    //�ν��Ͻ��� ���� ���� ���ۿ� �並 �����Ѵ�.
    CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
void CInstancingShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera
    * pCamera)
{
    CShader::Render(pd3dCommandList, pCamera);
    //��� ���� ��ü�� �ν��Ͻ� �����͸� ���ۿ� �����Ѵ�.
    UpdateShaderVariables(pd3dCommandList);
    //�ϳ��� ���� �����͸� ����Ͽ� ��� ���� ��ü(�ν��Ͻ�)���� �������Ѵ�.
    m_ppObjects[0]->Render(pd3dCommandList, pCamera, m_nObjects,
        m_d3dInstancingBufferView);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define INFO_VTX_NUM(x) (x-1)*LINE_SEP_NUM
#define LINE_SEP_NUM 50
void CObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
    * pd3dCommandList, void* pContext)
{
    CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)pContext;
    float fTerrainWidth = pTerrain->GetWidth(), fTerrainLength = pTerrain->GetLength();
    float fxPitch = 30.0f * 3.5f;
    float fyPitch = 30.0f * 3.5f;
    float fzPitch = 30.0f * 3.5f;
    //������ü�� ���� ǥ�鿡 �׸��� �������� ���� ��ġ�� ������ �������� ��ġ�Ѵ�.

    int xObjects = int(fTerrainWidth / fxPitch), yObjects = 1, zObjects = 
    int(fTerrainLength / fzPitch);
    std::cout << xObjects << ", " << zObjects << std::endl;

    //m_nObjects = xObjects * yObjects * zObjects;
    //m_ppObjects = new CGameObject * [m_nObjects];
    //CCubeMeshDiffused* pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
    //    12.0f, 12.0f, 12.0f);
    XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
    //CRotatingObject* pRotatingObject = NULL;
    std::cout << "���� " << std::endl;
    for (int i = 0, x = 0; x < xObjects; x++)
    {
        for (int z = 0; z < zObjects; z++)
        {
            for (int y = 0; y < yObjects; y++)
            {
                //pRotatingObject = new CRotatingObject(1);
                //pRotatingObject->SetMesh(0, pCubeMesh);
                float xPosition = x * fxPitch;
                float zPosition = z * fzPitch;
                //std::cout << "i�� " << i << "�̰� " << "x = " << x << ", y = " << y << ", z = " << z << std::endl << std::endl;
                float fHeight = pTerrain->GetHeight(xPosition, zPosition);
                //pRotatingObject->SetPosition(xPosition, fHeight + (y * 10.0f * fyPitch) +
                //    6.0f, zPosition);
                if (y == 0)
                {
                    /*������ ǥ�鿡 ��ġ�ϴ� ������ü�� ������ ���⿡ ���� ������ �ٸ��� ��ġ�Ѵ�. ������ü�� ��ġ�� ������ ����
                    ���� ����� ������ü�� y-���� ��ġ�ϵ��� �Ѵ�.*/
                    //xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
                    //xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
                    //    xmf3SurfaceNormal);
                    //if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f,
                    //    0.0f);
                    //float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
                    //    xmf3SurfaceNormal));
                    //pRotatingObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
                }
                //pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
                //pRotatingObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
                //m_ppObjects[i++] = pRotatingObject;
            }
        }
    }
    std::cout << "�� " << std::endl << std::endl;
    HermiteSpline hs;
    UINT vtx_n = 7;
    XMFLOAT3 axis_y = XMFLOAT3{ 0.0f,1.0f,0.0f };
    XMFLOAT3* info = new XMFLOAT3[INFO_VTX_NUM(vtx_n)];
    XMFLOAT3* SetPos = new XMFLOAT3[vtx_n]{
		XMFLOAT3{4000.0f, 0.0f, 0.0f}, XMFLOAT3{3700.0f, 0.0f, 1000.0f}, XMFLOAT3{3200.0f, 0.0f, 2000.0f},
        XMFLOAT3{3500.0f, 0.0f, 3000.0f}, XMFLOAT3{4000.0f, 0.0f, 4000.0f}, XMFLOAT3{5200.0f, 0.0f, 5000.0f},
        XMFLOAT3{4900.0f, 0.0f, 6380.0f}
    };
    XMFLOAT3* SetVtx = new XMFLOAT3[vtx_n]{
        XMFLOAT3{ -1000.0f, 0.0f, 0.0f }, XMFLOAT3{ 200.0f, 0.0f, 0.0f }, XMFLOAT3{ -200.0f, 0.0f, 0.0f },
        XMFLOAT3{ -500.0f, 0.0f, 0.0f }, XMFLOAT3{ 1000.0f, 0.0f, 0.0f }, XMFLOAT3{ -300.0f, 0.0f, 0.0f },
        XMFLOAT3{ 300.0f, 0.0f, 0.0f }
    };
    hs.SetPosition(SetPos, vtx_n);
    hs.SetVector(SetVtx);
    hs.SetVector(XMFLOAT3{ 0.0f, 400.0f, 0.0f }, XMFLOAT3{ -100.0f, -200.0f, -200.0f });
    hs.GetInfo(info, LINE_SEP_NUM);
	m_nObjects = INFO_VTX_NUM(vtx_n)*2;
	m_ppObjects = new CGameObject * [m_nObjects];
    CCubeMeshDiffused* pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList,
        12.0f, 12.0f, 12.0f);
	CRotatingObject* pRotatingObject = NULL;
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < INFO_VTX_NUM(vtx_n); ++i) {
			pRotatingObject = new CRotatingObject(info[i].x + 250 * j, pTerrain->GetHeight(info[i].x + 250 * j, info[i].z) + 6.0f, info[i].z, 1);
			pRotatingObject->SetMesh(0, pCubeMesh);
			/*������ ǥ�鿡 ��ġ�ϴ� ������ü�� ������ ���⿡ ���� ������ �ٸ��� ��ġ�Ѵ�. ������ü�� ��ġ�� ������ ����
			���� ����� ������ü�� y-���� ��ġ�ϵ��� �Ѵ�.*/
			xmf3SurfaceNormal = pTerrain->GetNormal(info[i].x, info[i].z);
			xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
				xmf3SurfaceNormal);
			if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f,
				0.0f);
			float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f),
				xmf3SurfaceNormal));
			pRotatingObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
			pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
			pRotatingObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
			m_ppObjects[INFO_VTX_NUM(vtx_n) * j + i] = pRotatingObject;
		}
	}
    CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
CTerrainShader::CTerrainShader()
{
}
CTerrainShader::~CTerrainShader()
{
}
D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
    UINT nInputElementDescs = 2;
    D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new
        D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
    pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
    d3dInputLayoutDesc.NumElements = nInputElementDescs;
    return(d3dInputLayoutDesc);
}
D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "VSDiffused", "vs_5_1",
        ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders.hlsl", "PSDiffused", "ps_5_1",
        ppd3dShaderBlob));
}
void CTerrainShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
    * pd3dGraphicsRootSignature)
{
    m_nPipelineStates = 1;
    m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
    CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPseudoLightingShader::CPseudoLightingShader()
{
}

CPseudoLightingShader::~CPseudoLightingShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPseudoLightingShader::CreateInputLayout()
{
    UINT nInputElementDescs = 2;
    D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

    pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

    D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
    d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
    d3dInputLayoutDesc.NumElements = nInputElementDescs;

    return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPseudoLightingShader::CreateVertexShader()
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders2.hlsl", "VSPseudoLighting", "vs_5_1", &m_pd3dVertexShaderBlob));
}

D3D12_SHADER_BYTECODE CPseudoLightingShader::CreatePixelShader()
{
    return(CShader::CompileShaderFromFile((wchar_t*)L"Shaders2.hlsl", "PSPseudoLighting", "ps_5_1", &m_pd3dPixelShaderBlob));
}

void CPseudoLightingShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
    m_nPipelineStates = 1;
    m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];

    CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

    m_d3dPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&m_d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);

    if (m_pd3dVertexShaderBlob) m_pd3dVertexShaderBlob->Release();
    if (m_pd3dPixelShaderBlob) m_pd3dPixelShaderBlob->Release();

    if (m_d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] m_d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CPseudoLightingShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
    OnPrepareRender(pd3dCommandList, nPipelineState);
}
