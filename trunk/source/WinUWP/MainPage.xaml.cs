using System;
using System.Collections.Generic;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.DataTransfer;
using Windows.ApplicationModel.Resources;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Pickers;
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

        private const string KeyAlreadyRun = "AlreadyRun";
        private const string KeyUppercase = "Uppercase";

        private ResourceLoader m_resourceLoaderMain;
        private UISettings m_uiSettings;
        private long m_tokenThemeChanged;

        private CoreApplicationViewTitleBar m_coreAppViewTitleBar;
        private ApplicationViewTitleBar m_appViewTitleBar;

        private Thickness m_marginForAboutBackButton = new Thickness(48, 0, 0, 0);
        private Thickness m_marginImageAppIcon;
        private Thickness m_marginTextBlockAppName;

        private ContentDialog m_dialogFind;
        private TextBox m_textBoxFindHash;

        private Paragraph m_paragraphMain;
        private Paragraph m_paragraphResult;
        private Paragraph m_paragraphFind;
        private List<Hyperlink> m_hyperlinksMain;
        private List<Hyperlink> m_hyperlinksResult = new List<Hyperlink>();
        private List<Hyperlink> m_hyperlinksFind = new List<Hyperlink>();
        private MenuFlyout m_menuFlyoutTextMain;
        private Hyperlink m_hyperlinkClicked = null;
        private Run m_runPrepare = null;

        private UIBridgeDelegate m_uiBridgeDelegate;
        private HashMgmt m_hashMgmt;

        private MainPageControlStat m_mainPageStat;
        private bool m_uppercaseChecked = false;
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
            Size sizePreferred = new Size(660, 420);
            ApplicationView.GetForCurrentView().SetPreferredMinSize(sizePreferred);
            object objAlreadyRun = UwpHelper.LoadLocalSettings(KeyAlreadyRun);
            if (objAlreadyRun == null)
            {
                // First run
                ApplicationView.PreferredLaunchViewSize = sizePreferred;
                ApplicationView.PreferredLaunchWindowingMode = ApplicationViewWindowingMode.PreferredLaunchViewSize;
                UwpHelper.SaveLocalSettings(KeyAlreadyRun, true);
            }
            else
            {
                ApplicationView.PreferredLaunchWindowingMode = ApplicationViewWindowingMode.Auto;
            }

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
                // ImageAppIcon.Margin = m_marginForAboutBackButton;
                ImageAppIcon.Visibility = Visibility.Collapsed;
                TextBlockAppName.Margin = m_marginForAboutBackButton;
                PopupAbout.IsOpen = true;
            }
        }

        public void HidePopupAbout()
        {
            if (PopupAbout.IsOpen)
            {
                PopupAbout.IsOpen = false;
                TextBlockAppName.Margin = m_marginTextBlockAppName;
                // ImageAppIcon.Margin = m_marginImageAppIcon;
                ImageAppIcon.Visibility = Visibility.Visible;
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
            InitDialogFind();
            InitMenuFlyoutTextMain();
        }

        private void InitCustomTitleBar()
        {
            m_coreAppViewTitleBar.ExtendViewIntoTitleBar = true;
            Window.Current.SetTitleBar(GridTitleBarCustom);
            m_marginImageAppIcon = ImageAppIcon.Margin;
            m_marginTextBlockAppName = TextBlockAppName.Margin;

            string titleAppName = m_resourceLoaderMain.GetString("TitleAppName");
#if DEBUG
            titleAppName += " Dev";
#endif
            TextBlockAppName.Text = titleAppName;

            UpdateTitleBarColor();
        }

        private void InitDialogFind()
        {
            m_textBoxFindHash = new TextBox()
            {
                Height = (double)Application.Current.Resources["TextControlThemeMinHeight"],
                Width = 400,
                PlaceholderText = m_resourceLoaderMain.GetString("HashValue")
            };
            m_dialogFind = new ContentDialog()
            {
                Title = m_resourceLoaderMain.GetString("FindDialogTitle"),
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
            MenuFlyoutItem menuItemVirusTotal = new MenuFlyoutItem();
            menuItemVirusTotal.Text = m_resourceLoaderMain.GetString("MenuItemVirusTotal");
            menuItemVirusTotal.Click += MenuItemVirusTotal_Click;

            m_menuFlyoutTextMain.Items.Add(menuItemCopy);
            m_menuFlyoutTextMain.Items.Add(new MenuFlyoutSeparator());
            m_menuFlyoutTextMain.Items.Add(menuItemGoogle);
            m_menuFlyoutTextMain.Items.Add(menuItemVirusTotal);
        }

        private void ScrollTextMainToBottom()
        {
            UwpHelper.ScrollViewerToBottom(ScrollViewerMain);
        }

        private Paragraph CreateParagraphForTextMain()
        {
            Paragraph paragraph = new Paragraph();
            paragraph.FontFamily = new FontFamily("Consolas");
            paragraph.LineHeight = 18;
            paragraph.LineStackingStrategy = LineStackingStrategy.BlockLineHeight;
            return paragraph;
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
                        m_hyperlinksMain.Clear();
                        m_hashMgmt.Clear();

                        ProgressBarMain.Value = 0;
                        TextBlockSpeed.Text = "";

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
                    CheckBoxUppercase.IsEnabled = true;
                    break;
                case MainPageControlStat.MainPageCalcIng:
                    HidePopupAbout();

                    m_calcStartTime = UwpHelper.GetCurrentMilliSec();
                    m_hashMgmt.SetStop(false);

                    TextBlockSpeed.Text = "";
                    ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenStop");
                    ButtonClear.IsEnabled = false;
                    ButtonVerify.IsEnabled = false;
                    CheckBoxUppercase.IsEnabled = false;
                    break;
                case MainPageControlStat.MainPageVerify:
                    ButtonVerify.IsEnabled = false;
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

        private void UpdateUppercaseStat(bool saveLocalSetting = true)
        {
            bool? uppercaseIsChecked = CheckBoxUppercase.IsChecked;
            if (uppercaseIsChecked.HasValue && uppercaseIsChecked.Value)
            {
                m_uppercaseChecked = true;
            }
            else
            {
                m_uppercaseChecked = false;
            }
            if (saveLocalSetting)
            {
                UwpHelper.SaveLocalSettings(KeyUppercase, m_uppercaseChecked);
            }
        }

        private void UpdateResultUppercase()
        {
            // Refresh stat
            UpdateUppercaseStat();

            // Refresh result & find
            List<List<Hyperlink>> hyperlinkLists = new List<List<Hyperlink>>();
            hyperlinkLists.Add(m_hyperlinksResult);
            hyperlinkLists.Add(m_hyperlinksFind);
            foreach (List<Hyperlink> hyperlinkListItr in hyperlinkLists)
            {
                foreach (Hyperlink hyperlink in hyperlinkListItr)
                {
                    if (hyperlink.Inlines.Count == 0)
                    {
                        continue;
                    }
                    string hyperLinkText = UwpHelper.GetTextFromHyperlink(hyperlink);
                    if (m_uppercaseChecked)
                    {
                        hyperLinkText = hyperLinkText.ToUpper();
                    }
                    else
                    {
                        hyperLinkText = hyperLinkText.ToLower();
                    }
                    Run runInHyperlink = (Run)hyperlink.Inlines[0];
                    runInHyperlink.Text = hyperLinkText;
                }
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
            if (m_mainPageStat == MainPageControlStat.MainPageVerify)
            {
                ClearFindResult();
            }

            m_hashMgmt.AddFiles(filePaths.ToArray());

            UpdateUppercaseStat();
            m_hashMgmt.SetUppercase(m_uppercaseChecked);
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

            long calcDurationTime = m_calcEndTime - m_calcStartTime;
            if (calcDurationTime > 10)
            {
                // speed is Bytes/ms
                double calcSpeed = ((double)m_hashMgmt.GetTotalSize()) / calcDurationTime;
                calcSpeed = calcSpeed * 1000; // Bytes/s
                ulong ulCalcSpeed = (ulong)calcSpeed;
                string strSpeed = "";
                if (ulCalcSpeed > 0)
                {
                    strSpeed = UwpHelper.ConvertSizeToShortSizeStr(ulCalcSpeed, true);
                    if (!string.IsNullOrEmpty(strSpeed))
                    {
                        strSpeed += "/s";
                    }
                }
                TextBlockSpeed.Text = strSpeed;
            }
            else
            {
                TextBlockSpeed.Text = "";
            }
        }

        private void CalculateStopped()
        {
            AppendInlineToTextMain(UwpHelper.GenRunFromString("\r\n"));

            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);
            ProgressBarMain.Value = 0;
        }

        private void AppendFileNameToTextMain(ResultDataNet resultData)
        {
            List<Inline> inlines = new List<Inline>();
            string strAppend = m_resourceLoaderMain.GetString("ResultFileName");
            strAppend += " ";
            strAppend += resultData.Path;
            inlines.Add(UwpHelper.GenRunFromString(strAppend));
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileMetaToTextMain(ResultDataNet resultData)
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
            if (!string.IsNullOrEmpty(resultData.Version))
            {
                string strVersion = m_resourceLoaderMain.GetString("ResultFileVersion");
                strVersion += " ";
                strVersion += resultData.Version;
                inlines.Add(UwpHelper.GenRunFromString(strVersion));
                inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            }
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileHashToTextMain(ResultDataNet resultData, bool uppercase)
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
            Hyperlink hyperlinkMD5 = GenHyperlinkFromStringForTextMain(strFileMD5);
            m_hyperlinksMain.Add(hyperlinkMD5);
            inlines.Add(hyperlinkMD5);
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            inlines.Add(UwpHelper.GenRunFromString("SHA1: "));
            Hyperlink hyperlinkSHA1 = GenHyperlinkFromStringForTextMain(strFileSHA1);
            m_hyperlinksMain.Add(hyperlinkSHA1);
            inlines.Add(hyperlinkSHA1);
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            inlines.Add(UwpHelper.GenRunFromString("SHA256: "));
            Hyperlink hyperlinkSHA256 = GenHyperlinkFromStringForTextMain(strFileSHA256);
            m_hyperlinksMain.Add(hyperlinkSHA256);
            inlines.Add(hyperlinkSHA256);
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            inlines.Add(UwpHelper.GenRunFromString("SHA512: "));
            Hyperlink hyperlinkSHA512 = GenHyperlinkFromStringForTextMain(strFileSHA512);
            m_hyperlinksMain.Add(hyperlinkSHA512);
            inlines.Add(hyperlinkSHA512);
            inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileErrToTextMain(ResultDataNet resultData)
        {
            List<Inline> inlines = new List<Inline>();
            string strAppend = resultData.Error;
            inlines.Add(UwpHelper.GenRunFromString(strAppend));
            inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileResultToTextMain(ResultDataNet resultData, bool uppercase)
        {
            if (resultData.EnumState == ResultStateNet.ResultNone)
            {
                return;
            }

            if (resultData.EnumState == ResultStateNet.ResultAll ||
                resultData.EnumState == ResultStateNet.ResultMeta ||
                resultData.EnumState == ResultStateNet.ResultError ||
                resultData.EnumState == ResultStateNet.ResultPath)
            {
                AppendFileNameToTextMain(resultData);
            }

            if (resultData.EnumState == ResultStateNet.ResultAll ||
                resultData.EnumState == ResultStateNet.ResultMeta)
            {
                AppendFileMetaToTextMain(resultData);
            }

            if (resultData.EnumState == ResultStateNet.ResultAll)
            {
                AppendFileHashToTextMain(resultData, uppercase);
            }

            if (resultData.EnumState == ResultStateNet.ResultError)
            {
                AppendFileErrToTextMain(resultData);
            }

            if (resultData.EnumState != ResultStateNet.ResultAll &&
                resultData.EnumState != ResultStateNet.ResultError)
            {
                AppendInlineToTextMain(UwpHelper.GenRunFromString("\r\n"));
            }
        }

        private void ShowFindResult(string strHashToFind, ResultDataNet[] resultDataNetArray)
        {
            // Switch m_paragraphMain
            RichTextMain.Blocks.Clear();
            m_paragraphMain = m_paragraphFind;
            RichTextMain.Blocks.Add(m_paragraphMain);
            m_hyperlinksMain = m_hyperlinksFind;

            // Show result
            List<Inline> inlines = new List<Inline>();
            string strFindResult = m_resourceLoaderMain.GetString("FindResultTitle");
            inlines.Add(UwpHelper.GenRunFromString(strFindResult));
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            string strHashValue = m_resourceLoaderMain.GetString("HashValue");
            strHashValue += ": ";
            inlines.Add(UwpHelper.GenRunFromString(strHashValue));
            inlines.Add(UwpHelper.GenRunFromString(strHashToFind));
            inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            string strFindResultBegin = m_resourceLoaderMain.GetString("FindResultBegin");
            inlines.Add(UwpHelper.GenRunFromString(strFindResultBegin));
            inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));
            AppendInlinesToTextMain(inlines);

            if (resultDataNetArray == null || resultDataNetArray.Length == 0)
            {
                // No match
                List<Inline> inlinesResult = new List<Inline>();
                string strFindNoResult = m_resourceLoaderMain.GetString("FindNoResult");
                inlinesResult.Add(UwpHelper.GenRunFromString(strFindNoResult));
                inlinesResult.Add(UwpHelper.GenRunFromString("\r\n"));
                AppendInlinesToTextMain(inlinesResult);
            }
            else
            {
                // Found some
                foreach (ResultDataNet resultData in resultDataNetArray)
                {
                    AppendFileResultToTextMain(resultData, m_uppercaseChecked);
                }
            }

            SetPageControlStat(MainPageControlStat.MainPageVerify);
        }

        private void ClearFindResult()
        {
            // Switch m_paragraphMain
            RichTextMain.Blocks.Clear();
            m_paragraphMain = m_paragraphResult;
            RichTextMain.Blocks.Add(m_paragraphMain);
            ScrollTextMainToBottom();
            m_hyperlinksMain = m_hyperlinksResult;

            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);

            // Clear find result
            m_paragraphFind.Inlines.Clear();
            m_hyperlinksFind.Clear();
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
            m_paragraphResult = CreateParagraphForTextMain();
            m_paragraphFind = CreateParagraphForTextMain();
            m_paragraphMain = m_paragraphResult;
            RichTextMain.Blocks.Add(m_paragraphMain);
            m_hyperlinksMain = m_hyperlinksResult;

            // Prepare controls
            ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenOpen");
            TextBlockSpeed.Text = "";

            object objUppercase = UwpHelper.LoadLocalSettings(KeyUppercase);
            bool uppercaseSetting = false;
            if (objUppercase != null)
            {
                uppercaseSetting = (bool)objUppercase;
            }
            CheckBoxUppercase.IsChecked = uppercaseSetting;
            UpdateUppercaseStat(false);

            // Init stat
            SetPageControlStat(MainPageControlStat.MainPageNone);

            HandleCommandLineArgs();
        }

        private async void ButtonOpen_Click(object sender, RoutedEventArgs e)
        {
            if (m_mainPageStat == MainPageControlStat.MainPageCalcIng)
            {
                StopHashCalc(false);
            }
            else
            {
                FileOpenPicker picker = new FileOpenPicker();
                picker.FileTypeFilter.Add("*");

                IReadOnlyList<StorageFile> pickFiles = await picker.PickMultipleFilesAsync();
                if (pickFiles != null)
                {
                    // Application now has read/write access to the picked file
                    List<string> strPickFilePaths = new List<string>();
                    foreach (IStorageItem storageItem in pickFiles)
                    {
                        string path = storageItem.Path;
                        if (!string.IsNullOrEmpty(path))
                        {
                            strPickFilePaths.Add(path);
                        }
                    }

                    if (strPickFilePaths.Count == 0)
                    {
                        return;
                    }

                    _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
                    {
                        StartHashCalc(strPickFilePaths);
                    }));
                }
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
                string strHashToFind = m_textBoxFindHash.Text;
                ResultDataNet[] resultDataNetArray = m_hashMgmt.FindResult(strHashToFind);
                ShowFindResult(strHashToFind, resultDataNetArray);
            }
        }

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {
            if (m_mainPageStat == MainPageControlStat.MainPageVerify)
            {
                ClearFindResult();
            }
            else
            {
                SetPageControlStat(MainPageControlStat.MainPageNone);
            }
        }

        private void CheckBoxUppercase_Checked(object sender, RoutedEventArgs e)
        {
            UpdateResultUppercase();
        }

        private void CheckBoxUppercase_Unchecked(object sender, RoutedEventArgs e)
        {
            UpdateResultUppercase();
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
            string strUrl = string.Format("https://www.google.com/search?q={0}&ie=utf-8&oe=utf-8", strHash);
            UwpHelper.OpenUrl(strUrl);
        }

        private void MenuItemVirusTotal_Click(object sender, RoutedEventArgs e)
        {
            if (m_hyperlinkClicked == null)
            {
                return;
            }

            string strHash = UwpHelper.GetTextFromHyperlink(m_hyperlinkClicked);
            string strUrl = string.Format("https://www.virustotal.com/#/search/{0}", strHash);
            UwpHelper.OpenUrl(strUrl);
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

        private void RichTextMain_SelectionChanged(object sender, RoutedEventArgs e)
        {
            double scrollViewerWidth = ScrollViewerMain.ActualWidth;
            double scrollViewerHeight = ScrollViewerMain.ActualHeight;
            GeneralTransform transformScrollViewer = ScrollViewerMain.TransformToVisual(UwpHelper.GetRootFrame());
            Point pointScrollViewer = transformScrollViewer.TransformPoint(new Point(0, 0));
            Point pointCursor = UwpHelper.GetCursorPointRelatedToRootFrame();
            double pointRelateScrollOffX = pointCursor.X - pointScrollViewer.X;
            double pointRelateScrollOffY = pointCursor.Y - pointScrollViewer.Y;

            double pointRelateScrollWidthOffX = pointRelateScrollOffX;
            if (pointRelateScrollWidthOffX > 0 && pointRelateScrollWidthOffX <= scrollViewerWidth)
            {
                // X inside
                pointRelateScrollWidthOffX = 0;
            }
            else if (pointRelateScrollWidthOffX > scrollViewerWidth)
            {
                // X outside right
                pointRelateScrollWidthOffX = pointRelateScrollWidthOffX - scrollViewerWidth;
            }

            double pointRelateScrollHeightOffY = pointRelateScrollOffY;
            if (pointRelateScrollHeightOffY > 0 && pointRelateScrollHeightOffY <= scrollViewerHeight)
            {
                // Y inside
                pointRelateScrollHeightOffY = 0;
            }
            else if (pointRelateScrollHeightOffY > scrollViewerHeight)
            {
                // Y outside right
                pointRelateScrollHeightOffY = pointRelateScrollHeightOffY - scrollViewerHeight;
            }
            // TextBlockSpeed.Text = string.Format("({0:0.00},{1:0.00})",
            //    pointRelateScrollWidthOffX, pointRelateScrollHeightOffY);

            if (pointRelateScrollWidthOffX == 0 && pointRelateScrollHeightOffY == 0)
            {
                // X and Y all inside
                // TextBlockSpeed.Text = TextBlockSpeed.Text + " no";
                return;
            }

            double scrollViewerCurOffX = ScrollViewerMain.HorizontalOffset;
            double scrollViewerCurOffY = ScrollViewerMain.VerticalOffset;
            double scrollViewerNewOffX = scrollViewerCurOffX;
            double scrollViewerNewOffY = scrollViewerCurOffY;
            if (pointRelateScrollWidthOffX != 0)
            {
                scrollViewerNewOffX += pointRelateScrollWidthOffX;
            }
            if (pointRelateScrollHeightOffY != 0)
            {
                scrollViewerNewOffY += pointRelateScrollHeightOffY;
            }
            ScrollViewerMain.ChangeView(scrollViewerNewOffX, scrollViewerNewOffY, null);
        }

        private void UIBridgeDelegate_PreparingCalcHandler()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                string strPrepare = m_resourceLoaderMain.GetString("ResultWaitingStart");
                m_runPrepare = UwpHelper.GenRunFromString(strPrepare);
                AppendInlineToTextMain(m_runPrepare);
            }));
        }

        private void UIBridgeDelegate_RemovePreparingCalcHandler()
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                if (m_runPrepare != null)
                {
                    m_runPrepare.Text = "";
                }
            }));
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
                AppendFileNameToTextMain(resultData);
            }));
        }

        private void UIBridgeDelegate_ShowFileMetaHandler(ResultDataNet resultData)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                AppendFileMetaToTextMain(resultData);
            }));
        }

        private void UIBridgeDelegate_ShowFileHashHandler(ResultDataNet resultData, bool uppercase)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                AppendFileHashToTextMain(resultData, uppercase);
            }));
        }

        private void UIBridgeDelegate_ShowFileErrHandler(ResultDataNet resultData)
        {
            _ = Dispatcher.RunAsync(CoreDispatcherPriority.Normal, new DispatchedHandler(() =>
            {
                AppendFileErrToTextMain(resultData);
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
