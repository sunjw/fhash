using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Documents;
using Microsoft.Windows.ApplicationModel.Resources;
using Microsoft.Windows.AppLifecycle;
using Windows.ApplicationModel.Activation;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Storage;
using Windows.System;

namespace SunJWBase
{
    public class WinUIHelper
    {
        private static ResourceLoader m_resourceLoaderMain = new();

        public static AppActivationArguments GetCurrentActivatedEventArgs()
        {
            return AppInstance.GetCurrent().GetActivatedEventArgs();
        }

        public static string GetLaunchActivatedEventArgs(AppActivationArguments appActivationArgs)
        {
            string launchArgs = null;
            if (appActivationArgs?.Kind == ExtendedActivationKind.Launch)
            {
                ILaunchActivatedEventArgs launchActivatedEventArgs = appActivationArgs.Data as ILaunchActivatedEventArgs;
                launchArgs = launchActivatedEventArgs?.Arguments;
            }
            return launchArgs;
        }

        public static ResourceLoader GetCurrentResourceLoader()
        {
            return m_resourceLoaderMain;
        }

        public static void ScrollViewerScrollTo(ScrollViewer scrollViewer, double offsetX, double offsetY)
        {
            if (offsetX < 0)
            {
               offsetX = 0;
            }
            if (offsetY < 0)
            {
               offsetY = 0;
            }
            if (offsetX > scrollViewer.ScrollableWidth)
            {
               offsetX = scrollViewer.ScrollableWidth;
            }
            if (offsetY > scrollViewer.ScrollableHeight)
            {
               offsetY = scrollViewer.ScrollableHeight;
            }
            scrollViewer.ChangeView(offsetX, offsetY, null);
        }


        public static void ScrollViewerToBottom(ScrollViewer scrollViewer)
        {
            scrollViewer.Measure(scrollViewer.RenderSize); // must Measure now
            scrollViewer.ChangeView(null, scrollViewer.ScrollableHeight, null);
        }

        public static void CopyStringToClipboard(string text)
        {
            DataPackage dataPackage = new() { RequestedOperation = DataPackageOperation.Copy };
            dataPackage.SetText(text);
            Clipboard.SetContent(dataPackage);
            Clipboard.Flush(); // very important
        }

        public static void OpenUrl(string url)
        {
            Uri uri = new(url);
            _ = Launcher.LaunchUriAsync(uri);
        }

        public static Run GenRunFromString(string strContent)
        {
            Run run = new();
            run.Text = strContent;
            return run;
        }

        public static Hyperlink GenHyperlinkFromString(string strContent, TypedEventHandler<Hyperlink, HyperlinkClickEventArgs> clickHandler)
        {
            Hyperlink hyperlink = new();
            hyperlink.Inlines.Add(GenRunFromString(strContent));
            hyperlink.UnderlineStyle = UnderlineStyle.None;
            if (clickHandler != null)
            {
                hyperlink.Click += clickHandler;
            }
            return hyperlink;
        }

        public static string GetTextFromHyperlink(Hyperlink hyperlink)
        {
            if (hyperlink.Inlines.Count == 0)
            {
                return null;
            }
            Run run = (Run)hyperlink.Inlines[0];
            return run.Text;
        }

        public static async Task<List<string>> GetDropFilesPath(DragEventArgs e)
        {
            List<string> strDropFilesPath = [];

            if (e.DataView.Contains(StandardDataFormats.StorageItems))
            {
                IReadOnlyList<IStorageItem> storageItems = await e.DataView.GetStorageItemsAsync();
                if (storageItems != null)
                {
                    foreach (IStorageItem storageItem in storageItems)
                    {
                        string path = storageItem.Path;
                        if (!string.IsNullOrEmpty(path))
                        {
                            strDropFilesPath.Add(path);
                        }
                    }
                }
            }

            return strDropFilesPath;
        }

        public static long GetCurrentMilliSec()
        {
            return DateTimeOffset.Now.ToUnixTimeMilliseconds();
        }

        public static string ConvertSizeToShortSizeStr(ulong size, bool conv1KSmaller = false)
        {
            string strSize = "";
            ulong sizek = 1024;

            if (size > sizek)
            {
                double k_size = ((double)size) / sizek;
                if (k_size > sizek)
                {
                    double m_size = k_size / sizek;
                    if (m_size > sizek)
                    {
                        double g_size = m_size / sizek;
                        strSize = string.Format("{0:0.00} GB", g_size);
                    }
                    else
                    {
                        strSize = string.Format("{0:0.00} MB", m_size);
                    }
                }
                else
                {
                    strSize = string.Format("{0:0.00} KB", k_size);
                }
            }
            else if (conv1KSmaller)
            {
                strSize = string.Format("{0:0.00} B", (double)size);
            }

            return strSize;
        }

        public static void SaveLocalSettings(string key, object val)
        {
            ApplicationDataContainer localSettings = ApplicationData.Current.LocalSettings;
            localSettings.Values[key] = val;
        }

        public static object LoadLocalSettings(string key)
        {
            ApplicationDataContainer localSettings = ApplicationData.Current.LocalSettings;
            if (localSettings.Values.ContainsKey(key))
            {
                return localSettings.Values[key];
            }
            return null;
        }
    }
}
