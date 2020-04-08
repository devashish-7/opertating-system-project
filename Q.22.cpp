#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define debugi(x) printf("DEBUG : %d\n", x)
#define FORN(i, n) for(__typeof(i) i = 0; i < n; i++)

typedef long long dd;

int random_f = 0;
void showInp(dd *virtual_mem_pages, dd no_of_pages, dd no_of_frames);
void drawL();

void popPages(dd *virtual_mem_pages, dd no_of_pages)
{
    for (int i = 0; i < no_of_pages; i++)
    {
        virtual_mem_pages[i] = (rand() % 10);
    }
}

int findPageH(dd *frame_list, dd no_of_frames, dd tar)
{
    for(int i = 0; i < no_of_frames; i++)
    {
        if(frame_list[i] == tar)
            return 1;
    }
    return 0;
}

void showInp(dd *virtual_mem_pages, dd no_of_pages, dd no_of_frames)
{
    printf("You have entered the following data");
    drawL();
    printf("Number of pages\t\t\t: %ddd\n", no_of_pages);
    if(random_f)
	{
	printf("Random ");
         }
    else 
	{
	printf("Custom ");
	printf("Virtual Memory Pages \t: ");
         }      
    for(int i = 0; i < no_of_pages; i++)
    {
        printf("%ddd ", virtual_mem_pages[i]);
    printf("Number of frames\t\t: %ddd", no_of_frames);
     }
    drawL();
}
void drawL()
{
    printf("\n");
    for(int i = 0; i < 30; i++)
    {
        printf("-");
    printf("\n");
     }
}

int main(int arg, const char **argc)
{
    srand(time(NULL));
    dd no_of_pages;
    dd *virtual_mem_pages;
    if(arg == 1)
    {
        printf("Enter the number of pages in virtual memory ");
        scanf("%ddd", &no_of_pages);
        virtual_mem_pages = (dd *)malloc(sizeof(dd) * no_of_pages);
        popPages(virtual_mem_pages, no_of_pages);
        random_f = 1;
    }
    else
    {
        no_of_pages = arg - 1;
        virtual_mem_pages = (dd *)malloc(sizeof(dd) * no_of_pages);
        int index = 0;
        for(int i = 1; i < argc; i++)
            virtual_mem_pages[index++] = atoll(argc[i]);
    }
    dd no_of_frames;
    printf("Enter the number of frames ");
    scanf("%ddd", &no_of_frames);
    showInp(virtual_mem_pages, no_of_pages, no_of_frames);
    dd *frame_list = (dd *)malloc(no_of_frames * sizeof(dd));
    memset(frame_list, -1, no_of_frames * sizeof(dd));

    dd count_page_h, count_page_fault;
    count_page_h = count_page_fault = 0;
    dd last = -1;
    for(int i = 0; i < no_of_pages; i++)
    {
        if(findPageH(frame_list, no_of_frames, virtual_mem_pages[i]))
        {
            count_page_h++;
        }
        else
        {
            frame_list[++last % no_of_frames] = virtual_mem_pages[i];
            count_page_fault++;
        }
    }

    printf("Number of Page hits are %ddd\n", count_page_h);
    printf("Number of Page faults are %ddd\n", count_page_fault);
    return 0;
}

