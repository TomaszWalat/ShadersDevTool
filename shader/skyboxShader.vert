#version 460

layout (location = 0) in vec3 VertexPosition;

//out vec3 Vec;
//out vec4 vecPosition;
out vec3 TexCoord;

//uniform mat4 MVP;
uniform mat4 ViewProjectionMatrix;
//uniform mat4 ModelMatrix;
//uniform mat4 ViewMatrix;
//uniform mat4 ProjectionMatrix;

void main()
{
// Junk
//    vecPosition = (MVP * vec4(VertexPosition, 1.0));
//    vecPosition = (ProjectionMatrix * ViewMatrix * vec4(VertexPosition, 1.0)).xyz;
//    vecPosition = ProjectionMatrix * ViewMatrix * vec4(VertexPosition, 1.0);
//    vecPosition = vec4(VertexPosition, 1.0);
//    TexCoord = vec3(vecPosition.x, vecPosition.y, -vecPosition.z);
//    gl_Position =  MVP * vec4(VertexPosition, 1.0);
//    gl_Position =  ModelViewMatrix /*MVP*/ * vec4(VertexPosition, 1.0);
//    gl_Position = vec4(VertexPosition.x, vecPosition.y, vecPosition.w, vecPosition.w);


//// Marius code - broke; MVP has been changed to VP matrix due to my Model usage - not that it makes a difference
//    Vec = (ProjectionMatrix * ViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
//    gl_Position = ProjectionMatrix * ViewMatrix * vec4(VertexPosition, 1.0f);

// Correct for my Model usage - og
//    vecPosition = (ProjectionMatrix * ViewMatrix * vec4(VertexPosition, 1.0));
//    TexCoord = VertexPosition;
//    gl_Position = vecPosition; 
// simplified
    TexCoord = VertexPosition;
    gl_Position = (ViewProjectionMatrix * vec4(VertexPosition, 1.0));
}
