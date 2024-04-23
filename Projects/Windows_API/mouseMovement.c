#include <Windows.h>
#include <stdio.h> 
#include <stdlib.h> 

int main() {
    // Set up a loop to keep the program running

//int value = rand() % (upper_bound - lower_bound + 1) + lower_bound; 

    int i = 0;

    while (1) {
        int xValue = rand() % 5;
        int yValue = rand() % 5;
        // Move the mouse slightly to the right
        i++;
       
        if(i % 11 == 0) //decided to use prime numbers. Figured they would both have multiples occur at similar rates.
        {
            mouse_event(MOUSEEVENTF_MOVE, xValue, (-1 * yValue), 0, 0);
        }
        else if (i % 13 == 0)
        {
            mouse_event(MOUSEEVENTF_MOVE, (-1 * xValue), yValue, 0, 0);
        }
        
        // Sleep for a short interval (e.g., 5 seconds)
        Sleep(50); // 5000 milliseconds = 5 seconds
    }
    
    return 0;
}