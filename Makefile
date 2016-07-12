CFLAGS   += -g3
LIBS = -lpthread

# Be super strict about everything
CFLAGS   += -std=c11 -Werror -Wall -Wextra -pedantic -O2
CPPFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

# Automatically sort out header dependencies
CPPFLAGS += -MD -MF $(patsubst src/%.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))

CPPFLAGS += -MD -MF $(patsubst src/execution_functions/%.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))

CPPFLAGS += -MD -MF $(patsubst src/structures/%.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))

LDFLAGS += -rdynamic


objs = \
  src/structures/array.o \
  src/structures/huo_ast.o \
  src/structures/string.o \
  src/structures/value.o \
  src/structures/hash_table.o \
  src/structures/token.o \
  src/constants.o \
  src/base_util.o \
  src/core_functions.o \
  src/apply_core_function.o \
  src/execution_functions/parallel_execution.o\
  src/execution_functions/for_each.o\
  src/execution_functions/for_loop.o\
  src/execution_functions/map_array.o\
  src/execution_functions/reduce.o\
  src/execution_functions/read_file.o\
  src/execution_functions/let_binding.o\
  src/execution_functions/reduce_ast.o\
  src/execution_functions/if_block.o\
  src/execution_functions/switch.o\
  src/execution_functions/while_loop.o\
  src/execution_functions/evaluate.o\
  src/execution_functions/read_line.o\
  src/apply_execution_function.o\
  src/apply_single_value_func.o\
  src/tokenizer.o \
  src/process_defs.o \
  src/store_defs.o \
  src/parser.o \
  src/execute.o \
  src/config.o \
  src/huo.o

all: huo

huo: $(objs)
	$(CC) $(LDFLAGS) -o huo $(objs) $(LIBS)

clean: ; rm -f -- ./src/*.mk  ./src/*.o & rm -f ./src/execution_functions/*.o & rm -f ./src/structures/*.o

.PHONY: all clean
.DELETE_ON_ERROR:
