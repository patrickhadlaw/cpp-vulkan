# Makefile

VULKAN_SDK_PATH = C:/VulkanSDK/1.0.61.1

CC=g++
INCLUDEDIRS=-I/include -I/src -I$(VULKAN_SDK_PATH)/Include
LDFLAGS= -L$(VULKAN_SDK_PATH)/lib -L/lib -lglfw3 -lvulkan
CFLAGS=-std=c++11 -Wall -g $(LDFLAGS) $(INCLUDEDIRS)

MAIN=cpp-vulkan.cpp
SOURCES=src/shaderprogram.cpp src/init.cpp src/app.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cpp-vulkan

all: $(OBJECTS)

$(SOURCES):
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS)

$(EXECUTABLE):
	$(CC) $(CFLAGS) $(MAIN) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)