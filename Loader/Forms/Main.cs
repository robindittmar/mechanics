using System;
using System.Windows.Forms;

namespace Loader
{
    enum HackStatus
    {
        Undetected,
        Updating,
        Unknown
    }

    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();

            // TODO: get player informations
            lblUsername.Text = "Myridor";
        }

        private void listBox_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            // TODO: check if player is allowed to inject
            // TODO: get informations about hack
            HackStatus iHackStatus = (HackStatus)listBox.SelectedIndex;
            bool bEnableInject = false;
            DateTime dtExpiresOn = DateTime.Now;
            dtExpiresOn = dtExpiresOn.AddMinutes(listBox.SelectedIndex);
            // TODO:

            lblExpiresOnValue.Text = dtExpiresOn.ToShortDateString() + ' ' + dtExpiresOn.ToShortTimeString();
            lblExpiresOnValue.ForeColor = dtExpiresOn > DateTime.Now ? System.Drawing.Color.Green : System.Drawing.Color.Red;

            switch (iHackStatus)
            {
                case HackStatus.Unknown:
                    lblStatusValue.Text = "Unknown";
                    lblStatusValue.ForeColor = System.Drawing.Color.Orange;
                    bEnableInject = true;
                    break;
                case HackStatus.Updating:
                    lblStatusValue.Text = "Updating";
                    lblStatusValue.ForeColor = System.Drawing.Color.Red;
                    bEnableInject = false;
                    break;
                case HackStatus.Undetected:
                    lblStatusValue.Text = "Undetected";
                    lblStatusValue.ForeColor = System.Drawing.Color.Green;
                    bEnableInject = true;
                    break;
            }

            btnInject.Enabled = bEnableInject;
        }

        private void btnInject_Click(object sender, System.EventArgs e)
        {
            // TODO: inject the game
        }
    }
}
