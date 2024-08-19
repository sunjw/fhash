using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using Microsoft.Windows.AppLifecycle;
using SunJWBase;
using Windows.UI;
using Windows.UI.ViewManagement;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace FilesHashWUI
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        private const int AppMinWidth = 400;
        private const int AppMinHeight = 420;
        private const nuint SubclassId = 18;
        private const string KeyWinMaximize = "WinMaximize";
        private const string KeyWinPosX = "WinPosX";
        private const string KeyWinPosY = "WinPosY";
        private const string KeyWinSizeWidth = "WinSizeWidth";
        private const string KeyWinSizeHeight = "WinSizeHeight";

        private UISettings m_uiSettings;
        private Page m_pageCurrent = null;

        public static MainWindow CurrentWindow { get; private set; } = null;

        public MainWindow()
        {
            InitializeComponent();

            m_uiSettings = new();

            ExtendsContentIntoTitleBar = true;
            SetTitleBar(AppTitleBar);

            Closed += MainWindow_Closed;
            m_uiSettings.ColorValuesChanged += UISettings_ColorValuesChanged;

            //LoadWindowPosSize();
        }

        private void UpdateTitleBarColor()
        {
            AppWindowTitleBar curTitleBar = AppWindow.TitleBar;
            if (curTitleBar == null)
                return;

            ElementTheme curElementTheme = MainFrame.ActualTheme;
            Color fgColor = Colors.Black;
            if (curElementTheme == ElementTheme.Dark)
                fgColor = Colors.White;

            // Windows 11
            Application.Current.Resources["WindowCaptionForeground"] = fgColor;
            AppWindow.TitleBar.ButtonForegroundColor = fgColor;

            // Windows 10?
            //Color bgColor = Colors.Transparent;
            //Color fgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlPageTextBaseHighBrush"]).Color;
            //Color inactivefgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundChromeDisabledLowBrush"]).Color;
            //Color hoverbgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlBackgroundListLowBrush"]).Color;
            //Color hoverfgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundBaseHighBrush"]).Color;
            //Color pressedbgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlBackgroundListMediumBrush"]).Color;
            //Color pressedfgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundBaseHighBrush"]).Color;

            //AppWindow.TitleBar.ButtonBackgroundColor = bgColor;
            //AppWindow.TitleBar.ButtonForegroundColor = fgColor;
            //AppWindow.TitleBar.ButtonInactiveBackgroundColor = bgColor;
            //AppWindow.TitleBar.ButtonInactiveForegroundColor = inactivefgColor;
            //AppWindow.TitleBar.ButtonHoverBackgroundColor = hoverbgColor;
            //AppWindow.TitleBar.ButtonHoverForegroundColor = hoverfgColor;
            //AppWindow.TitleBar.ButtonPressedBackgroundColor = pressedbgColor;
            //AppWindow.TitleBar.ButtonPressedForegroundColor = pressedfgColor;
        }

        private void MainFrame_Loaded(object sender, RoutedEventArgs e)
        {
            CurrentWindow = this;
            UpdateTitleBarColor();
            //MainFrame.Navigate(typeof(MainPage));
        }

        private void MainFrame_Navigated(object sender, NavigationEventArgs e)
        {
            bool isAppStart = false;
            if (m_pageCurrent == null)
            {
                isAppStart = true;
            }
            m_pageCurrent = e.Content as Page;
            //if (isAppStart && IsMainPageCurrent())
            //{
            //    AppActivationArguments args = WinUIHelper.GetCurrentActivatedEventArgs();
            //    string appActivateArgs = WinUIHelper.GetLaunchActivatedEventArgs(args);
            //    (m_pageCurrent as MainPage).OnRedirected(appActivateArgs);
            //}
        }

        private void UISettings_ColorValuesChanged(UISettings sender, object args)
        {
            DispatcherQueue.TryEnqueue(UpdateTitleBarColor);
        }

        private /*async*/ void MainWindow_Closed(object sender, WindowEventArgs args)
        {
            //SaveWindowPosSize();

            //if (IsAboutPageCurrent())
            //{
            //    AboutPage aboutPageCur = m_pageCurrent as AboutPage;
            //    if (aboutPageCur.ThreadRunning)
            //    {
            //        args.Handled = true;
            //        aboutPageCur.StopThread(() => DispatcherQueue.TryEnqueue(Close));
            //    }
            //}
        }
    }
}
