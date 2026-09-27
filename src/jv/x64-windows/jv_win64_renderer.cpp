#include "jv/jv.h"
#include <jv/x64-windows/jv_win64_impl.h>

#include <cmath>
#include <deque>
#include <list>
#include <numbers>
#include <string>

#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi.h>

struct jv::gpu::VertexBuffer
{
    std::string name;
    size_t count = 0;
    ID3D11Buffer* buffer = nullptr;
    bool bDestroy = false;
};

struct jv::gpu::Texture
{
    std::string name;
    jv::util::Vec2 size = {};
    ID3D11Texture2D* pTexture = nullptr;
    ID3D11ShaderResourceView* pTextureView = nullptr;
    ID3D11SamplerState* pTextureSampler = nullptr;
    bool bDestroy = false;
};

struct Renderer
{
    jv::util::Vec2 size = {};
    jv::util::Vec2 renderSize = {};
    jv::util::Color backgroundColor = {};
    jv::gpu::Texture* pWhiteTexture = nullptr;

    bool bInsideDrawFrame = false;

    std::list<jv::gpu::VertexBuffer> vertexBuffers;
    std::list<jv::gpu::Texture> textures;

    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapchain = nullptr;
    ID3D11RenderTargetView* pRtv = nullptr;
    ID3D11VertexShader* pVertexShader = nullptr;
    ID3D11PixelShader* pPixelShader = nullptr;
    ID3D11InputLayout* pInputLayout = nullptr;
    ID3D11Buffer* pConstantBuffer = nullptr;
    ID3D11RasterizerState* pRasterState = nullptr;
    ID3D11BlendState* pBlendState = nullptr;
};

static Renderer g_rdr;

void jv::win64::InitRenderer(HWND hWindow)
{
    // Determine initial renderer size based on window size.
    RECT windowRect;
    GetClientRect(hWindow, &windowRect);
    HRESULT hr = 0;

    g_rdr.size.x = windowRect.right - windowRect.left;
    g_rdr.size.y = windowRect.bottom - windowRect.top;
    g_rdr.renderSize = g_rdr.size;

    DXGI_SWAP_CHAIN_DESC swapchainDesc{};
    swapchainDesc.BufferDesc.Width = g_rdr.size.x;
    swapchainDesc.BufferDesc.Height = g_rdr.size.y;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 1;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 60; // 60 fps
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.OutputWindow = hWindow;
    swapchainDesc.Windowed = TRUE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.Flags = 0;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL outFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        // D3D11_CREATE_DEVICE_DEBUG,
        &featureLevel, 1, D3D11_SDK_VERSION, &swapchainDesc, &g_rdr.pSwapchain, &g_rdr.pDevice,
        &outFeatureLevel, &g_rdr.pContext);
    if (FAILED(hr))
        jv::util::Panic("D3D11CreateDeviceAndSwapChain failed");

    ID3D11Texture2D* pBackbuffer = nullptr;
    hr = g_rdr.pSwapchain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&pBackbuffer);
    if (FAILED(hr))
        jv::util::Panic("g_rdr.pswapchain->GetBuffer failed");

    hr = g_rdr.pDevice->CreateRenderTargetView(pBackbuffer, nullptr, &g_rdr.pRtv);
    if (FAILED(hr))
        jv::util::Panic("g_rdr.pdevice->CreateRenderTargetView failed");
    pBackbuffer->Release();

    ID3DBlob* pVertexShaderBlob = nullptr;
    ID3DBlob* pPixelShaderBlob = nullptr;
    ID3DBlob* pErrorMsg = nullptr;

    hr = D3DCompile(jv::win64::kVertexShader, strlen(jv::win64::kVertexShader), nullptr, nullptr,
        nullptr, "main", "vs_5_0", 0, 0, &pVertexShaderBlob, &pErrorMsg);
    if (FAILED(hr))
        jv::util::Panic("D3DCompile vertex failed");

    hr = D3DCompile(jv::win64::kPixelShader, strlen(jv::win64::kPixelShader), nullptr, nullptr,
        nullptr, "main", "ps_5_0", 0, 0, &pPixelShaderBlob, &pErrorMsg);
    if (FAILED(hr))
        jv::util::Panic("D3DCompile pixel failed");

    hr = g_rdr.pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(), nullptr, &g_rdr.pVertexShader);
    if (FAILED(hr))
        jv::util::Panic("CreateVertexShader failed");

    hr = g_rdr.pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(),
        pPixelShaderBlob->GetBufferSize(), nullptr, &g_rdr.pPixelShader);
    if (FAILED(hr))
        jv::util::Panic("CreatePixelShader failed");

    D3D11_INPUT_ELEMENT_DESC inputs[3]{};
    inputs[0].SemanticName = "POSITION";
    inputs[0].SemanticIndex = 0;
    inputs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputs[0].InputSlot = 0;
    inputs[0].AlignedByteOffset = offsetof(jv::gpu::Vertex, pos);
    inputs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputs[1].SemanticName = "TEXCOORD";
    inputs[1].SemanticIndex = 0;
    inputs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputs[1].InputSlot = 0;
    inputs[1].AlignedByteOffset = offsetof(jv::gpu::Vertex, uv);
    inputs[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputs[2].SemanticName = "COLOR";
    inputs[2].SemanticIndex = 0;
    inputs[2].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    inputs[2].InputSlot = 0;
    inputs[2].AlignedByteOffset = offsetof(jv::gpu::Vertex, color);
    inputs[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    hr = g_rdr.pDevice->CreateInputLayout(inputs, 3, pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(), &g_rdr.pInputLayout);
    if (FAILED(hr))
        jv::util::Panic("CreateInputLayout failed");

    pVertexShaderBlob->Release();
    pPixelShaderBlob->Release();

    D3D11_BUFFER_DESC cbufferDesc = {};
    cbufferDesc.ByteWidth = sizeof(jv::win64::ShaderCBuffer);
    cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = g_rdr.pDevice->CreateBuffer(&cbufferDesc, nullptr, &g_rdr.pConstantBuffer);
    if (FAILED(hr))
        jv::util::Panic("CreateBuffer constant failed");

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = g_rdr.pDevice->CreateRasterizerState(&rasterDesc, &g_rdr.pRasterState);
    if (FAILED(hr))
        jv::util::Panic("CreateRasterizerState failed");

    D3D11_BLEND_DESC blendDesc = {0};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED |
                                                      D3D11_COLOR_WRITE_ENABLE_GREEN |
                                                      D3D11_COLOR_WRITE_ENABLE_BLUE;
    hr = g_rdr.pDevice->CreateBlendState(&blendDesc, &g_rdr.pBlendState);
    if (FAILED(hr))
        jv::util::Panic("CreateBlendState failed");

    jv::util::Color color = {255, 255, 255, 255};
    g_rdr.pWhiteTexture = jv::gpu::CreateTexture("white", {1, 1}, {&color, 1});
}

void jv::win64::ResizeRenderer(jv::util::Vec2 size)
{
    g_rdr.pContext->Flush();
    g_rdr.pRtv->Release();
    g_rdr.pRtv = nullptr;

    HRESULT hr = g_rdr.pSwapchain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    if (FAILED(hr))
        jv::util::Panic("g_rdr.pSwapchain->ResizeBuffers failed");

    ID3D11Texture2D* pBackbuffer = nullptr;
    hr = g_rdr.pSwapchain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&pBackbuffer);
    if (FAILED(hr))
        jv::util::Panic("g_rdr.pSwapchain->GetBuffer failed");

    hr = g_rdr.pDevice->CreateRenderTargetView(pBackbuffer, nullptr, &g_rdr.pRtv);
    if (FAILED(hr))
        jv::util::Panic("g_rdr.pDevice->CreateRenderTargetView failed");
    pBackbuffer->Release();

    g_rdr.size = size;
}

void jv::win64::ShutRenderer()
{
    // XDino_DestroyVertexBuffer(XDino_VBUFID_EMPTY);
    // XDino_DestroyGpuTexture(XDino_TEXID_FONT);
    // XDino_DestroyGpuTexture(XDino_TEXID_WHITE);
    // XDino_Win64_PurgeDeadResources();

    // OutputDebugStringA("--- RESOURCES ALIVE BEGIN ---\n");
    // for (std::string const& s : XDino_Win64_CollectRessources())
    //     OutputDebugStringA((s + "\n").c_str());
    // OutputDebugStringA("--- RESOURCES ALIVE END ---\n");
    // std::puts("--- RESOURCES ALIVE BEGIN ---");
    // for (std::string s : XDino_Win64_CollectRessources())
    //     std::puts(s.c_str());
    // std::puts("--- RESOURCES ALIVE END ---");

    // for (auto& [name, texture] : g_rdr.ptextures) {
    //     texture.pTextureSampler->Release();
    //     texture.pTextureView->Release();
    //     texture.pTexture->Release();
    // }
    // g_rdr.ptextures.clear();

    g_rdr.pBlendState->Release();
    g_rdr.pBlendState = nullptr;
    g_rdr.pRasterState->Release();
    g_rdr.pRasterState = nullptr;
    g_rdr.pConstantBuffer->Release();
    g_rdr.pConstantBuffer = nullptr;
    g_rdr.pInputLayout->Release();
    g_rdr.pInputLayout = nullptr;
    g_rdr.pPixelShader->Release();
    g_rdr.pPixelShader = nullptr;
    g_rdr.pVertexShader->Release();
    g_rdr.pVertexShader = nullptr;
    g_rdr.pRtv->Release();
    g_rdr.pRtv = nullptr;
    g_rdr.pContext->Release();
    g_rdr.pContext = nullptr;
    g_rdr.pSwapchain->Release();
    g_rdr.pSwapchain = nullptr;
    g_rdr.pDevice->Release();
    g_rdr.pDevice = nullptr;
}

void jv::gpu::SetBackgroundColor(util::Color color) noexcept
{
    g_rdr.backgroundColor = color;
}

void jv::gpu::SetRenderSize(util::Vec2 renderSize) noexcept
{
    if (renderSize.x < 1)
        renderSize.x = 1;
    if (renderSize.y < 1)
        renderSize.y = 1;
    g_rdr.renderSize = renderSize;
}

jv::util::Vec2 jv::gpu::GetRenderSize() noexcept
{
    return g_rdr.renderSize;
}

jv::gpu::VertexBuffer* jv::gpu::CreateVertexBuffer(
    std::string_view label, std::span<Vertex const> vertices)
{

    ID3D11Buffer* buf = nullptr;
    if (vertices.size() > 0)
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = static_cast<UINT>(vertices.size_bytes());
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA bufferData = {};
        bufferData.pSysMem = vertices.data();

        HRESULT hr = g_rdr.pDevice->CreateBuffer(&bufferDesc, &bufferData, &buf);
        if (FAILED(hr))
            jv::util::Panic("CreateBuffer vertex failed");
    }

    VertexBuffer& vbuf = g_rdr.vertexBuffers.emplace_back();
    vbuf.name = label;
    vbuf.count = vertices.size();
    vbuf.buffer = buf;
    return &vbuf;
}

void jv::gpu::DestroyVertexBuffer(VertexBuffer* pVertexBuffer)
{
    if (!pVertexBuffer)
        return;
    if (pVertexBuffer->bDestroy)
        jv::util::Panic("Destruction d'un vertex buffer déjà détruite.");
    if (pVertexBuffer->count > 0)
        pVertexBuffer->buffer->Release();
    pVertexBuffer->bDestroy = true;
}

jv::gpu::Texture* jv::gpu::CreateTexture(
    std::string_view label, jv::util::Vec2 textureSize, std::span<jv::util::Color const> pixels)
{
    if (pixels.size() != textureSize.x * textureSize.y)
        jv::util::Panic("Pixel count does not match texture size.");
    if (pixels.size() == 0)
        return nullptr;

    HRESULT hr;

    jv::gpu::Texture& texture = g_rdr.textures.emplace_back();

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = textureSize.x;
    texDesc.Height = textureSize.y;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels.data();
    initData.SysMemPitch = 4 * texDesc.Width;
    initData.SysMemSlicePitch = 4 * texDesc.Width * texDesc.Height;
    hr = g_rdr.pDevice->CreateTexture2D(&texDesc, &initData, &texture.pTexture);
    if (FAILED(hr))
        jv::util::Panic("CreateTexture2D failed");

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    hr = g_rdr.pDevice->CreateShaderResourceView(texture.pTexture, &srvDesc, &texture.pTextureView);
    if (FAILED(hr))
        jv::util::Panic("CreateShaderResourceView failed");

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    hr = g_rdr.pDevice->CreateSamplerState(&samplerDesc, &texture.pTextureSampler);
    if (FAILED(hr))
        jv::util::Panic("CreateSamplerState failed");

    texture.name = label;
    texture.size = textureSize;
    return &texture;
}

void jv::gpu::DestroyTexture(Texture* pTexture)
{
    if (!pTexture)
        return;
    if (pTexture->bDestroy)
        jv::util::Panic("Destruction d'une texture déjà détruite.");

    pTexture->pTextureSampler->Release();
    pTexture->pTextureView->Release();
    pTexture->pTexture->Release();
    pTexture->bDestroy = true;
}

void jv::win64::BeginRendererDraw()
{
    jv::util::Color color = g_rdr.backgroundColor;
    float clearColor[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
    g_rdr.pContext->ClearRenderTargetView(g_rdr.pRtv, clearColor);

    g_rdr.pContext->IASetInputLayout(g_rdr.pInputLayout);
    g_rdr.pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_rdr.pContext->VSSetShader(g_rdr.pVertexShader, nullptr, 0);
    g_rdr.pContext->VSSetConstantBuffers(0, 1, &g_rdr.pConstantBuffer);

    float width = g_rdr.size.x;
    float height = g_rdr.size.x * g_rdr.renderSize.y / g_rdr.renderSize.x;
    if (height > g_rdr.size.y)
    {
        width = g_rdr.size.y * g_rdr.renderSize.x / g_rdr.renderSize.y;
        height = g_rdr.size.y;
        ;
    }

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = (g_rdr.size.x - width) / 2;
    viewport.TopLeftY = (g_rdr.size.y - height) / 2;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    g_rdr.pContext->RSSetViewports(1, &viewport);
    g_rdr.pContext->RSSetState(g_rdr.pRasterState);

    g_rdr.pContext->PSSetShader(g_rdr.pPixelShader, nullptr, 0);

    g_rdr.pContext->OMSetRenderTargets(1, &g_rdr.pRtv, nullptr);

    g_rdr.pContext->OMSetBlendState(g_rdr.pBlendState, nullptr, 0xFFFFFFFF);

    g_rdr.bInsideDrawFrame = true;
}

void jv::gpu::Draw(VertexBuffer* pVertices, Texture* pTexture, Transform transform)
{
    D3D11_MAPPED_SUBRESOURCE resource;

    if (pVertices && pVertices->bDestroy)
        jv::util::Panic("Vertex buffer déjà détruit!");
    if (pTexture && pTexture->bDestroy)
        jv::util::Panic("Texture déjà détruite!");
    if (!g_rdr.bInsideDrawFrame)
        jv::util::Panic("jv::gpu::Draw() appelé en dehors de jv::game::Draw()");

    if (!pVertices || pVertices->count == 0)
        return;
    if (!pTexture)
        pTexture = g_rdr.pWhiteTexture;

    g_rdr.pContext->PSSetShaderResources(0, 1, &pTexture->pTextureView);
    g_rdr.pContext->PSSetSamplers(0, 1, &pTexture->pTextureSampler);

    // Mettre à jour les transformations.

    double rotationRadians = transform.rotation * (std::numbers::pi / 180.0);
    jv::win64::ShaderCBuffer cbuffer{};
    cbuffer.half_vp_size = {g_rdr.renderSize.x / 2.f, g_rdr.renderSize.y / 2.f};
    cbuffer.tex_size = pTexture->size;
    cbuffer.offset = transform.translation;
    cbuffer.rot_cos_sin = {std::cosf(rotationRadians), std::sinf(rotationRadians)};
    cbuffer.scale = transform.scale;

    g_rdr.pContext->Map(g_rdr.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, &cbuffer, sizeof(cbuffer));
    g_rdr.pContext->Unmap(g_rdr.pConstantBuffer, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_rdr.pContext->IASetVertexBuffers(0, 1, &pVertices->buffer, &stride, &offset);
    g_rdr.pContext->Draw(static_cast<UINT>(pVertices->count), 0);
}

void jv::win64::EndRendererDraw()
{
    // if (g_rdr.pbDrawStats) {
    //     std::vector<std::string> lines = XDino_Win64_CollectRessources(true);

    //    g_rdr.pstatScroll += g_rdr.pstatScrollUser;
    //    g_rdr.pstatScroll = XDinoImpl_DrawStats(g_rdr.pstatScroll, g_rdr.prdrHeight, 1,
    //    std::move(lines));

    //    g_rdr.pbDrawStats = false;
    //}
    {
        auto it = g_rdr.vertexBuffers.begin();
        while (it != g_rdr.vertexBuffers.end())
            if (it->bDestroy)
                it = g_rdr.vertexBuffers.erase(it);
            else
                ++it;
    }
    {
        auto it = g_rdr.textures.begin();
        while (it != g_rdr.textures.end())
            if (it->bDestroy)
                it = g_rdr.textures.erase(it);
            else
                ++it;
    }

    // XDino_ProfileBegin({0x44, 0x44, 0x44, 0xFF}, "SwapchainPresent");
    g_rdr.pSwapchain->Present(1, 0);
    // XDino_ProfileEnd();
}
