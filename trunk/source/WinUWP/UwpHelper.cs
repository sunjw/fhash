using System;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.System;
using Windows.UI.Xaml;

namespace FilesHashUwp
{
    class UwpHelper
    {
        public static UIElement GetRootFrame()
        {
            return Window.Current.Content;
        }

        public static Point GetCursorPointRelatedToRootFrame()
        {
            Point cursorPoint = Window.Current.CoreWindow.PointerPosition;
            Rect windowBount = Window.Current.Bounds;
            Point relativePoint = new Point(cursorPoint.X - windowBount.X, cursorPoint.Y - windowBount.Y);
            return relativePoint;
        }

        public static void CopyStringToClipboard(string text)
        {
            DataPackage dataPackage = new DataPackage { RequestedOperation = DataPackageOperation.Copy };
            dataPackage.SetText(text);
            Clipboard.SetContent(dataPackage);
        }

        public static void OpenUrl(string url)
        {
            Uri uri = new Uri(url);
            _ = Launcher.LaunchUriAsync(uri);
        }
    }
}
