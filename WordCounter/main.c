/* main.c
 * ----------------------------------------------------------
 *  CS350
 *  Assignment 1
 *  Question 1
 *
 *  Purpose:  Gain experience with threads and basic 
 *  synchronization.
 * ----------------------------------------------------------
 */

// Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Local Includes
#include "data.h"
#include "map.h"

// Defines
#define MAXWORDSIZE 25
#define MINWORDSIZE 1

// Globals
int NUMARTICLES = 10000;
int MINARTICLESIZE = 7500;
int MAXARTICLESIZE = 10000;



/* --------------------------------------------------------------------
 * GenerateWord
 * --------------------------------------------------------------------
 * Produces a string of randomly generated letters over [a-z] with
 * a maximum length of MAXWORDSIZE.
 * --------------------------------------------------------------------
 */
char * GenerateWord()
{
    // Create space for word - this decides a length randomly and makes sure its between 1 and 25.
    unsigned int length = MINWORDSIZE + rand() % (MAXWORDSIZE-MINWORDSIZE);
    char * word = ( char * )malloc( ( length + 1 ) * sizeof( char ) );

    // Fill word with random letters
    for ( unsigned int i = 0; i < length; i ++ )
    {
        unsigned int c = 97 + rand() % 26;
        word[i] = (char)c;
    }

    word[length] = 0;
    return word;
}

struct Article * GenerateArticle()
{
    // Create article
    struct Article * article = (struct Article *)malloc( sizeof( struct Article ) );

    // How many words?
    unsigned int numWords = MINARTICLESIZE + rand() % (MAXARTICLESIZE - MINARTICLESIZE);

    // Allocate space for words
    char ** words = (char **)malloc( numWords * sizeof( char * ));

    // Create words, add to article
    for ( unsigned int i = 0; i < numWords; i ++ )
    {
        words[i] = GenerateWord();
    }
    article->words = words;
    article->numWords = numWords;
    return article;
}

struct Library * GenerateLibrary()
{
    // Choose the number of articles
    unsigned int numArticles = NUMARTICLES;
    
    // Create Library
    struct Library * library = ( struct Library * )malloc( sizeof( struct Library ) );

    // Allocate space for articles
    library->articles = ( struct Article ** )malloc( numArticles * sizeof( struct Article * ) );

    // Produce articles
    for ( unsigned int i = 0; i < numArticles; i ++ )
    {
        library->articles[i] = GenerateArticle();
    }

    library->numArticles = numArticles;
    return library;
}

void FreeLibrary( struct Library * lib )
{
    for ( unsigned int i = 0; i < lib->numArticles; i ++ )
    {
        struct Article * art = lib->articles[i];
        for ( unsigned int j = 0; j < art->numWords; j ++)
        {
            char * word = art->words[j];
            free( word );
        }
        free( art->words );
        free( art );
    }

    free( lib->articles );
    free( lib );
}

pthread_mutex_t msgmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mainmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t condmain = PTHREAD_COND_INITIALIZER;

struct Arg
{
    char* c;
    struct Library* liba;
};


void* taskA (void* ptr)
{   
        pthread_mutex_lock(&msgmutex);
        ((struct Arg *)ptr)->liba = GenerateLibrary();
        pthread_mutex_unlock(&msgmutex);
        pthread_cond_signal(&cond);
}

void* taskB (void* ptr)
{
    pthread_mutex_lock(&msgmutex);
    while (((struct Arg *)ptr)->liba == NULL)
    {
        printf("Waiting for library generation!\n");
        pthread_cond_wait(&cond, &msgmutex);

    }
    struct Arg * arg = (struct Arg *)ptr;
    clock_t start;
    clock_t stop;
    start = clock();
    int count = CountOccurrences( ((struct Arg *)ptr)->liba, ((struct Arg *)ptr)->c);
    printf( "There are %d occurrences of %s\n", count, ((struct Arg *)ptr)->c);
    stop = clock();
    pthread_cond_signal(&cond);
    double time = (double)(stop - start)/ (double)CLOCKS_PER_SEC;
    printf( "Total time: %f\n", time );
    pthread_mutex_unlock(&msgmutex);
    pthread_cond_signal(&condmain);
}




int main( int argv, char ** argc )
{   
    int rc1, rc2;
    pthread_t thread1, thread2;
    // Usage
    if ( argv != 3 )
    {
        printf( "Usage: a1q1 [NUMARTICLES] [WORD]\n" );
        return 0;
    }
    else
        NUMARTICLES = atoi( argc[1] );

    // Init random number generator
    srand( 43 );

    // Allocate space for the library of articles
    //struct Library* lib = NULL;
    struct Arg* arg = malloc(sizeof(struct Arg));
    arg->c = argc[2];
    arg->liba = NULL;

    rc1 = pthread_create(&thread1, NULL, taskA, (void*)arg);
    rc2 = pthread_create(&thread2, NULL, taskB, (void*)arg);
    
    pthread_cond_wait(&condmain, &mainmutex);
    printf("No of articles %d", arg->liba->numArticles);
    free(arg);

    //pthread_exit(NULL);
    return 0;
}
