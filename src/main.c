#include <windows.h>
#include <stdio.h>

#define WINDOW_MAX_WIDTH 808
#define WINDOW_MIN_WIDTH 0
#define WINDOW_MAX_HEIGHT 630
#define WINDOW_MIN_HEIGHT 0

#define SQUARE_SIZE 100
#define MOVE_STEP 5
#define GRAVITY 1
#define TIMER_INTERVAL 30
#define JUMP_INITIAL_VELOCITY -20

// Global variables to store the square's position and velocity
int squareX = SQUARE_SIZE;
int squareY = WINDOW_MAX_HEIGHT - SQUARE_SIZE; // Start at the bottom
int velocityY = 0;
int isJumping = 0;
int moveLeft = 0;
int moveRight = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hRedBrush;
    static HBRUSH hBackgroundBrush;

    switch (uMsg)
    {
    case WM_CREATE:
        // Create brushes when the window is created
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
        hBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255)); // White background
        // Set a timer to update the square's position regularly
        SetTimer(hwnd, 1, TIMER_INTERVAL, NULL);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Get the client area of the window
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // Clear the client area with the background color
        FillRect(hdc, &clientRect, hBackgroundBrush);

        // Define the position and size of the red square
        RECT redSquare = {
            squareX,
            squareY,
            squareX + SQUARE_SIZE,
            squareY + SQUARE_SIZE};

        // Fill the rectangle with the red brush
        FillRect(hdc, &redSquare, hRedBrush);

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_TIMER:
        // Apply gravity to the vertical velocity if the square is jumping
        if (isJumping) {
            velocityY += GRAVITY;

            // Update the square's position
            squareY += velocityY;

            // Ensure the square stays within the window boundaries
            if (squareY + SQUARE_SIZE > WINDOW_MAX_HEIGHT)
            {
                squareY = WINDOW_MAX_HEIGHT - SQUARE_SIZE;
                velocityY = 0; // Reset velocity when hitting the ground
                isJumping = 0; // Stop jumping
            }
        }

        // Update the square's horizontal position based on the movement flags
        if (moveLeft && squareX > WINDOW_MIN_WIDTH)
        {
            squareX -= MOVE_STEP;
        }
        if (moveRight && squareX < (WINDOW_MAX_WIDTH - SQUARE_SIZE))
        {
            squareX += MOVE_STEP;
        }

        // Redraw the window to update the position of the square
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;

    case WM_KEYDOWN:
        // Handle arrow key inputs to move the square
        switch (wParam)
        {
        case VK_LEFT:
            moveLeft = 1;
            break;
        case VK_RIGHT:
            moveRight = 1;
            break;
        case VK_UP:
            if (!isJumping)
            {
                velocityY = JUMP_INITIAL_VELOCITY; // Start the jump with an initial velocity
                isJumping = 1; // Indicate that the square is jumping
            }
            break;
        }
        break;

    case WM_KEYUP:
        // Handle arrow key releases to stop moving the square
        switch (wParam)
        {
        case VK_LEFT:
            moveLeft = 0;
            break;
        case VK_RIGHT:
            moveRight = 0;
            break;
        }
        break;

    case WM_DESTROY:
        // Clean up resources
        DeleteObject(hRedBrush);
        DeleteObject(hBackgroundBrush);
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Entry point for the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance; // Suppress unused parameter warning
    (void)lpCmdLine;     // Suppress unused parameter warning
    const char CLASS_NAME[] = "My game engine";

    // Register the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                                                  // Optional window styles.
        CLASS_NAME,                                         // Window class
        "My game engine",                                   // Window text
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_SIZEBOX, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_MAX_WIDTH, WINDOW_MAX_HEIGHT,
        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
