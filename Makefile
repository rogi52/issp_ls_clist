TARGET = selector
SRCDIR    = ./src
INCLUDE = $(wildcard $(SRCDIR)/*.hpp)
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)
CXX = clang++
CXXFLAGS = -O2 -DNDEBUG -std=c++17 -lpagmo -lboost_serialization -fpermissive 

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(CXX) -o  $(TARGET) $^ $(CXXFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS)  $<

clean:
	rm -rf $(OBJS) $(DEPS) $(TARGET)