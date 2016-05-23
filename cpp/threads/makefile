CC = g++

#BASE_FLAGS = -m32 -wAll
BASE_FLAGS = -m32

# INCLUDE BASE DIRECTORY AND BOOST DIRECTORY FOR HEADERS
LDFLAGS = -I/usr/local/Cellar/boost/1.59.0/include -I/opt/local/include

# INCLUDE BASE DIRECTORY AND BOOST DIRECTORY FOR LIB FILES
LLIBFLAGS = -L/usr/local/Cellar/boost/1.59.0/lib

# SPECIFIY LINK OPTIONS
LINKFLAGS = -l boost_thread-mt -lboost_system

# FINAL FLAGS -- TO BE USED THROUGHOUT
#FLAGS = $(BASE_FLAGS) $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)
FLAGS = $(LLIBFLAGS) $(LDFLAGS) $(LINKFLAGS)

# NOTE FOR BOOST -- YOU ONLY NEED TO INCLUDE THE PATH BECAUSE IT ONLY INSTALLS HEADER FILES
all: boost_threads_test.cpp boost_thread_set_prio.cpp
	$(CC) $(FLAGS) -o boost_threads_test.out boost_threads_test.cpp
	$(CC) $(FLAGS) -o boost_thread_set_prio.out boost_thread_set_prio.cpp

#all: boost_threads_test boost_pthread_sched
#boost_threads_test: boost_threads_test.cpp
#	$(CC) $(FLAGS) -o boost_threads_test.out boost_threads_test.cpp
#boost_pthread_sched: boost_pthread_sched.cpp
#	$(CC) $(FLAGS) -o boost_pthread_sched.out boost_pthread_sched.cpp
