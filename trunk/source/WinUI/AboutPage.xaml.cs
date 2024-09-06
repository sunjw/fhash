using Microsoft.UI.Text;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Documents;
using Microsoft.Windows.ApplicationModel.Resources;
using SunJWBase;
using Windows.ApplicationModel;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace FilesHashWUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AboutPage : Page
    {
        private ResourceLoader m_resourceLoaderMain = WinUIHelper.GetCurrentResourceLoader();
        private bool m_pageInited = false;

        public AboutPage()
        {
            InitializeComponent();
        }

        private void GridAboutMain_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_pageInited)
            {
                return;
            }
            m_pageInited = true;

            NativeHelper nativeHelper = new();

            string strAboutTitle = m_resourceLoaderMain.GetString("AboutTitle");
            Run runAboutTitle = WinUIHelper.GenRunFromString(strAboutTitle);
            runAboutTitle.FontWeight = FontWeights.Bold;
            TextBlockAboutTitle.Inlines.Add(runAboutTitle);

            string strAboutInfoTitle = m_resourceLoaderMain.GetString("AboutInfoTitle");
            PackageVersion packVersion = Package.Current.Id.Version;
            string strVersion = string.Format("{0}.{1}.{2}.{3}",
                packVersion.Major, packVersion.Minor, packVersion.Build, packVersion.Revision);
            string strArch = nativeHelper.GetTargetArch();
            string strRight = m_resourceLoaderMain.GetString("AboutInfoRight");
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strAboutInfoTitle));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(" "));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strVersion));
            if (!string.IsNullOrEmpty(strArch))
            {
                TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(" "));
                TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strArch));
            }
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strRight));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n\r\n"));

            string strInfoMD5 = m_resourceLoaderMain.GetString("AboutInfoMD5");
            string strInfoSHA256 = m_resourceLoaderMain.GetString("AboutInfoSHA256");
            string strInfoSHA512 = m_resourceLoaderMain.GetString("AboutInfoSHA512");
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strInfoMD5));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strInfoSHA256));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strInfoSHA512));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n\r\n"));

            string strAboutInfoOsTitle = m_resourceLoaderMain.GetString("AboutInfoOsTitle");
            string strOsInfo = nativeHelper.GetWindowsInfo();
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strAboutInfoOsTitle));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString("\r\n"));
            TextBlockAbout.Inlines.Add(WinUIHelper.GenRunFromString(strOsInfo));
        }

        private void ButtonOK_Click(object sender, RoutedEventArgs e)
        {
            if (Frame.CanGoBack)
            {
                Frame.GoBack();
            }
        }
    }
}
