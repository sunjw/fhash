using System.Collections.Generic;
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
        private MainWindow m_mainWindow;

        private ResourceLoader m_resourceLoaderMain = WinUIHelper.GetCurrentResourceLoader();
        private bool m_pageInit = false;

        private Paragraph m_paragraphTest = new();

        public MainPage()
        {
            InitializeComponent();

            m_mainWindow = MainWindow.CurrentWindow;
        }

        private Hyperlink GenHyperlinkFromStringForRichTextMain(string strContent)
        {
            return WinUIHelper.GenHyperlinkFromString(strContent, RichTextMainHyperlink_Click);
        }

        private void ShowTest()
        {
            m_paragraphTest.FontFamily = new FontFamily("Consolas");
            m_paragraphTest.LineHeight = 18;
            m_paragraphTest.LineStackingStrategy = LineStackingStrategy.BlockLineHeight;

            RichTextMain.TextWrapping = TextWrapping.NoWrap;
            RichTextMain.Blocks.Clear();
            RichTextMain.Blocks.Add(m_paragraphTest);

            List<Inline> inlinesTest = new();

            inlinesTest.Add(WinUIHelper.GenRunFromString("Name: C:\\Users\\sunjw\\Downloads\\"));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("江苏省省道公路网规划（2023—2035 年）.pdf"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("File Size: 2417386 Byte(s) (2.31 MB)"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("Modified Date: 2024-04-11 14:18"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("MD5: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("E44B1734EA01D8C76D2CEABBBA8DF964"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA1: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("A79DE28FB4F6083661467457C13A6A3DC02F154D"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA256: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("74FA39D52397BCA1816DF251504490BCC2F5E987D2947DDF0C4D576CDC1A45AC"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA512: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("317084BEA676D335B0BFAC49421F0510B561CD1D967F1481AF119DFDA51B91EDA41B23D4BC2DFECB7A9CE6531D1412DAB17C805439573C545CCAD1AD737F797A"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));

            inlinesTest.Add(WinUIHelper.GenRunFromString("文件名: /Users/sunjw/Library/CloudStorage/OneDrive-个人/Apps/fHash/insider/"));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("fHash-3.3.2-macOS.dmg"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("文件大小: 725422 字节 (725.42 KB)"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("修改日期: 2024-06-03 14:02"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("MD5: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("249B290A86B3AB1A6836682BA7C79160"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA1: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("054DD2135CD9397EEBEDFB3E4570C0CC0552E3B3"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA256: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("04F0D0D7BD3F95300F78D20D056331D0D11BA2FF4A4839E6AEF36AE650958CAB"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA512: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("A0E36C44680CC1A8B2FBAD283956300273D23F6693B47D6528792CCA56E69FC5871573CD87B0BDD5C286ABF9166F70346DB5FE81C40564567EBE97975A7AD9AE"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));

            inlinesTest.Add(WinUIHelper.GenRunFromString("文件名: C:\\Users\\Sun Junwen\\OneDrive\\Apps\\fHash\\insider\\"));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("fHashUwpWap_3.3.1.0_Test.7z"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("文件大小: 83672897 字节 (79.80 MB)"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("修改日期: 2024-04-22 23:06"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("MD5: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("0A0B98B7C370D2DCB611C581AC9C9278"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA1: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("7385065E0B5A74E9F9D0B32FE0C9CF788FB543DD"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA256: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("B37645E34682AE0A2BF87DA2BB5F361ADD919D2BCFE27FEEEC1B2C97DDB60B69"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("SHA512: "));
            inlinesTest.Add(GenHyperlinkFromStringForRichTextMain("DBBFEAE9818B0AB23D4D546FC635F4CC60B2178F9AC19A8FD1C435D905F05B495E16BEBFDE789F40C556A18640719C48B949F1CD8784E67FEBBB233BD1113741"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));
            inlinesTest.Add(WinUIHelper.GenRunFromString("\r\n"));

            // finish
            foreach (Inline inline in inlinesTest)
            {
                m_paragraphTest.Inlines.Add(inline);
            }

            WinUIHelper.ScrollViewerToBottom(ScrollViewerMain);
        }

        private void HandleCommandLineArgs()
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                AppActivationArguments args = WinUIHelper.GetCurrentActivatedEventArgs();
                string appActivateArgs = WinUIHelper.GetLaunchActivatedEventArgs(args);
                OnRedirected(appActivateArgs);
            });
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
            Windows.Foundation.Point pointScrollView = transformScrollView.TransformPoint(new Windows.Foundation.Point(0, 0));

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

        public void OnRedirected(string someArgs)
        {
        }

        private void GridMain_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_pageInit)
            {
                return;
            }
            m_pageInit = true;

            //ShowTest();

            //HandleCommandLineArgs();
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
            Frame.Navigate(typeof(AboutPage));
        }
    }
}
