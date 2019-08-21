#pragma once

#include "engine/core/render/interface/FrameBuffer.h"
#include "mt_render_base.h"

namespace Echo
{
    class MTFrameBufferWindow : public FrameBuffer
    {
    public:
        MTFrameBufferWindow(ui32 width, ui32 height);
        virtual ~MTFrameBufferWindow();

        // begin render
        virtual bool begin(bool clearColor, const Color& backgroundColor, bool clearDepth, float depthValue, bool clearStencil, ui8 stencilValue) override;
        virtual bool end() override;

        // on resize
        virtual void onSize(ui32 width, ui32 height) override;
        
    private:
    };
}