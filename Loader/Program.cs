using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Loader
{
    static class Program
    {
        /// <summary>
        /// Der Haupteinstiegspunkt für die Anwendung.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Login fLogin = new Login();
            if (fLogin.ShowDialog() == DialogResult.OK)
            {
                Application.Run(new Main());
            }
        }
    }
}
