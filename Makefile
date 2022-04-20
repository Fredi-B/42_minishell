NAME = minishell

CC = gcc

CFLAGS = -g -Wall -Wextra -Werror #-g -ggdb3 -fsanitize=address
# -L followed by the relative path of the library to link it 
LINUX_LIBRARIES = -L ./libft -lft -L/usr/include/readline -lreadline
MAC_LIBRARIES = -L ./libft -lft -L$(HOME)/.brew/opt/readline/lib -lreadline
MAC_INCLUDES = -I$(HOME)/.brew/opt/readline/include 
CFILES = \
main.c \
signals_v2.c \
builtin_main.c builtin_cd.c builtin_echo.c builtin_export.c builtin_unset.c \
free_fcts.c err_exit.c \
parsing_lexer.c parsing_lexer_utils.c parsing_split_pipe.c parsing_split_pipe_utils.c\
parsing_tokens.c parsing_tokens_utils.c parsing_tokens_dollar.c parsing_trim_quotes.c\
parsing.c parsing_ins_and_outs.c parsing_cmd_arr.c \
parsing_here_docs.c parsing_here_docs_utils.c \
launch_cmds.c launch_cmds_utils.c launch_pipes.c launch_prep_fd_in.c launch_prep_fd_out.c \
#test_fcts.c #delete?

VPATH = $(ODIR)
ODIR = o-files
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
UNAME_S := $(shell uname -s)
$(NAME): $(OBJECTS) 
	@make -C ./libft
ifeq ($(UNAME_S),Linux)
	clang $(CFLAGS) $(addprefix $(ODIR)/,$(OBJECTS)) -lm $(LINUX_LIBRARIES) -o $(NAME)
else
	$(CC) $(CFLAGS) $(addprefix $(ODIR)/,$(OBJECTS)) -lm $(MAC_LIBRARIES) -o $(NAME)
endif

%.o: %.c minishell.h
ifeq ($(UNAME_S),Linux)
	clang $(CFLAGS) -c $< -o $(ODIR)/$@
else
	$(CC) $(CFLAGS) $(MAC_INCLUDES) -c $< -o $(ODIR)/$@ 
endif


.phony: all
all: $(NAME)

.phony: clean
clean: clean_libft 
	rm -f $(ODIR)/*.o

.phony: fclean
fclean: clean fclean_libft
	rm -f $(NAME) *~

.phony: re
re: fclean all

# @ makes it silence
.phony: clean_libft
clean_libft:
	@make clean -C ./libft/

.phony: fclean_libft
fclean_libft:
	@make fclean -C ./libft/
