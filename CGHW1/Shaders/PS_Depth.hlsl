struct PS_IN
{  
    float4 pos : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};


float4 PSMain(PS_IN input) : SV_TARGET
{
	float depthValue;
	float4 color;
	
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    //input.depthPosition.w = 10.0f;
	depthValue = input.depthPosition.z / input.depthPosition.w;

    color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}
