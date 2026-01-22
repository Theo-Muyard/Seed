# | ================================================ |
# 						NAMES
# | ================================================ |

NAME				=	seed_test
BUILD_DIR			=	build/tests

# | ================================================ |
# 					COMPILATION
# | ================================================ |

CC					=	cc
CFLAGS				=	-Wall -Wextra -Werror

# | ================================================ |
# 					INCLUDES
# | ================================================ |
INCLUDES			=	-I includes -I includes/systems

# | ================================================ |
# 					SOURCE FILES
# | ================================================ |

WRITING_SRC			=	src/systems/writing.c \
						tests/systems/TEST_writing.c

# | ================================================ |
# 					OBJ FILES
# | ================================================ |

WRITING_OBJ			=	$(addprefix $(BUILD_DIR)/, $(notdir $(WRITING_SRC:.c=.o)))

# | ================================================ |
# 					COLORS / WIDTH
# | ================================================ |

COL_WIDTH	=	30
RED			=	\033[31m
GREEN		=	\033[32m
BLUE		=	\033[34m
WHITE		=	\033[37m

# | ================================================ |
# 					TEST FUNCTION
# | ================================================ |

define TEST_RULE
$1: OBJ := $2
$1: $(NAME)
endef

# | ================================================ |
# 					COMPILE FUNCTION
# | ================================================ |

define COMPILE_OBJ
$(BUILD_DIR)/$(notdir $(1:.c=.o)): $(1) | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $$< -o $$@ $(INCLUDES)
	@printf "$(BLUE)%-$(COL_WIDTH)s$(WHITE): ✔️\n" "$$(notdir $$@)"
endef

# | ================================================ |
# 					MAKE RULE
# | ================================================ |

all:
	@echo "$(BLUE)Usage$(WHITE): make test TARGET=<target>"

$(NAME): $(BUILD_DIR) $(WRITING_OBJ)
	@$(CC) $(CFLAGS) $(WRITING_OBJ) -o $(NAME)
	@echo "$(GREEN)Done$(WHITE)."

# | ================================================ |
# 					RUN RULE
# | ================================================ |

run:
	@echo "$(BLUE)Run$(WHITE) the test...\n"
	./$(NAME)
	@echo "\nTest $(RED)ended$(WHITE)."

# | ================================================ |
# 					WRITING RULE
# | ================================================ |
$(eval $(call TEST_RULE, writing, $(WRITING_OBJ)))

# | ================================================ |
# 					DIRECTORY
# | ================================================ |

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@printf "$(BLUE)%-$(COL_WIDTH)s$(WHITE): ✔️\n" "$(BUILD_DIR) (directory)"

# | ================================================ |
# 					OBJECTS
# | ================================================ |

$(foreach src, $(WRITING_SRC), $(eval $(call COMPILE_OBJ,$(src))))

.PHONY: all run writing