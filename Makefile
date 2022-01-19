###############################################################################
##								MAKEFILE									 ##
##								ygeslin 									 ##
###############################################################################

NAME			=	webserv

# FLAGS
CC				= clang++
FLAGS 			= -Wall -Wextra -Werror -std=c++98

# DIRECTORIES
SRCS_DIR		= ./
BUILDING_DIR	= ./building_dir/
INCLUDES_DIR	= ./


# FILES
SRCS			= 	main \
					httpContext \
					utils \
					webserv

FILES			= $(addsuffix .cpp, $(SRCS))
OBJECTS			= $(addprefix $(BUILDING_DIR),$(FILES:.cpp=.o))

# BUILDING RULES
all:			$(NAME)

$(BUILDING_DIR)%.o:$(SRCS_DIR)%.cpp
				mkdir -p building_dir
				$(CC) $(FLAGS) -I ${INCLUDES_DIR} -c $< -o $@

${NAME}:		${OBJECTS}
				$(CC) $(FLAGS) ${OBJECTS}  -o ${NAME}

# CLEANING RULES
clean:
				rm -Rf building_dir

fclean:			clean
				rm -f $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
