using System;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.Windows.AppLifecycle;
using SunJWBase;

namespace FilesHashWUI
{
    public static class Program
    {
        private const string AppInstanceKey = "FilesHashWUI";

        [STAThread]
        static void Main(string[] args)
        {
            WinRT.ComWrappersSupport.InitializeComWrappers();

            bool isRedirect = DecideRedirection();
            if (!isRedirect)
            {
                Application.Start((p) =>
                {
                    var context = new DispatcherQueueSynchronizationContext(
                        DispatcherQueue.GetForCurrentThread());
                    SynchronizationContext.SetSynchronizationContext(context);
                    new App();
                });
            }

            return;
        }

        private static bool DecideRedirection()
        {
            bool isRedirect = false;
            AppActivationArguments args = WinUIHelper.GetCurrentActivatedEventArgs();
            AppInstance keyInstance = AppInstance.FindOrRegisterForKey(AppInstanceKey);

            if (keyInstance.IsCurrent)
            {
                keyInstance.Activated += OnActivated;
            }
            else
            {
                isRedirect = true;
                RedirectActivationTo(keyInstance, args);
            }
            return isRedirect;
        }

        public static void RedirectActivationTo(AppInstance keyInstance, AppActivationArguments args)
        {
            var redirectSemaphore = new Semaphore(0, 1);
            Task.Run(() =>
            {
                keyInstance.RedirectActivationToAsync(args).AsTask().Wait();
                redirectSemaphore.Release();
            });
            redirectSemaphore.WaitOne();
        }

        private static void OnActivated(object sender, AppActivationArguments args)
        {
            // ExtendedActivationKind kind = args.Kind;
            App appCurrent = Application.Current as App;
            if (appCurrent != null)
            {
                appCurrent.OnRedirected(args);
            }
        }
    }
}
