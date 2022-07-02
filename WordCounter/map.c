/* map.c
 * ----------------------------------------------------------
 *  CS350
 *  Assignment 1
 *  Question 1
 *
 *  Purpose:  Gain experience with threads and basic 
 *  synchronization.
 *
 *  YOU MAY ADD WHATEVER YOU LIKE TO THIS FILE.
 *  YOU CANNOT CHANGE THE SIGNATURE OF CountOccurrences.
 * ----------------------------------------------------------
 */
#include "data.h"

/* --------------------------------------------------------------------
 * CountOccurrences
 * --------------------------------------------------------------------
 * Takes a Library of articles containing words and a word.
 * Returns the total number of times that the word appears in the 
 * Library.
 *
 * For example, "There the thesis sits on the theatre bench.", contains
 * 2 occurences of the word "the".
 * --------------------------------------------------------------------
 */
int CountOccurrences( struct  Library * lib, char * word )
{
    int n = lib->numArticles;
    int count = 0;
    for (int k=0; k<n; k++)
    {
        int nwords = (lib->articles)[k]->numWords; 
        for (int i=0; i<nwords; i++)
        {  
            char* c = (lib->articles)[k]->words[i];
            //printf("%s \n",c);
            if (strcmp(c, word)==0)
            {
                count++;
            }
        }
    }
    return count;
}

