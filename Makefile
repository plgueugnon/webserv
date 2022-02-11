# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/10 12:56:57 by pgueugno          #+#    #+#              #
#    Updated: 2022/02/07 14:25:45 by pgueugno         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                               MAKEFILE                                       #
#                            by Yannou & PL <3                                 #
################################################################################

#_______________________________Filename output________________________________#

NAME			=	webserv

#______________________________Commands and arguments__________________________#

CXX				= @ clang++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98 -g3 -MMD
DEBUG_LEAK		= -fsanitize=address
DEBUG_THREAD	= -fsanitize=thread #-lpthread
RM				= @ rm -f

#_________________________Sources and objects directories______________________#

SRCS_DIR		= ./
BUILDING_DIR	= ./building_dir/
INCLUDES_DIR	= ./

FILES			= $(addsuffix .cpp, $(SRCS))
OBJECTS			= $(addprefix $(BUILDING_DIR),$(FILES:.cpp=.o))
DEPENDS			= $(OBJECTS:.o=.d)

#_______________________________Sources filenames______________________________#

SRCS			= 	main \
					request \
					response \
					utils \
					cgi \
					webserv \
					Server \
					#listener
					#server_socket_generator

#________________________________Building rules________________________________#

all:			init_hooks init $(NAME)

init:
		@ if test -f $(NAME);\
		then echo "$(_CYAN)[program already created]$(_NC)";\
		else echo "$(_YELLOW)[Initialize program]$(_NC)";\
		fi
		$(shell mkdir -p $(BUILDING_DIR))

$(BUILDING_DIR)%.o:$(SRCS_DIR)%.cpp
				@ echo "\t$(_YELLOW) compiling... $*.cpp$(_NC)"
				$(CXX) $(CXXFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

${NAME}:		$(OBJECTS)
				@ echo "\t$(_YELLOW)[Creating program]$(_NC)"
				$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)
				@ echo "$(_GREEN)[program created & ready]$(_NC)"

#________________________________Debugging rules_______________________________#

debug:	CXXFLAGS += $(DEBUG_LEAK)
debug:	all
rd:		fclean
rd:		debug

thread:	CXXFLAGS += $(DEBUG_THREAD)
thread:	all

verbose:	CXXFLAGS += -D DEBUG
verbose:	all
rv:			fclean
rv:			verbose

#________________________________Cleaning rules________________________________#

clean:
				@ echo "$(_RED)[cleaning up objects & dependancies files]$(_NC)"
				$(RM) $(OBJECTS) $(DEPENDS)

fclean:			clean
				@ echo "$(_RED)[cleaning up binary files]$(_NC)"
				$(RM) $(NAME)

re:				fclean all

#_____________________________________Hooks____________________________________#

init_hooks:
			@if [ -e .git/hooks/commit-msg ];\
			then :;\
			else cp .hooks/commit-msg .git/hooks/commit-msg;\
			fi

#_____________________________________Misc.____________________________________#

-include $(DEPENDS)

_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_CYAN		=	\033[96m
_NC			=	\033[0m

.PHONY:			all clean fclean re
#.SILENT:		all