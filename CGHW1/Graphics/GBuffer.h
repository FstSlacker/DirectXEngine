#pragma once
#include "RenderTarget.h"
#include <memory>

class GBuffer
{
public:
	RenderTarget rtDiffuse;
	RenderTarget rtEmissive;
	RenderTarget rtNormals;
	RenderTarget rtWorldPos;

	bool Initialize(ID3D11Device* device, UINT width, UINT height)
	{
		if (!rtDiffuse.Initialize(device, width, height))
			return false;
		if (!rtEmissive.Initialize(device, width, height))
			return false;
		if (!rtNormals.Initialize(device, width, height))
			return false;
		if (!rtWorldPos.Initialize(device, width, height))
			return false;

		return true;
	}
};

