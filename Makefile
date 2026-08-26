NAME       := ft_vox

CXX        := c++
CC         := cc
CXXSTD     ?= c++17

SRC_DIR    := src
OBJ_DIR    := obj
EXT_DIR    := external

GLAD_DIR   := $(EXT_DIR)/glad
GLFW_DIR   := $(EXT_DIR)/glfw
GLFW_BUILD := $(GLFW_DIR)/build
GLFW_LIB   := $(GLFW_BUILD)/src/libglfw3.a

CXXFLAGS   := -Wall -Wextra -Werror -std=$(CXXSTD) -MMD -MP
CPPFLAGS   := -I$(SRC_DIR) -I$(GLAD_DIR)/include -I$(GLFW_DIR)/include
LDFLAGS    :=
LDLIBS     := $(GLFW_LIB) -ldl -lpthread -lm

SRCS       := $(addprefix src/,main.cpp \
		$(addprefix app/,application.cpp engine.cpp) \
		$(addprefix platform/,glfw_context.cpp window.cpp glad_context.cpp))
OBJS       := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GLAD_OBJ   := $(OBJ_DIR)/glad.o
DEPS       := $(OBJS:.o=.d)

RM         := rm -f

# -----------------------------------------------------------------------------

all: $(NAME)

$(NAME): $(GLFW_LIB) $(OBJS) $(GLAD_OBJ)
	$(CXX) $(LDFLAGS) $(OBJS) $(GLAD_OBJ) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_DIR)/src/glad.c
	@mkdir -p $(dir $@)
	$(CC) -O2 -I$(GLAD_DIR)/include -c $< -o $@

$(GLFW_LIB):
	cmake -S $(GLFW_DIR) -B $(GLFW_BUILD) \
		-D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF \
		-D GLFW_BUILD_DOCS=OFF -D GLFW_BUILD_WAYLAND=ON -D GLFW_BUILD_X11=OFF \
		-D BUILD_SHARED_LIBS=OFF
	cmake --build $(GLFW_BUILD) --parallel

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) compile_commands.json

fclean-glfw: fclean
	$(RM) -r $(GLFW_BUILD)

re: fclean
	$(MAKE) all

cdb:
	@command -v bear >/dev/null 2>&1 || \
		{ echo "cdb: bear not found (install bear)"; exit 1; }
	$(MAKE) $(GLFW_LIB)
	$(MAKE) fclean
	bear -- $(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean fclean-glfw re cdb
