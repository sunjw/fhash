using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
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
        private bool m_pageInit = false;

        public MainPage()
        {
            InitializeComponent();

            m_mainWindow = MainWindow.CurrentWindow;
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

            HandleCommandLineArgs();
        }

        private void ButtonAbout_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(AboutPage));
        }

        private void RichTextMain_SelectionChanged(object sender, RoutedEventArgs e)
        {
            HandleRichTextSelectionScroll(ScrollViewerMain);
        }
    }
}
