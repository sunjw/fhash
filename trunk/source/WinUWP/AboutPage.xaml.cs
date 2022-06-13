using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

namespace FilesHashUwp
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class AboutPage : UserControl
    {
        public AboutPage()
        {
            this.InitializeComponent();
        }

        private void HideSelf()
        {
            MainPage mainPage = MainPage.GetCurrentMainPage();
            mainPage?.HidePopupAbout();
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
