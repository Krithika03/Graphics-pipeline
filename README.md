# Graphics-pipeline
This work mimics OpenGL in the following tasks: <br> <br>
• Matrix manipulation: When translated or rotated the camera along its own axis, calculated the model-view matrix. Also, when the vertex projection is done from CCS to image plane, the projection matrix is implemented. It is exactly the same as the matrix when gluLookat() or glPerspective() is used. <br>
• Shader-based rendering: instead of using the OpenGL fixed rendering pipeline, used GLSL to write my own vertex shader and fragment shader. In the vertex shader, for vertex position transformation by passed my own model-view and projection matrix. In the fragment shader, proper color was set. <br>
Goal: Covered geometrical transformations, perspective projection, OpenGL pipeline and basic GLSL grammar. 
