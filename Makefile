# Makefile

VULKAN_SDK_PATH = C:/VulkanSDK/1.0.61.1/x86_64

CC=g++
INC=/include /src $(VULKAN_SDK_PATH)/include 
CFLAGS=-std=c++11 -Wall $(INC)
LDFLAGS= -L$(VULKAN_SDK_PATH)/lib -lglfw3 -lvulkan

SOURCES=cpp-vulkan.cpp shaderprogram.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cpp-vulkan

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)

	$(CC) -I$(MYINCLUDES) $(SOURCES) -o$(EXECUTABLE) -l$(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)