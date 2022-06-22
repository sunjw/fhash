using Windows.ApplicationModel;
using Windows.ApplicationModel.Resources;
using Windows.UI.Text;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Documents;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

namespace FilesHashUwp
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class AboutPage : UserControl
    {
        private ResourceLoader m_resourceLoaderMain;

        private bool m_textBlockAboutInit = false;

        public AboutPage()
        {
            InitializeComponent();

            m_resourceLoaderMain = ResourceLoader.GetForCurrentView();
        }

        private void HideSelf()
        {
            MainPage mainPage = MainPage.GetCurrentMainPage();
            mainPage?.HidePopupAbout();
        }

        private void GridRoot_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_textBlockAboutInit)
            {
                return;
            }
            m_textBlockAboutInit = true;

            NativeHelper nativeHelper = new NativeHelper();

            string strAboutTitle = m_resourceLoaderMain.GetString("AboutTitle");
            Run runAboutTitle = UwpHelper.GenRunFromString(strAboutTitle);
            runAboutTitle.FontWeight = FontWeights.Bold;
            TextBlockAboutTitle.Inlines.Add(runAboutTitle);

            string strAboutInfoTitle = m_resourceLoaderMain.GetString("AboutInfoTitle");
            PackageVersion packVersion = Package.Current.Id.Version;
            string strVersion = string.Format("{0}.{1}.{2}.{3}",
                packVersion.Major, packVersion.Minor, packVersion.Build, packVersion.Revision);
            string strArch = nativeHelper.GetTargetArch();
            string strRight = m_resourceLoaderMain.GetString("AboutInfoRight");
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strAboutInfoTitle));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(" "));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strVersion));
            if (!string.IsNullOrEmpty(strArch))
            {
                TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(" "));
                TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strArch));
            }
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strRight));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));

            string strInfoMD5 = m_resourceLoaderMain.GetString("AboutInfoMD5");
            string strInfoSHA256 = m_resourceLoaderMain.GetString("AboutInfoSHA256");
            string strInfoSHA512 = m_resourceLoaderMain.GetString("AboutInfoSHA512");
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strInfoMD5));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strInfoSHA256));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strInfoSHA512));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n\r\n"));


            string strAboutInfoOsTitle = m_resourceLoaderMain.GetString("AboutInfoOsTitle");
            string strOsInfo = nativeHelper.GetWindowsInfo();
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strAboutInfoOsTitle));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(UwpHelper.GenRunFromString(strOsInfo));
        }

        private void ButtonOK_Click(object sender, RoutedEventArgs e)
        {
            HideSelf();
        }

        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            HideSelf();
        }
    }
}
