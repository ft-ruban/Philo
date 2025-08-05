override SRCSDIR	:= srcs/
override SRCS		= $(addprefix $(SRCSDIR), $(SRC))

override PARSERDIR := parsing/
override EXECDIR := exec/

SRC += $(addprefix $(MAINDIR), $(addsuffix .c, $(MAIN)))

override MAIN := \
	main \
	debug \
	utils \

SRC += $(addprefix $(PARSERDIR), $(addsuffix .c, $(PARSERSRC)))

override PARSERSRC := \
	parsing_utils \
	parsing \


SRC += $(addprefix $(EXECDIR), $(addsuffix .c, $(EXECSRC)))

override EXECSRC := \
	philo \
	philo_utils