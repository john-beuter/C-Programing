#include <Windows.h>

int main() {
    // Set up a loop to keep the program running
    while (1) {
        // Move the mouse slightly to the right
        mouse_event(MOUSEEVENTF_MOVE, 1, 0, 0, 0);
        
        // Sleep for a short interval (e.g., 5 seconds)
        Sleep(5); // 5000 milliseconds = 5 seconds
    }
    
    return 0;
}