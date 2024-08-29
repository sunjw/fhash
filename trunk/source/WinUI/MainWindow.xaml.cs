using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using Microsoft.Windows.ApplicationModel.Resources;
using Microsoft.Windows.AppLifecycle;
using SunJWBase;
using Windows.ApplicationModel.DataTransfer;
using Windows.Graphics;
using Windows.UI;
using Windows.UI.ViewManagement;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.Shell;
using Windows.Win32.UI.WindowsAndMessaging;
using WinRT.Interop;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace FilesHashWUI
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        public delegate void RedirectedHandler(string strAppActiveArgs);
        public delegate void OnCloseStopHandler();
        public delegate void OnDropFilesHandler(List<string> strDropFilesPath);

        public delegate bool IsAbleToCalcHandler();
        public delegate bool IsCalculatingHandler();

        private const int AppPrefWidth = 680;
        private const int AppPrefHeight = 460;
        private const int AppMinWidth = 540;
        private const int AppMinHeight = 440;

        private const nuint SubclassId = 18;

        private const string KeyWinMaximize = "WinMaximize";
        private const string KeyWinPosX = "WinPosX";
        private const string KeyWinPosY = "WinPosY";
        private const string KeyWinSizeWidth = "WinSizeWidth";
        private const string KeyWinSizeHeight = "WinSizeHeight";

        private ResourceLoader m_resourceLoaderMain = WinUIHelper.GetCurrentResourceLoader();
        private UISettings m_uiSettings = null;
        private Page m_pageCurrent = null;
        private SUBCLASSPROC m_subclassProc = null;

        private bool m_windowInited = false;

        private AdvTaskbarHelper m_advTaskbarHelper = null;

        public static MainWindow CurrentWindow { get; private set; } = null;

        public bool IsAppPackaged { get; private set; } = false;
        public IntPtr HWNDHandle { get; private set; } = 0;
        public double Scale { get; private set; } = 1.0;

        public HashMgmtClr HashMgmt { get; private set; } = null;
        public UIBridgeDelegates UIBridgeHandlers { get; private set; } = new();
        public IsAbleToCalcHandler IsAbleToCalc = null;
        public IsCalculatingHandler IsCalculating = null;

        public event RedirectedHandler RedirectedEventHandler = null;
        public event OnCloseStopHandler OnCloseStopEventHandler = null;
        public event OnDropFilesHandler OnDropFilesEventHandler = null;

        public MainWindow()
        {
            InitializeComponent();

            IsAppPackaged = Win32Helper.IsAppPackaged();
            HWNDHandle = WindowNative.GetWindowHandle(this);
            Scale = Win32Helper.GetScaleFactor(HWNDHandle);

            m_uiSettings = new();
            m_advTaskbarHelper = new(HWNDHandle);

            HashMgmt = new(UIBridgeHandlers);
            HashMgmt.Init();

            InitTitleBarAndTaskBar();

            Closed += MainWindow_Closed;
            m_uiSettings.ColorValuesChanged += UISettings_ColorValuesChanged;

            LoadWindowPosSize();
            InitWindowSubclass();
        }

        private bool IsPageCurrent(Type pageType)
        {
            return m_pageCurrent?.GetType() == pageType;
        }

        public System.Drawing.Point GetCursorRelativePoint()
        {
            System.Drawing.Point pointRelative = new(0, 0);
            System.Drawing.Point pointPointer = Win32Helper.GetPointerPoint();

            if (AppWindow != null)
            {
                PointInt32 pointAppWindow = AppWindow.Position;
                pointRelative.X = pointPointer.X - pointAppWindow.X;
                pointRelative.Y = pointPointer.Y - pointAppWindow.Y;
            }

            return pointRelative;
        }

        private void InitTitleBarAndTaskBar()
        {
            string titleAppName = m_resourceLoaderMain.GetString("TitleAppName");
#if DEBUG
            titleAppName += " Dev";
#endif
            // Taskbar
            AppWindow.SetIcon("Assets/fhashWUI.ico");
            Title = titleAppName;

            // Titlebar
            TextBlockAppName.Text = titleAppName;
            ExtendsContentIntoTitleBar = true;
            SetTitleBar(AppTitleBar);
        }

        private void UpdateTitleBarColor()
        {
            if (AppWindow.TitleBar == null)
                return;

            ElementTheme curElementTheme = FrameMain.ActualTheme;
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

        private void InitWindowSubclass()
        {
            m_subclassProc = new(WndSubProc);
            PInvoke.SetWindowSubclass(new HWND(HWNDHandle), m_subclassProc, SubclassId, 0);
        }

        private LRESULT WndSubProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam, nuint uIdSubclass, nuint dwRefData)
        {
            if (uMsg == PInvoke.WM_GETMINMAXINFO)
            {
                var minMaxInfo = Marshal.PtrToStructure<MINMAXINFO>(lParam);
                minMaxInfo.ptMinTrackSize.X = Win32Helper.GetScaledPixel(AppMinWidth, Scale);
                minMaxInfo.ptMinTrackSize.Y = Win32Helper.GetScaledPixel(AppMinHeight, Scale);
                Marshal.StructureToPtr(minMaxInfo, lParam, false);
            }

            return PInvoke.DefSubclassProc(hWnd, uMsg, wParam, lParam);
        }

        private void LoadWindowPosSize()
        {
            bool windowMaximize = false;
            PointInt32 windowPos = new(-1, -1);
            SizeInt32 windowSize = new(Win32Helper.GetScaledPixel(AppPrefWidth, Scale), Win32Helper.GetScaledPixel(AppPrefHeight, Scale));

            object objWinMaximize = WinUIHelper.LoadLocalSettings(KeyWinMaximize);
            windowMaximize = (bool)(objWinMaximize ?? false);
            object objWinPosX = WinUIHelper.LoadLocalSettings(KeyWinPosX);
            object objWinPosY = WinUIHelper.LoadLocalSettings(KeyWinPosY);
            if (objWinPosX != null && objWinPosY != null)
            {
                windowPos.X = Win32Helper.GetScaledPixel((int)objWinPosX, Scale);
                windowPos.Y = Win32Helper.GetScaledPixel((int)objWinPosY, Scale);
            }
            object objWinSizeWidth = WinUIHelper.LoadLocalSettings(KeyWinSizeWidth);
            object objWinSizeHeight = WinUIHelper.LoadLocalSettings(KeyWinSizeHeight);
            if (objWinSizeWidth != null && objWinSizeHeight != null)
            {
                windowSize.Width = Win32Helper.GetScaledPixel((int)objWinSizeWidth, Scale);
                windowSize.Height = Win32Helper.GetScaledPixel((int)objWinSizeHeight, Scale);
            }

            AppWindow.Resize(windowSize);
            if (windowPos.X != -1 && windowPos.Y != -1)
            {
                AppWindow.Move(windowPos);
            }
            if (windowMaximize)
            {
                Win32Helper.MaximizeWindow(HWNDHandle);
            }
        }

        private void SaveWindowPosSize()
        {
            bool windowMaximize = Win32Helper.IsWindowMaximize(HWNDHandle);
            PointInt32 windowPos = AppWindow.Position;
            windowPos.X = Win32Helper.GetUnscaledPixel(windowPos.X, Scale);
            windowPos.Y = Win32Helper.GetUnscaledPixel(windowPos.Y, Scale);
            SizeInt32 windowSize = AppWindow.Size;
            windowSize.Width = Win32Helper.GetUnscaledPixel(windowSize.Width, Scale);
            windowSize.Height = Win32Helper.GetUnscaledPixel(windowSize.Height, Scale);

            WinUIHelper.SaveLocalSettings(KeyWinMaximize, windowMaximize);
            if (!windowMaximize)
            {
                // not override normal position and size
                WinUIHelper.SaveLocalSettings(KeyWinPosX, windowPos.X);
                WinUIHelper.SaveLocalSettings(KeyWinPosY, windowPos.Y);
                WinUIHelper.SaveLocalSettings(KeyWinSizeWidth, windowSize.Width);
                WinUIHelper.SaveLocalSettings(KeyWinSizeHeight, windowSize.Height);
            }
        }

        public void SetTaskbarProgress(ulong prog)
        {
            if (prog > 99)
                prog = 99;
            m_advTaskbarHelper.SetTaskbarProg(prog);
        }

        public void OnRedirected(AppActivationArguments args)
        {
            string strAppActiveArgs = WinUIHelper.GetLaunchActivatedEventArgs(args);
            DispatcherQueue.TryEnqueue(() => RedirectedEventHandler?.Invoke(strAppActiveArgs));
        }

        private void FrameMain_Loaded(object sender, RoutedEventArgs e)
        {
            CurrentWindow = this;

            UpdateTitleBarColor();

            FrameMain.Navigate(typeof(MainPage));
        }

        private void FrameMain_Navigated(object sender, NavigationEventArgs e)
        {
            m_pageCurrent = e.Content as Page;

            if (!m_windowInited)
                m_windowInited = true;
        }

        private void UISettings_ColorValuesChanged(UISettings sender, object args)
        {
            DispatcherQueue.TryEnqueue(UpdateTitleBarColor);
        }

        private void MainWindow_Closed(object sender, WindowEventArgs args)
        {
            if (IsCalculating?.Invoke() ?? false)
            {
                args.Handled = true;
                OnCloseStopEventHandler?.Invoke();
                return;
            }

            SaveWindowPosSize();
        }

        private void GridRoot_DragOver(object sender, DragEventArgs e)
        {
            if (IsAbleToCalc?.Invoke() ?? false)
                e.AcceptedOperation = DataPackageOperation.Copy;
            else
                e.AcceptedOperation = DataPackageOperation.None;
        }

        private async void GridRoot_Drop(object sender, DragEventArgs e)
        {
            List<string> strDropFilesPath = await WinUIHelper.GetDropFilesPath(e);
            DispatcherQueue.TryEnqueue(() => OnDropFilesEventHandler?.Invoke(strDropFilesPath));
        }
    }
}
