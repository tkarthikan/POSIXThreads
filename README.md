This project is to get familiarized with threads and synchronization using
a common multi-platform library called pthreads (POSIX Threads).

WordCounter project:

In this, we have a large library filled with articles. We want to know how many times a specific word occurs in the entirety of that library. For example, perhaps we want to know how many
times the word “the” occurs on Wikipedia.

The GenerateLibrary method in main.c generates a Library of Articles containing words according to the
parameters provided. After creating the Library, it calls CountOccurrences (found in map.c), to
determine how many times the specified word (parameter) are found in the Library. The count
is output to the screen along with the runtime of the function.

We implemented CountOccurrences using threads, and locks (pthread_mutext_t) and condition
variables (pthread_cond_t) to create a barrier and protect shared variables. Due to the simple nature of the task, it is possible that your multi-threaded solution is slower than a single-threaded one.