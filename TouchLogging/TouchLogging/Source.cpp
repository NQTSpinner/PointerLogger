#include <Windows.h>
#include <fstream>

void PrintPointerInfo(const UINT32 PointerCount, const POINTER_INFO * PointerInfo)
{
    std::ofstream FileOut("Output.txt", std::ofstream::app);
    
    FileOut << "Number of Pointer in Frame: \t" << PointerCount << '\n';

    for (unsigned int i = 0; i < PointerCount; ++i)
    {
        FileOut << "Pointer #" << i+1 << ":\n";
        // Pointer Type
        switch (PointerInfo[i].pointerType)
        {
            case PT_TOUCH:
                FileOut << "Pointer Type: \tTouch\n";
                break;
            case PT_MOUSE:
                FileOut << "Pointer Type: \tMouse\n";
                break;
            case PT_PEN:
                FileOut << "Pointer Type: \tPen\n";
                break;
            case PT_TOUCHPAD:
                FileOut << "Pointer Type: \tTouchPad\n";
                break;
            default:
                break;
        }

        // Pointer Id:
        FileOut << "Pointer ID: \t" << PointerInfo[i].pointerId << '\n';

        // Frame Id:
        FileOut << "Frame ID: \t" << PointerInfo[i].frameId << '\n';

        // Source Device
        FileOut << "Source Device Handle: \t" << PointerInfo[i].sourceDevice << '\n';

        // Pixel Location
        FileOut << "Predicted Pixel Location:\n";
        FileOut << "\tX:\t" << PointerInfo[i].ptPixelLocation.x << '\n';
        FileOut << "\tY:\t" << PointerInfo[i].ptPixelLocation.y << '\n';

        // Pixel Location Raw
        FileOut << "Raw Pixel Location:\n";
        FileOut << "\tX:\t" << PointerInfo[i].ptPixelLocationRaw.x << '\n';
        FileOut << "\tY:\t" << PointerInfo[i].ptPixelLocationRaw.y << '\n';
        FileOut << "\n\n";
    }
    

    FileOut.close();
}

LRESULT CALLBACK
WindowCallback(HWND WindowHandle,
               UINT Message,
               WPARAM WParam,
               LPARAM LParam)
{
    LRESULT Result = 0;
    switch (Message)
    {
        case WM_POINTERENTER:
        case WM_NCPOINTERDOWN:
        case WM_NCPOINTERUPDATE:
        case WM_NCPOINTERUP:
        case WM_POINTERACTIVATE:
        case WM_POINTERLEAVE:
        case WM_POINTERCAPTURECHANGED:
        case WM_POINTERDOWN:
        case WM_POINTERUPDATE:
        case WM_POINTERUP:
        {
            UINT32 PointerId = GET_POINTERID_WPARAM(WParam);
            UINT32 PointerCount = 10;
            POINTER_INFO PointerInfo[10];
            if (GetPointerFrameInfo(PointerId, &PointerCount, PointerInfo))
            {
               PrintPointerInfo(PointerCount, PointerInfo);
            }
            else
            {
                DWORD Error = GetLastError();
                int a = 0;
            }
        }
        break;
        default:
            Result = DefWindowProc(WindowHandle, Message, WParam, LParam);
            break;
    }

    return Result;
}

int CALLBACK
WinMain (HINSTANCE Instance,
         HINSTANCE PrevInstance,
         LPSTR CmdLine,
         int ShowOption)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = nullptr;
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.lpszClassName = L"Pointer receiver";
    WindowClass.lpfnWndProc = WindowCallback;

    if (RegisterClass(&WindowClass))
    {
        HWND WindowHandle = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP,
                                           WindowClass.lpszClassName,
                                           L"Pointer Receiver",
                                           WS_OVERLAPPED | WS_MAXIMIZE | WS_VISIBLE,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           nullptr,
                                           nullptr,
                                           Instance,
                                           nullptr);

        if (WindowHandle)
        {
            ShowWindow(WindowHandle, SW_MAXIMIZE);
            MSG Message;
            while (GetMessage(&Message, nullptr, 0, 0))
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }
        else
        {
            OutputDebugStringA("Cannot create window");
        }
    }
    else
    {
        OutputDebugStringA("Cannot register Window Class");
    }
}