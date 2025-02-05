using System;
using System.Drawing;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;

namespace SunJWBase
{
    public class Win32Helper
    {
        public static unsafe bool IsAppPackaged()
        {
            uint bufferLength = 0;
            WIN32_ERROR result = PInvoke.GetCurrentPackageId(ref bufferLength, null);
            bool isPackaged = true;
            if (result == WIN32_ERROR.APPMODEL_ERROR_NO_PACKAGE)
            {
                isPackaged = false;
            }
            return isPackaged;
        }

        public static double GetScaleFactor(IntPtr hWnd)
        {
            double dpi = PInvoke.GetDpiForWindow(new HWND(hWnd));
            return dpi / 96.0;
        }

        public static bool IsWindowMaximize(IntPtr hWnd)
        {
            WINDOWPLACEMENT windowPlacement = default;
            if (PInvoke.GetWindowPlacement(new HWND(hWnd), ref windowPlacement))
            {
                if (windowPlacement.showCmd == SHOW_WINDOW_CMD.SW_MAXIMIZE)
                    return true;
            }
            return false;
        }

        public static void MaximizeWindow(IntPtr hWnd)
        {
            PInvoke.ShowWindow(new HWND(hWnd), SHOW_WINDOW_CMD.SW_MAXIMIZE);
        }

        public static int GetScaledPixel(int pixel, double scale)
        {
            return (int)(pixel * scale);
        }

        public static int GetUnscaledPixel(int pixel, double scale)
        {
            return (int)(pixel / scale);
        }

        public static Point GetPointerPoint()
        {
            Point pointCursor = new();
            PInvoke.GetCursorPos(out pointCursor);
            return pointCursor;
        }

        public static bool SetForegroundWindow(IntPtr hWnd)
        {
            return PInvoke.SetForegroundWindow(new HWND(hWnd));
        }
    }
}
