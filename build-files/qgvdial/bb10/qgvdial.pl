if ($#ARGV != 1) {
    printf("Incorrect args: %d\n", $#ARGV);
    die;
}

my $makejobs=2;
my $repo = "https://qgvdial.googlecode.com/svn/trunk";
my $cmd;

# Delete any existing version file
system("rm -f ver.cfg");
# Get the latest version file from the repository
$cmd = "svn export $repo/build-files/ver.cfg";
system($cmd);

# Pull out the version from the file
open(QVARFILE, "ver.cfg") or die;
my $qver = <QVARFILE>;
close QVARFILE;
chomp $qver;

# Get the subversion checkin version
my $svnver = `svn log $repo --limit=1 | grep \"^r\"`;
# Parse out the version number from the output we pulled out
$svnver =~ m/^r(\d+)*/;
$svnver = $1;
# Create the version suffix
$qver = "$qver.$svnver";

# Get the full path of the base directory
my $basedir = `pwd`;
chomp $basedir;
$basedir = "$basedir/qgvdial-$qver";

# Delete any previous checkout directories
system("rm -rf tmp.sh qgvdial-* qgvtp-* qgvdial_* qgvtp_* qgvdial*.bar version.pl");

$cmd = "svn export $repo qgvdial-$qver";
system($cmd);
system("cp $basedir/build-files/version.pl .");

# Version replacement
$cmd = "perl version.pl __QGVDIAL_VERSION__ $qver $basedir";
print "$cmd\n";
system($cmd);

# Cipher replacement
open my $qgvcipfile, '<', "cipher_qgvdial";
my $cipher = <$qgvcipfile>;
close $qgvcipfile;
chomp $cipher;
$cmd = "perl version.pl __THIS_IS_MY_EXTREMELY_LONG_KEY_ '$cipher' $basedir";
print "$cmd\n";
system($cmd);

# __QT5_BB10__ replacement
my $rep = $ARGV[1];
$rep =~ s/\//\\\//g;
print($rep);
$cmd = "perl version.pl __QT5_BB10__ '$rep' $basedir";
print "$cmd\n";
system($cmd);

# Mixpanel replacement
open my $mixpanel_token_file, '<', "mixpanel.token";
my $mixpanel_token = <$mixpanel_token_file>;
close $mixpanel_token_file;
chomp $mixpanel_token;
$cmd = "perl version.pl __MY_MIXPANEL_TOKEN__ '$mixpanel_token' $basedir";
print "$cmd\n";
system($cmd);

# Copy the client secret file to the api directory
$cmd = "cd $basedir/api ; cp ../../client_secret_284024172505-2go4p60orvjs7hdmcqpbblh4pr5thu79.apps.googleusercontent.com.json .";
print "$cmd\n";
system($cmd);

# Prepare the bar-descriptor
$cmd = "cd $basedir/qgvdial/bb10-qt5 ; mv bar-descriptor-deploy.xml bar-descriptor.xml";
print "$cmd\n";
system($cmd);

system("rm -f tmp.sh ; echo '#!/bin/bash' > tmp.sh ; echo source $ARGV[0] qt5 arm >> tmp.sh ; echo 'cd $basedir/qgvdial/bb10-qt5 ; qmake ; make -j$makejobs ; ntoarmv7-strip qgvdial' >> tmp.sh");
$cmd = "chmod +x tmp.sh ; cat tmp.sh ; ./tmp.sh ; rm tmp.sh";
print "$cmd\n";
system($cmd);

system("rm -f tmp.sh ; echo '#!/bin/bash' > tmp.sh ; echo source $ARGV[0] qt4 arm >> tmp.sh ; echo 'cd $basedir/qgvdial/bb10-qt5/qt4srv ; qmake ; make -j$makejobs ; ntoarmv7-strip qt4srv' >> tmp.sh");
$cmd = "chmod +x tmp.sh ; cat tmp.sh ; ./tmp.sh ; rm tmp.sh";
print "$cmd\n";
system($cmd);

system("rm -f tmp.sh ; echo '#!/bin/bash' > tmp.sh ; echo source $ARGV[0] qt5 arm >> tmp.sh ; echo 'cd $basedir/qgvdial/bb10-qt5 ; blackberry-nativepackager -package qgvdial_$qver.bar bar-descriptor.xml' >> tmp.sh");
$cmd = "chmod +x tmp.sh ; cat tmp.sh ; ./tmp.sh ; rm tmp.sh";
print "$cmd\n";
system($cmd);

$cmd = "cd $basedir/qgvdial/bb10-qt5 ; cp qgvdial_$qver.bar ../../..";
print "$cmd\n";
system($cmd);
