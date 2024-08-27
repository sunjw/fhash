using System;
using System.Collections.Generic;
using System.CommandLine.Parsing;
using System.Linq;
using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Documents;
using Microsoft.UI.Xaml.Media;
using Microsoft.Windows.ApplicationModel.Resources;
using Microsoft.Windows.AppLifecycle;
using SunJWBase;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace FilesHashWUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private enum MainPageControlStat
        {
            MainPageNone = 0, // clear stat
            MainPageCalcIng,  // calculating
            MainPageCalcFinish, // calculating finished/stopped
            MainPageVerify, // verifying
            MainPageWaitingExit, // waiting thread stop and exit
        };

        private const string KeyUppercase = "Uppercase";

        private MainWindow m_mainWindow = null;

        private ResourceLoader m_resourceLoaderMain = WinUIHelper.GetCurrentResourceLoader();
        private bool m_pageInited = false;

        private ContentDialog m_dialogFind = null;
        private TextBox m_textBoxFindHash = null;

        private Paragraph m_paragraphMain = null;
        private Paragraph m_paragraphResult = null;
        private Paragraph m_paragraphFind = null;
        private List<Hyperlink> m_hyperlinksMain = null;
        private List<Hyperlink> m_hyperlinksResult = new List<Hyperlink>();
        private List<Hyperlink> m_hyperlinksFind = new List<Hyperlink>();
        private MenuFlyout m_menuFlyoutTextMain = null;
        private Hyperlink m_hyperlinkClicked = null;
        private Run m_runPrepare = null;

        private MainPageControlStat m_mainPageStat;
        private bool m_uppercaseChecked = false;
        private long m_calcStartTime = 0;
        private long m_calcEndTime = 0;

        public MainPage()
        {
            InitializeComponent();

            m_mainWindow = MainWindow.CurrentWindow;
            m_mainWindow.RedirectedEventHandler += MainWindow_RedirectedEventHandler;
        }

        private void HideAboutPage()
        {
            if (Frame.CanGoBack)
                Frame.GoBack();
        }

        private void ScrollTextMainToBottom()
        {
            WinUIHelper.ScrollViewerToBottom(ScrollViewerMain);
        }

        private Hyperlink GenHyperlinkFromStringForRichTextMain(string strContent)
        {
            return WinUIHelper.GenHyperlinkFromString(strContent, RichTextMainHyperlink_Click);
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
            List<Inline> inlines = [inline];
            AppendInlinesToTextMain(inlines);
        }

        private void ClearTextMain()
        {
            m_paragraphMain.Inlines.Clear();
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
                        m_mainWindow.HashMgmt.Clear();

                        ProgressBarMain.Value = 0;
                        TextBlockSpeed.Text = "";

                        Span spanInit = new Span();
                        string strPageInit = m_resourceLoaderMain.GetString("MainPageInitInfo");
                        spanInit.Inlines.Add(WinUIHelper.GenRunFromString(strPageInit));
                        spanInit.Inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
                        ClearTextMain();
                        AppendInlineToTextMain(spanInit);
                    }
                    // Passthrough to MainPageControlStat.MainPageCalcFinish
                    m_calcEndTime = WinUIHelper.GetCurrentMilliSec();

                    ButtonOpen.Content = m_resourceLoaderMain.GetString("ButtonOpenOpen");
                    ButtonClear.IsEnabled = true;
                    ButtonVerify.IsEnabled = true;
                    CheckBoxUppercase.IsEnabled = true;
                    break;
                case MainPageControlStat.MainPageCalcIng:
                    HideAboutPage();

                    m_calcStartTime = WinUIHelper.GetCurrentMilliSec();
                    m_mainWindow.HashMgmt.SetStop(false);

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
                DispatcherQueue.TryEnqueue(m_mainWindow.Close);
            }
        }

        private void CalculateFinished()
        {
            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);
            ProgressBarMain.Value = m_mainWindow.UIBridgeHandlers.GetProgMax();

            long calcDurationTime = m_calcEndTime - m_calcStartTime;
            if (calcDurationTime > 10)
            {
                // speed is Bytes/ms
                double calcSpeed = ((double)m_mainWindow.HashMgmt.GetTotalSize()) / calcDurationTime;
                calcSpeed = calcSpeed * 1000; // Bytes/s
                ulong ulCalcSpeed = (ulong)calcSpeed;
                string strSpeed = "";
                if (ulCalcSpeed > 0)
                {
                    strSpeed = WinUIHelper.ConvertSizeToShortSizeStr(ulCalcSpeed, true);
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
            AppendInlineToTextMain(WinUIHelper.GenRunFromString("\r\n"));

            SetPageControlStat(MainPageControlStat.MainPageCalcFinish);
            ProgressBarMain.Value = 0;
        }

        private void AppendFileNameToTextMain(ResultDataNet resultData)
        {
            List<Inline> inlines = new List<Inline>();
            string strAppend = m_resourceLoaderMain.GetString("ResultFileName");
            strAppend += " ";
            strAppend += resultData.Path;
            inlines.Add(WinUIHelper.GenRunFromString(strAppend));
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileMetaToTextMain(ResultDataNet resultData)
        {
            string strShortSize = WinUIHelper.ConvertSizeToShortSizeStr(resultData.Size);
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
            inlines.Add(WinUIHelper.GenRunFromString(strSize));
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlines.Add(WinUIHelper.GenRunFromString(strModifiedTime));
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            if (!string.IsNullOrEmpty(resultData.Version))
            {
                string strVersion = m_resourceLoaderMain.GetString("ResultFileVersion");
                strVersion += " ";
                strVersion += resultData.Version;
                inlines.Add(WinUIHelper.GenRunFromString(strVersion));
                inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
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
            inlines.Add(WinUIHelper.GenRunFromString("MD5: "));
            Hyperlink hyperlinkMD5 = GenHyperlinkFromStringForRichTextMain(strFileMD5);
            m_hyperlinksMain.Add(hyperlinkMD5);
            inlines.Add(hyperlinkMD5);
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlines.Add(WinUIHelper.GenRunFromString("SHA1: "));
            Hyperlink hyperlinkSHA1 = GenHyperlinkFromStringForRichTextMain(strFileSHA1);
            m_hyperlinksMain.Add(hyperlinkSHA1);
            inlines.Add(hyperlinkSHA1);
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlines.Add(WinUIHelper.GenRunFromString("SHA256: "));
            Hyperlink hyperlinkSHA256 = GenHyperlinkFromStringForRichTextMain(strFileSHA256);
            m_hyperlinksMain.Add(hyperlinkSHA256);
            inlines.Add(hyperlinkSHA256);
            inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlines.Add(WinUIHelper.GenRunFromString("SHA512: "));
            Hyperlink hyperlinkSHA512 = GenHyperlinkFromStringForRichTextMain(strFileSHA512);
            m_hyperlinksMain.Add(hyperlinkSHA512);
            inlines.Add(hyperlinkSHA512);
            inlines.Add(WinUIHelper.GenRunFromString("\r\n\r\n"));
            AppendInlinesToTextMain(inlines);
        }

        private void AppendFileErrToTextMain(ResultDataNet resultData)
        {
            List<Inline> inlines = new List<Inline>();
            string strAppend = resultData.Error;
            inlines.Add(WinUIHelper.GenRunFromString(strAppend));
            inlines.Add(WinUIHelper.GenRunFromString("\r\n\r\n"));
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
                AppendInlineToTextMain(WinUIHelper.GenRunFromString("\r\n"));
            }
        }

        private void HandleRichTextSelectionScroll(ScrollViewer scrollViewerWrapper)
        {
            //string strDebug = "";

            // cursor position
            double scale = m_mainWindow.Scale;
            System.Drawing.Point pointCursor = m_mainWindow.GetCursorRelativePoint();

            //strDebug = string.Format("{0:0.00} : {1:0.00}", pointCursor.X, pointCursor.Y);
            //TextBlockDebug.Text = strDebug;

            // ScrollView position
            GeneralTransform transformScrollView = scrollViewerWrapper.TransformToVisual(null);
            Windows.Foundation.Point pointScrollView = transformScrollView.TransformPoint(new(0, 0));

            // cursor offset relative to ScrollView
            double cursorRelateScrollOffX = pointCursor.X - pointScrollView.X - (scrollViewerWrapper.Margin.Left * scale);
            double cursorRelateScrollOffY = pointCursor.Y - pointScrollView.Y - (scrollViewerWrapper.Margin.Top * scale);

            double scrollViewWidth = scrollViewerWrapper.ActualWidth * scale;
            double scrollViewHeight = scrollViewerWrapper.ActualHeight * scale;

            double cursorOutScrollWidthOffX = cursorRelateScrollOffX;
            if (cursorOutScrollWidthOffX > 0 && cursorOutScrollWidthOffX <= scrollViewWidth)
            {
                // X inside
                cursorOutScrollWidthOffX = 0;
            }
            else if (cursorOutScrollWidthOffX > scrollViewWidth)
            {
                // X outside right
                cursorOutScrollWidthOffX = cursorOutScrollWidthOffX - scrollViewWidth;
            }

            double cursorOutScrollHeightOffY = cursorRelateScrollOffY;
            if (cursorOutScrollHeightOffY > 0 && cursorOutScrollHeightOffY <= scrollViewHeight)
            {
                // Y inside
                cursorOutScrollHeightOffY = 0;
            }
            else if (cursorOutScrollHeightOffY > scrollViewHeight)
            {
                // Y outside right
                cursorOutScrollHeightOffY = cursorOutScrollHeightOffY - scrollViewHeight;
            }

            //strDebug = string.Format("{0:0.00} : {1:0.00}", cursorOutScrollWidthOffX, cursorOutScrollHeightOffY);

            if (cursorOutScrollWidthOffX == 0 && cursorOutScrollHeightOffY == 0)
            {
                // X and Y all inside
                //strDebug = string.Format("{0:0.00} : {1:0.00}", cursorOutScrollWidthOffX, cursorOutScrollHeightOffY);
                //TextBlockDebug.Text = strDebug;
                return;
            }

            double scrollViewCurOffX = scrollViewerWrapper.HorizontalOffset;
            double scrollViewCurOffY = scrollViewerWrapper.VerticalOffset;
            double scrollViewNewOffX = scrollViewCurOffX + cursorOutScrollWidthOffX;
            double scrollViewNewOffY = scrollViewCurOffY + cursorOutScrollHeightOffY;

            //strDebug = string.Format("{0:0.00} : {1:0.00}", scrollViewNewOffX, scrollViewNewOffY);
            WinUIHelper.ScrollViewerScrollTo(scrollViewerWrapper, scrollViewNewOffX, scrollViewNewOffY);

            //TextBlockDebug.Text = strDebug;
        }

        private void MainWindow_RedirectedEventHandler(string strAppActiveArgs)
        {
            if (Frame.CanGoBack)
                Frame.GoBack();

            OnRedirected(strAppActiveArgs);
        }

        private void OnRedirected(string someArgs)
        {
            if (string.IsNullOrEmpty(someArgs))
                return;

            string[] splitArgs = CommandLineStringSplitter.Instance.Split(someArgs).ToArray();

            List<Inline> inlinesTest = new();
            foreach (string argPart in splitArgs)
            {
                inlinesTest.Add(WinUIHelper.GenRunFromString(argPart));
                inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            }
        }

        private void GridMain_Loaded(object sender, RoutedEventArgs e)
        {
            if (!m_pageInited)
            {
                DispatcherQueue.TryEnqueue(() =>
                {
                    AppActivationArguments appActiveArgs = WinUIHelper.GetCurrentActivatedEventArgs();
                    string strAppActiveArgs = WinUIHelper.GetLaunchActivatedEventArgs(appActiveArgs);
                    OnRedirected(strAppActiveArgs);
                });

                m_pageInited = true;
            }

            // Fix for color changed.
            DispatcherQueueTimer timerScrollBar = DispatcherQueue.CreateTimer();
            timerScrollBar.Interval = TimeSpan.FromMilliseconds(300);
            timerScrollBar.IsRepeating = false;
            timerScrollBar.Tick += (timer, sender) =>
            {
                ScrollViewerMain.HorizontalScrollBarVisibility = ScrollBarVisibility.Auto;
                ScrollViewerMain.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
            };
            timerScrollBar.Start();
        }

        private void GridMain_Unloaded(object sender, RoutedEventArgs e)
        {
        }

        private void RichTextMainHyperlink_Click(Hyperlink sender, HyperlinkClickEventArgs args)
        {

        }

        private void RichTextMain_SelectionChanged(object sender, RoutedEventArgs e)
        {
            HandleRichTextSelectionScroll(ScrollViewerMain);
        }

        private void ButtonAbout_Click(object sender, RoutedEventArgs e)
        {
            // Fix for color changed.
            ScrollViewerMain.HorizontalScrollBarVisibility = ScrollBarVisibility.Hidden;
            ScrollViewerMain.VerticalScrollBarVisibility = ScrollBarVisibility.Hidden;

            Frame.Navigate(typeof(AboutPage));
        }

        private void UIBridgeDelegate_PreparingCalcHandler()
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                string strPrepare = m_resourceLoaderMain.GetString("ResultWaitingStart");
                m_runPrepare = WinUIHelper.GenRunFromString(strPrepare);
                AppendInlineToTextMain(m_runPrepare);
            });
        }

        private void UIBridgeDelegate_RemovePreparingCalcHandler()
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                if (m_runPrepare != null)
                {
                    m_runPrepare.Text = "";
                }
            });
        }

        private void UIBridgeDelegate_CalcStopHandler()
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                CalculateStopped();
            });
        }

        private void UIBridgeDelegate_CalcFinishHandler()
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                CalculateFinished();
            });
        }

        private void UIBridgeDelegate_ShowFileNameHandler(ResultDataNet resultData)
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                AppendFileNameToTextMain(resultData);
            });
        }

        private void UIBridgeDelegate_ShowFileMetaHandler(ResultDataNet resultData)
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                AppendFileMetaToTextMain(resultData);
            });
        }

        private void UIBridgeDelegate_ShowFileHashHandler(ResultDataNet resultData, bool uppercase)
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                AppendFileHashToTextMain(resultData, uppercase);
            });
        }

        private void UIBridgeDelegate_ShowFileErrHandler(ResultDataNet resultData)
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                AppendFileErrToTextMain(resultData);
            });
        }

        private void UIBridgeDelegate_UpdateProgWholeHandler(int value)
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                double newValue = value;
                double oldValue = ProgressBarMain.Value;
                if (oldValue == newValue)
                {
                    return;
                }
                ProgressBarMain.Value = newValue;
            });
        }
    }
}
