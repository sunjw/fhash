using System;
using System.Collections.Generic;
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

            // Init native
            m_uiBridgeDelegate = new UIBridgeDelegate();
            m_uiBridgeDelegate.PreparingCalcHandler += UIBridgeDelegate_PreparingCalcHandler;
            m_uiBridgeDelegate.RemovePreparingCalcHandler += UIBridgeDelegate_RemovePreparingCalcHandler;
            m_uiBridgeDelegate.CalcStopHandler += UIBridgeDelegate_CalcStopHandler;
            m_uiBridgeDelegate.CalcFinishHandler += UIBridgeDelegate_CalcFinishHandler;
            m_uiBridgeDelegate.ShowFileNameHandler += UIBridgeDelegate_ShowFileNameHandler;
            m_uiBridgeDelegate.ShowFileMetaHandler += UIBridgeDelegate_ShowFileMetaHandler;
            m_uiBridgeDelegate.ShowFileHashHandler += UIBridgeDelegate_ShowFileHashHandler;
            m_uiBridgeDelegate.ShowFileErrHandler += UIBridgeDelegate_ShowFileErrHandler;
            m_uiBridgeDelegate.UpdateProgWholeHandler += UIBridgeDelegate_UpdateProgWholeHandler;

            m_hashMgmt = new HashMgmt(m_uiBridgeDelegate);
            m_hashMgmt.Init();

            // Init resource and titlebar
            m_resourceLoaderMain = ResourceLoader.GetForCurrentView();
            m_coreAppViewTitleBar = CoreApplication.GetCurrentView().TitleBar;
            m_appViewTitleBar = ApplicationView.GetForCurrentView().TitleBar;

            // Init size
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
                    ButtonClear.IsEnabled = true;
                    ButtonVerify.IsEnabled = true;
                    break;
                case MainPageControlStat.MainPageCalcIng:
                    HidePopupAbout();

                    m_calcStartTime = UwpHelper.GetCurrentMilliSec();
                    m_hashMgmt.SetStop(false);

                    ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenStop");
                    ButtonClear.IsEnabled = false;
                    ButtonVerify.IsEnabled = false;
                    break;
                case MainPageControlStat.MainPageVerify:
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

        private void StartHashCalc(List<string> filePaths)
        {
            if (!IsAbleToCalcFiles())
            {
                return;
            }

            if (m_mainPageStat == MainPageControlStat.MainPageNone)
            {
                ClearTextMain();
            }

            m_hashMgmt.AddFiles(filePaths.ToArray());
            ProgressBarMain.Value = 0;

            SetPageControlStat(MainPageControlStat.MainPageCalcIng);
            m_hashMgmt.StartHashThread();
        }

        private void StopHashCalc(bool needExit)
        {
            if (m_mainPageStat == MainPageControlStat.MainPageCalcIng)
            {
                m_hashMgmt.SetStop(true);

                if (needExit)
                {
                    SetPageControlStat(MainPageControlStat.MainPageWaitingExit);
                }
            }
        }

        private void CalculateFinished()
        {
            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);
            ProgressBarMain.Value = m_uiBridgeDelegate.GetProgMax();
        }

        private void CalculateStopped()
        {
            AppendInlineToTextMain(UwpHelper.GenRunFromString("\r\n"));

            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);
            ProgressBarMain.Value = 0;
        }

        private void HandleCommandLineArgs()
        {
            if (!IsAbleToCalcFiles())
            {
                return;
            }

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

            if (cmdArgFiles.Count == 0)
            {
                return;
            }

            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                StartHashCalc(cmdArgFiles);
            }));
        }

        public void CommandLineActivated()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                HandleCommandLineArgs();
            }));
        }

        private void MainPage_CloseRequested(object sender, SystemNavigationCloseRequestedPreviewEventArgs e)
        {
            if (IsCalculating())
            {
                e.Handled = true;
                StopHashCalc(true);
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

            HandleCommandLineArgs();
        }

        private void ButtonOpen_Click(object sender, RoutedEventArgs e)
        {
            if (m_mainPageStat == MainPageControlStat.MainPageCalcIng)
            {
                StopHashCalc(false);
            }
            else
            {
            }
        }

        private void ButtonAbout_Click(object sender, RoutedEventArgs e)
        {
            ShowPopupAbout();
        }

        private async void ButtonVerify_Click(object sender, RoutedEventArgs e)
        {
            m_textBoxFindHash.Text = "";
            ContentDialogResult result = await m_dialogFind.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                string strHash = m_textBoxFindHash.Text;
            }
        }

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {
            SetPageControlStat(MainPageControlStat.MainPageNone);
        }

        private void CheckBoxUppercase_Click(object sender, RoutedEventArgs e)
        {
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
            if (IsAbleToCalcFiles())
            {
                e.AcceptedOperation = DataPackageOperation.Copy;
            }
            else
            {
                e.AcceptedOperation = DataPackageOperation.None;
            }
        }

        private async void GridRoot_Drop(object sender, DragEventArgs e)
        {
            if (!IsAbleToCalcFiles())
            {
                return;
            }

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
            foreach (IStorageItem storageItem in storageItems)
            {
                string path = storageItem.Path;
                if (!string.IsNullOrEmpty(path))
                {
                    strDropFilePaths.Add(path);
                }
            }

            if (strDropFilePaths.Count == 0)
            {
                return;
            }

            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                StartHashCalc(strDropFilePaths);
            }));
        }

        private void UIBridgeDelegate_PreparingCalcHandler()
        {
        }

        private void UIBridgeDelegate_RemovePreparingCalcHandler()
        {
        }

        private void UIBridgeDelegate_CalcStopHandler()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                CalculateStopped();
            }));
        }

        private void UIBridgeDelegate_CalcFinishHandler()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                CalculateFinished();
            }));
        }

        private void UIBridgeDelegate_ShowFileNameHandler(ResultDataNet resultData)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                List<Inline> inlines = new List<Inline>();
                string strAppend = m_resourceLoaderMain.GetString("ResultFileName");
                strAppend += " ";
                strAppend += resultData.Path;
                inlines.Add(UwpHelper.GenRunFromString(strAppend));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                AppendInlinesToTextMain(inlines);
            }));
        }

        private void UIBridgeDelegate_ShowFileMetaHandler(ResultDataNet resultData)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                string strShortSize = UwpHelper.ConvertSizeToShortSizeStr(resultData.Size);
                List<Inline> inlines = new List<Inline>();
                string strSize = m_resourceLoaderMain.GetString("ResultFileSize");
                strSize += " ";
                strSize += resultData.Size;
                strSize += " ";
                strSize += m_resourceLoaderMain.GetString("ResultByte");
                if (!string.IsNullOrEmpty(strShortSize))
                {
                    strSize += " (";
                    strSize += strShortSize;
                    strSize += ")";
                }
                string strModifiedTime = m_resourceLoaderMain.GetString("ResultModifiedTime");
                strModifiedTime += " ";
                strModifiedTime += resultData.ModifiedDate;
                inlines.Add(UwpHelper.GenRunFromString(strSize));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                inlines.Add(UwpHelper.GenRunFromString(strModifiedTime));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                AppendInlinesToTextMain(inlines);
            }));
        }

        private void UIBridgeDelegate_ShowFileHashHandler(ResultDataNet resultData, bool uppercase)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                string strFileMD5, strFileSHA1, strFileSHA256, strFileSHA512;

                if (uppercase)
                {
                    strFileMD5 = resultData.MD5.ToUpper();
                    strFileSHA1 = resultData.SHA1.ToUpper();
                    strFileSHA256 = resultData.SHA256.ToUpper();
                    strFileSHA512 = resultData.SHA512.ToUpper();
                }
                else
                {
                    strFileMD5 = resultData.MD5.ToLower();
                    strFileSHA1 = resultData.SHA1.ToLower();
                    strFileSHA256 = resultData.SHA256.ToLower();
                    strFileSHA512 = resultData.SHA512.ToLower();
                }

                List<Inline> inlines = new List<Inline>();
                inlines.Add(UwpHelper.GenRunFromString("MD5: "));
                inlines.Add(GenHyperlinkFromStringForTextMain(strFileMD5));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                inlines.Add(UwpHelper.GenRunFromString("SHA1: "));
                inlines.Add(GenHyperlinkFromStringForTextMain(strFileSHA1));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                inlines.Add(UwpHelper.GenRunFromString("SHA256: "));
                inlines.Add(GenHyperlinkFromStringForTextMain(strFileSHA256));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
                inlines.Add(UwpHelper.GenRunFromString("SHA512: "));
                inlines.Add(GenHyperlinkFromStringForTextMain(strFileSHA512));
                inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
                AppendInlinesToTextMain(inlines);
            }));
        }

        private void UIBridgeDelegate_ShowFileErrHandler(ResultDataNet resultData)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                List<Inline> inlines = new List<Inline>();
                string strAppend = resultData.Error;
                inlines.Add(UwpHelper.GenRunFromString(strAppend));
                inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
                AppendInlinesToTextMain(inlines);
            }));
        }

        private void UIBridgeDelegate_UpdateProgWholeHandler(int value)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                double newValue = value;
                double oldValue = ProgressBarMain.Value;
                if (oldValue == newValue)
                {
                    return;
                }
                ProgressBarMain.Value = newValue;
            }));
        }
    }
}
