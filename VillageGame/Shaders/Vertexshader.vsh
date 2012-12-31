struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color
};

VS_OUTPUT main( in float4 Position : POSITION, in float4 Color : COLOR0  )
{
	VS_OUTPUT Output;
	
	Output.Position = Position;
	Output.Diffuse = Color;
	
	return Output;
}