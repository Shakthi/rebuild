 
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := ...
CC_FLAGS := ...

main.exe: $(OBJ_FILES)
   g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
   g++ $(CC_FLAGS) -c -o $@ $<

