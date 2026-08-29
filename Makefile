NAME       := ft_vox

CXX        := c++
CC         := cc
CXXSTD     ?= c++26

SRC_DIR    := src
OBJ_DIR    := obj
EXT_DIR    := external

GLAD_DIR   := $(EXT_DIR)/glad
GLM_DIR    := $(EXT_DIR)/glm
STB_DIR    := $(EXT_DIR)/stb_image

CXXFLAGS   := -Wall -Wextra -Werror -std=$(CXXSTD) -MMD -MP
CPPFLAGS   := -I$(SRC_DIR) -I$(GLAD_DIR)/include -I$(GLM_DIR) -I$(STB_DIR)
LDFLAGS    :=
LDLIBS     := -ldl -lpthread -lm -lglfw -lGL -lX11 -lXi

SRCS       := $(addprefix src/,main.cpp \
		$(addprefix app/,application.cpp engine.cpp) \
		$(addprefix platform/,glfw_context.cpp window.cpp glad_context.cpp) \
		$(addprefix render/,shader.cpp texture.cpp) \
		$(addprefix world/,camera.cpp chunk.cpp))
OBJS       := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GLAD_OBJ   := $(OBJ_DIR)/glad.o
DEPS       := $(OBJS:.o=.d)

RM         := rm -f

# -----------------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJS) $(GLAD_OBJ)
	$(CXX) $(LDFLAGS) $(OBJS) $(GLAD_OBJ) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_DIR)/src/glad.c
	@mkdir -p $(dir $@)
	$(CC) -O2 -I$(GLAD_DIR)/include -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) compile_commands.json

re: fclean
	$(MAKE) all

cdb:
	@command -v bear >/dev/null 2>&1 || \
		{ echo "cdb: bear not found (install bear)"; exit 1; }
	$(MAKE) fclean
	bear -- $(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re cdb
