using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.DataTransfer;
using Windows.ApplicationModel.Resources;
using Windows.Foundation;
using Windows.Storage;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Core.Preview;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Documents;
using Windows.UI.Xaml.Media;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace FilesHashUwp
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private enum MainPageControlStat
        {
            MainPageNone = 0, // clear stat
            MainPageCalcIng,  // calculating
            MainPageCalcFinish, // calculating finished/stopped
            MainPageVerify, // verfing
            MainPageWaitingExit, // waiting thread stop and exit
        };

        private CoreApplicationViewTitleBar m_coreAppViewTitleBar;
        private ApplicationViewTitleBar m_appViewTitleBar;

        private ContentDialog m_dialogExitConfirm;
        private ContentDialog m_dialogFind;

        private TextBox m_textBoxFindHash;
        private MenuFlyout m_menuFlyoutTextMain;

        private ResourceLoader m_resourceLoaderMain;
        private UISettings m_uiSettings;
        private long m_tokenThemeChanged;
        private Thickness m_imageAppIconMargin;
        private Paragraph m_paragraphMain;
        private Hyperlink m_hyperlinkClicked;
        private int m_testCount = 0;

        private UIBridgeDelegate m_uiBridgeDelegate;
        private HashMgmt m_hashMgmt;

        private MainPageControlStat m_mainPageStat;
        private long m_calcStartTime = 0;
        private long m_calcEndTime = 0;

        public MainPage()
        {
            InitializeComponent();

            SystemNavigationManagerPreview.GetForCurrentView().CloseRequested += MainPage_CloseRequested;
            Window.Current.SizeChanged += WindowSizeChanged;

            m_uiBridgeDelegate = new UIBridgeDelegate();
            m_hashMgmt = new HashMgmt(m_uiBridgeDelegate);
            m_hashMgmt.Init();

            m_resourceLoaderMain = ResourceLoader.GetForCurrentView();
            m_coreAppViewTitleBar = CoreApplication.GetCurrentView().TitleBar;
            m_appViewTitleBar = ApplicationView.GetForCurrentView().TitleBar;

            ApplicationView.PreferredLaunchViewSize = new Size(640, 400);
            ApplicationView.PreferredLaunchWindowingMode = ApplicationViewWindowingMode.PreferredLaunchViewSize;

            InitLayout();
        }

        public static MainPage GetCurrentMainPage()
        {
            Frame frame = (Frame)UwpHelper.GetRootFrame();

            if (frame != null)
            {
                MainPage mainPage = (MainPage)frame.Content;
                return mainPage;
            }

            return null;
        }

        private void ChangeThemeLight()
        {
            ChangeTheme(ElementTheme.Light);
        }

        private void ChangeTheme(ElementTheme theme)
        {
            FrameworkElement root = (FrameworkElement)UwpHelper.GetRootFrame();
            root.RequestedTheme = theme;
            UpdateControlColor();
        }

        private void UpdateControlColor()
        {
            UpdateTitleBarColor();
        }

        private void UpdateTitleBarColor()
        {
            if (m_appViewTitleBar == null)
            {
                return;
            }

            Color bgColor = Colors.Transparent;
            Color fgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlPageTextBaseHighBrush"]).Color;
            Color inactivefgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundChromeDisabledLowBrush"]).Color;
            Color hoverbgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlBackgroundListLowBrush"]).Color;
            Color hoverfgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundBaseHighBrush"]).Color;
            Color pressedbgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlBackgroundListMediumBrush"]).Color;
            Color pressedfgColor = ((SolidColorBrush)Application.Current.Resources["SystemControlForegroundBaseHighBrush"]).Color;
            m_appViewTitleBar.ButtonBackgroundColor = bgColor;
            m_appViewTitleBar.ButtonForegroundColor = fgColor;
            m_appViewTitleBar.ButtonInactiveBackgroundColor = bgColor;
            m_appViewTitleBar.ButtonInactiveForegroundColor = inactivefgColor;
            m_appViewTitleBar.ButtonHoverBackgroundColor = hoverbgColor;
            m_appViewTitleBar.ButtonHoverForegroundColor = hoverfgColor;
            m_appViewTitleBar.ButtonPressedBackgroundColor = pressedbgColor;
            m_appViewTitleBar.ButtonPressedForegroundColor = pressedfgColor;
        }

        private void ShowPopupAbout()
        {
            UpdatePopupAboutSize();

            if (!PopupAbout.IsOpen)
            {
                GridMain.Visibility = Visibility.Collapsed;
                ImageAppIcon.Margin = new Thickness(48, 0, 0, 0);
                PopupAbout.IsOpen = true;
            }
        }

        public void HidePopupAbout()
        {
            if (PopupAbout.IsOpen)
            {
                PopupAbout.IsOpen = false;
                ImageAppIcon.Margin = m_imageAppIconMargin;
                GridMain.Visibility = Visibility.Visible;
            }
        }

        private void UpdatePopupAboutSize()
        {
            if (PopupAboutContent == null)
            {
                FindName("PopupAboutContent");
            }

            Rect windowBounds = Window.Current.Bounds;
            PopupAboutContent.Width = windowBounds.Width;
            PopupAboutContent.Height = windowBounds.Height;
        }

        private void InitLayout()
        {
            InitCustomTitleBar();
            InitDialogExitConfirm();
            InitDialogFind();
            InitMenuFlyoutTextMain();
        }

        private void InitCustomTitleBar()
        {
            m_coreAppViewTitleBar.ExtendViewIntoTitleBar = true;
            Window.Current.SetTitleBar(GridTitleBarCustom);
            m_imageAppIconMargin = ImageAppIcon.Margin;

            UpdateTitleBarColor();
        }

        private void InitDialogExitConfirm()
        {
            m_dialogExitConfirm = new ContentDialog
            {
                Title = "Exit",
                Content = "Really go?",
                PrimaryButtonText = "Bye!",
                CloseButtonText = "No",
                DefaultButton = ContentDialogButton.Primary
            };
        }

        private void InitDialogFind()
        {
            m_textBoxFindHash = new TextBox()
            {
                Height = (double)Application.Current.Resources["TextControlThemeMinHeight"],
                Width = 400,
                PlaceholderText = "Some hash"
            };
            m_dialogFind = new ContentDialog()
            {
                // Title = "Find",
                // MaxWidth = ActualWidth,
                PrimaryButtonText = "OK",
                SecondaryButtonText = "Cancel",
                Content = m_textBoxFindHash,
                DefaultButton = ContentDialogButton.Primary
            };
        }

        private void InitMenuFlyoutTextMain()
        {
            m_menuFlyoutTextMain = new MenuFlyout();
            MenuFlyoutItem menuItemCopy = new MenuFlyoutItem();
            menuItemCopy.Text = m_resourceLoaderMain.GetString("MenuItemCopy");
            menuItemCopy.Click += MenuItemCopy_Click;
            MenuFlyoutItem menuItemGoogle = new MenuFlyoutItem();
            menuItemGoogle.Text = m_resourceLoaderMain.GetString("MenuItemGoogle");
            menuItemGoogle.Click += MenuItemGoogle_Click;
            m_menuFlyoutTextMain.Items.Add(menuItemCopy);
            m_menuFlyoutTextMain.Items.Add(menuItemGoogle);
        }

        private void ScrollTextMainToBottom()
        {
            UwpHelper.ScrollViewerToBottom(ScrollViewerMain);
        }

        private Hyperlink GenHyperlinkFromStringForTextMain(string strContent)
        {
            return UwpHelper.GenHyperlinkFromString(strContent, TextMainHyperlink_Click);
        }

        private List<Inline> GenInlinesFromPaths(List<string> strPaths)
        {
            List<Inline> inlines = new List<Inline>();
            foreach (string path in strPaths)
            {
                inlines.Add(UwpHelper.GenRunFromString(path));
                inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
            }
            return inlines;
        }

        private void ClearTextMain()
        {
            m_paragraphMain.Inlines.Clear();
        }

        private void AppendInlinesToTextMain(List<Inline> inlines, bool scrollBottom = true)
        {
            if (inlines != null)
            {
                foreach (Inline inline in inlines)
                {
                    m_paragraphMain.Inlines.Add(inline);
                }
            }
            if (scrollBottom)
            {
                ScrollTextMainToBottom();
            }
        }


        private void AppendInlineToTextMain(Inline inline)
        {
            List<Inline> inlines = new List<Inline>();
            inlines.Add(inline);
            AppendInlinesToTextMain(inlines);
        }

        private void ClearAndShowInlinesInTextMain(List<Inline> inlines)
        {
            m_paragraphMain.Inlines.Clear();
            if (inlines != null)
            {
                foreach (Inline inline in inlines)
                {
                    m_paragraphMain.Inlines.Add(inline);
                }
            }
            ScrollTextMainToBottom();
        }

        private void SetPageControlStat(MainPageControlStat newStat)
        {
            switch (newStat)
            {
                case MainPageControlStat.MainPageNone:
                case MainPageControlStat.MainPageCalcFinish:
                    // MainPageControlStat.MainPageNone
                    if (newStat == MainPageControlStat.MainPageNone)
                    {
                        m_hashMgmt.Clear();

                        ProgressBarMain.Value = 0;

                        Span spanInit = new Span();
                        string strPageInit = m_resourceLoaderMain.GetString("MainPageInitInfo");
                        spanInit.Inlines.Add(UwpHelper.GenRunFromString(strPageInit));
                        spanInit.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                        ClearTextMain();
                        AppendInlineToTextMain(spanInit);
                    }
                    // Passthrough to MainPageControlStat.MainPageCalcFinish
                    m_calcEndTime = UwpHelper.GetCurrentMilliSec();
                    ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenOpen");
                    ButtonVerify.IsEnabled = true;
                    break;
                case MainPageControlStat.MainPageCalcIng:
                    m_calcStartTime = UwpHelper.GetCurrentMilliSec();
                    m_hashMgmt.SetStop(false);

                    ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenStop");
                    ButtonVerify.IsEnabled = false;
                    break;
                case MainPageControlStat.MainPageVerify:
                    break;
                case MainPageControlStat.MainPageWaitingExit:
                    break;
            }

            MainPageControlStat oldStat = m_mainPageStat;
            m_mainPageStat = newStat;

            if (oldStat == MainPageControlStat.MainPageWaitingExit &&
                m_mainPageStat == MainPageControlStat.MainPageCalcFinish)
            {
                // Wait to close
                Application.Current.Exit();
            }
        }

        private bool IsAbleToCalcFiles()
        {
            return !IsCalculating();
        }

        private bool IsCalculating()
        {
            return (m_mainPageStat == MainPageControlStat.MainPageCalcIng ||
                m_mainPageStat == MainPageControlStat.MainPageWaitingExit);
        }

        private void ShowCmdArgs(string strInit = "")
        {
            List<string> cmdArgFiles = new List<string>();
            App curApp = (App)Application.Current;
            if (!string.IsNullOrEmpty(curApp.CmdArgs))
            {
                string[] cmdArgsArray = curApp.CmdArgs.Split("\"");
                for (int i = 0; i < cmdArgsArray.Length; i++)
                {
                    string cmdArg = cmdArgsArray[i];
                    cmdArg = cmdArg.Trim();
                    if (string.IsNullOrEmpty(cmdArg))
                    {
                        continue;
                    }
                    cmdArgFiles.Add(cmdArg);
                }
            }

            List<Inline> inlines = new List<Inline>();
            if (cmdArgFiles.Count > 0)
            {
                inlines = GenInlinesFromPaths(cmdArgFiles);
            }
            else
            {
                inlines.Add(UwpHelper.GenRunFromString(strInit));
            }
            ClearAndShowInlinesInTextMain(inlines);
        }

        private void DoTest1()
        {
            Span span1 = new Span();

            span1.Inlines.Add(UwpHelper.GenRunFromString("文件名: C:\\Users\\Sun Junwen\\OneDrive\\Apps\\fHash\\fHash64-2.3.0-win64.zip\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("文件大小: 383692 字节(374.70 KB)\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("修改日期: 2022-03-28 15:17\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("MD5: "));
            span1.Inlines.Add(GenHyperlinkFromStringForTextMain("c0232b5bc37e70eebad95fbfc6499d92"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("SHA1: "));
            span1.Inlines.Add(GenHyperlinkFromStringForTextMain("14017068424f5ca356b9440cb025eb0f848e08d9"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("SHA256: "));
            span1.Inlines.Add(GenHyperlinkFromStringForTextMain("ce25ecf0650ebfa1f8f2d8ebc557ad3db0cd75e80bda647c59314668e0281a35"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("SHA512: "));
            span1.Inlines.Add(GenHyperlinkFromStringForTextMain("aeda1930fc0ae1feda19b68170b78074f4a408ec50080256110dd0d9eda005abaa7167ae6d62ca302f6995f60f3d038af6c21667ea922e1206bb3670bc1c5e71"));
            span1.Inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));

            List<Inline> inlines = new List<Inline>();
            inlines.Add(span1);
            ClearAndShowInlinesInTextMain(inlines);
            ProgressBarMain.Value = 30;
        }

        private void DoTest2()
        {
            Span span2 = new Span();

            span2.Inlines.Add(UwpHelper.GenRunFromString("Name: /Users/sunjw/Library/CloudStorage/OneDrive-个人/Apps/fHash/fHash-2.3.0-macOS.dmg\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("File Size: 656793 Byte(s)(656.79 KB)\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("Modified Date: 2022-03-28 14:58\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("MD5: "));
            span2.Inlines.Add(GenHyperlinkFromStringForTextMain("1C880E1191F884EB225F9CFA36CDC355"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("SHA1: "));
            span2.Inlines.Add(GenHyperlinkFromStringForTextMain("4528265C32082A3FB115C05CB29FA36E96A942E5"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("SHA256: "));
            span2.Inlines.Add(GenHyperlinkFromStringForTextMain("B704FD5E9E6818F1AC8E7EBECF97F0A56E1FDB8DB9AB9E01FE0F6B574A586FEF"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("SHA512: "));
            span2.Inlines.Add(GenHyperlinkFromStringForTextMain("15F634DF65D0A41E943A9024CA69FF5C31E4B05446180FE5900B7CB0068CFF9DF2820EFD3F8AFCB48924C3FE45D0113FDC8E81F5E4E6CDD60830BE4581F8B11C"));
            span2.Inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));

            m_paragraphMain.Inlines.Add(span2);
            ScrollTextMainToBottom();
            ProgressBarMain.Value = 60;
        }

        private void DoTest3()
        {
            Span span3 = new Span();

            span3.Inlines.Add(UwpHelper.GenRunFromString("文件名: C:\\Users\\Sun Junwen\\OneDrive\\新建文件夹\\jstool\\JSMinNPP.dll\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("文件大小: 432640 字节(422.50 KB)\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("修改日期: 2022-05-24 12:58\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("版本: 1.2205.0.0\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("MD5: "));
            span3.Inlines.Add(GenHyperlinkFromStringForTextMain("68373918248f972c784cf6fd921670dd"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("SHA1: "));
            span3.Inlines.Add(GenHyperlinkFromStringForTextMain("9a0acfb02d983bc1feaf0364cd04884dd554c47d"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("SHA256: "));
            span3.Inlines.Add(GenHyperlinkFromStringForTextMain("f828b4973c72721b78aafeb9caa82362465ca95ebb8a296f39ef3c45aa8f0d10"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("SHA512: "));
            span3.Inlines.Add(GenHyperlinkFromStringForTextMain("e2f8f248b0dbada7799f02eeacccd53ca4d5ef4296504e6826e72cb6aa1259ad61d15c222f9ea1638d06990328a433e3dc871761fc38aef22dfa7bc786145213"));
            span3.Inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));

            m_paragraphMain.Inlines.Add(span3);
            ScrollTextMainToBottom();
            ProgressBarMain.Value = 100;
        }

        private void DoTest4()
        {
            ClearAndShowInlinesInTextMain(null);
            ProgressBarMain.Value = 0;
        }

        public void CommandLineActivated()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                HidePopupAbout();
                ShowCmdArgs();
            }));
        }

        private async void MainPage_CloseRequested(object sender, SystemNavigationCloseRequestedPreviewEventArgs e)
        {
            e.Handled = true;
            ContentDialogResult result = await m_dialogExitConfirm.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                Application.Current.Exit();
            }
        }

        private void ColorValuesChanged(UISettings sender, object e)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                UpdateControlColor();
            }));
        }

        private void RequestedThemeChanged(DependencyObject sender, DependencyProperty dp)
        {
            if (RequestedThemeProperty == dp)
            {
                _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
                {
                    UpdateControlColor();
                }));
            }
        }

        private void WindowSizeChanged(object sender, WindowSizeChangedEventArgs e)
        {
            UpdatePopupAboutSize();
        }

        private void GridRoot_Loaded(object sender, RoutedEventArgs e)
        {
            // Init UI
            Frame rootFrame = (Frame)UwpHelper.GetRootFrame();

            // Theme changed callback
            m_uiSettings = new UISettings();
            m_uiSettings.ColorValuesChanged += ColorValuesChanged;
            // RequestedThemeProperty seems not work at all...
            m_tokenThemeChanged = rootFrame.RegisterPropertyChangedCallback(
                RequestedThemeProperty, RequestedThemeChanged);

            // Prepare RichTextMain
            RichTextMain.TextWrapping = TextWrapping.NoWrap;
            m_paragraphMain = new Paragraph();
            m_paragraphMain.FontFamily = new FontFamily("Consolas");
            m_paragraphMain.LineHeight = 18;
            m_paragraphMain.LineStackingStrategy = LineStackingStrategy.BlockLineHeight;
            RichTextMain.Blocks.Add(m_paragraphMain);

            // Prepare controls
            ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenOpen");

            // Init stat
            SetPageControlStat(MainPageControlStat.MainPageNone);
        }

        private void ButtonOpen_Click(object sender, RoutedEventArgs e)
        {
            switch (m_testCount)
            {
                case 0:
                    DoTest1();
                    break;
                case 1:
                    DoTest2();
                    break;
                case 2:
                    DoTest3();
                    break;
                case 3:
                    DoTest4();
                    break;
            }

            m_testCount++;
            m_testCount = m_testCount % 4;
        }

        private void ButtonAbout_Click(object sender, RoutedEventArgs e)
        {
            ShowPopupAbout();
        }

        private async void ButtonFind_Click(object sender, RoutedEventArgs e)
        {
            m_textBoxFindHash.Text = "";
            ContentDialogResult result = await m_dialogFind.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                string strHash = m_textBoxFindHash.Text;
                List<Inline> inlines = new List<Inline>();
                inlines.Add(UwpHelper.GenRunFromString(strHash));
                inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
                ClearAndShowInlinesInTextMain(inlines);
            }
        }

        private void TextMainHyperlink_Click(Hyperlink sender, HyperlinkClickEventArgs args)
        {
            if (sender == null)
            {
                return;
            }
            m_hyperlinkClicked = sender;
            m_menuFlyoutTextMain.ShowAt(UwpHelper.GetRootFrame(), UwpHelper.GetCursorPointRelatedToRootFrame());
        }

        private void MenuItemCopy_Click(object sender, RoutedEventArgs e)
        {
            if (m_hyperlinkClicked == null)
            {
                return;
            }
            string strHash = UwpHelper.GetTextFromHyperlink(m_hyperlinkClicked);
            UwpHelper.CopyStringToClipboard(strHash);
        }

        private void MenuItemGoogle_Click(object sender, RoutedEventArgs e)
        {
            if (m_hyperlinkClicked == null)
            {
                return;
            }

            string strHash = UwpHelper.GetTextFromHyperlink(m_hyperlinkClicked);
            string strGoogleUrl = string.Format("https://www.google.com/search?q={0}&ie=utf-8&oe=utf-8", strHash);
            UwpHelper.OpenUrl(strGoogleUrl);
        }

        private void GridRoot_DragOver(object sender, DragEventArgs e)
        {
            e.AcceptedOperation = DataPackageOperation.Copy;
        }

        private async void GridRoot_Drop(object sender, DragEventArgs e)
        {
            if (!e.DataView.Contains(StandardDataFormats.StorageItems))
            {
                return;
            }

            IReadOnlyList<IStorageItem> storageItems = await e.DataView.GetStorageItemsAsync();
            if (storageItems == null)
            {
                return;
            }

            List<string> strDropFilePaths = new List<string>();
            foreach (StorageFile storageFile in storageItems)
            {
                string path = storageFile.Path;
                if (!string.IsNullOrEmpty(path))
                {
                    strDropFilePaths.Add(path);
                }
            }

            HidePopupAbout();
            List<Inline> inlines = GenInlinesFromPaths(strDropFilePaths);
            ClearAndShowInlinesInTextMain(inlines);
        }

    }
}
