# GLTF-loader
Simple model loader of format gltf/glb


So these are just a simple model loader of file(s) format gltf/glb.
You can read about them from here -> https://github.khronos.org/glTF-Tutorials/gltfTutorial/



Requirements ->

The things that you need to set it up and running are basic stuff that mostly all of you
seeing this might know ->

1. Basics of opengl(obvious)
2. tinygltf -> https://github.com/syoyo/tinygltf
3. glad(glad 1 specifically) -> https://glad.dav1d.de/
4. stb image loader -> https://github.com/nothings/stb/blob/master/stb_image.h
5. A custom OpenGL Shader file. I used the one from learnopengl.com -> https://learnopengl.com/Getting-started/Shaders



How to use ->

Just add it in your main file as #include "/path/to/gltLoader.h" , and make sure to define all implementations once and properly
