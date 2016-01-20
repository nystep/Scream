INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib -L/usr/lib/x86_64-linux-gnu


FLAGS = -Wall
CC = g++
CFLAGS = $(FLAGS) $(INCLUDE)
LDFLAGS = 
LIBS =  -lGL -lGLU -lGLEW -lm -lX11 -lXext -lwebp -lIL -lILUT
OBJECTS = tinyxml2.o blendMode.o director.o layer.o layerParam.o camera.o context_linux.o dataBuffer.o dataFile.o dct.o gpuRadixSort.o iobFile.o iobHelper.o meshAttributeSet.o meshDDPTransform.o meshDistanceTransform.o meshGpu.o meshNSFLoader.o meshVBO.o radixSort.o sampler.o shader.o shaderBufferSet.o shaderConstantBuffer.o shaderStringBuffer.o shaderTextureSet.o shaderUniform.o shaderUniformSet.o smath.o spline.o texgen.o texture.o texture2d.o texture2dArray.o texture3d.o textureBuffer.o textureCubeMap.o textureCubeMapArray.o textureUtils.o textureVideo.o context.o canvas.o layerResource.o main.o

All: scream

scream: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBDIR) $(LIBS)

clean:
	rm -rf demo $(OBJECTS)
