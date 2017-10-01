# Makefile

VULKAN_SDK_PATH = C:/VulkanSDK/1.0.61.1/x86_64

CC=g++
INCLUDES=/include /src $(VULKAN_SDK_PATH)/include 
CFLAGS=-std=c++11 -Wall -I$(INCLUDES)
LDFLAGS= -L$(VULKAN_SDK_PATH)/lib -lglfw3 -lvulkan

MAIN=cpp-vulkan.cpp
SOURCES=shaderprogram.cpp init.cpp app.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cpp-vulkan

all: $(EXECUTABLE)

$(SOURCES): $(OBJECTS)
	$(CC) $(CFLAGS) $(MAIN) -l$(LDFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)