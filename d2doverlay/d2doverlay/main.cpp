#include "D2DOverlay.h"

//
// Provides the entry point to the application.
//
int WINAPI WinMain(
    HINSTANCE /* hInstance */,
    HINSTANCE /* hPrevInstance */,
    LPSTR /* lpCmdLine */,
    int /* nCmdShow */
)
{
    // Ignore the return value because we want to continue running even in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            D2DOverlay app(1.0, NULL);

            if (SUCCEEDED(app.Initialize(false)))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}
