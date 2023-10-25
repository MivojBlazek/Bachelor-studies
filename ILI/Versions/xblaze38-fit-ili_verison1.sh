#! /bin/bash

# 1)
dd if=/dev/zero of=/var/tmp/ukol.img bs=200MB count=1
echo "Creating ukol.img file in /var/tmp/"

# 2)
losetup /var/tmp/ukol.img # > XXX ### --find --show vezmeme nazev /dev/loop0
echo "Creating loop device for ukol.img"

# 3)
mkfs.ext4 /dev/loop0
echo "Making filesystem on loop device"

# 4)
# echo "/var/tmp/ukol.img /var/www/html/ukol ext4 defaults 0 0" > /etc/fstab
# echo "Creating record in /etc/fstab for automatic mounting fs"

# 5)
mkdir /var/www/html/ukol
echo "Making folder for our fs"

mount /dev/loop0 /var/www/html/ukol
echo "Mounting fs"

# 6)
cd /var/www/html/ukol
echo "Entering to /var/www/html/ukol directory"

for package in "$@"
do
	yum download --source $package
	echo "Downloading package $package"
done
echo "Downloading packages from arguments"

cd /root ### mby dat na zacatek taky, abych mel jistotu, kde jsem
echo "Entering back to /root directory"

# 7)
dnf install createrepo
echo "Installing createrepo package"

createrepo /var/www/html/ukol
echo "Creating repodata via createrepo"

restorecon -Rv /var/www/html/ukol
echo "Editing selinux context" ### ??? co to kurva je

# 8)
touch /etc/yum.repos.d/ukol.repo
echo "Creating file /etc/yum.repos.d/ukol.repo"

echo "[localrepo]\nname=ukol\nbaseurl=http://localhost/ukol" > /etc/yum.repos.d/ukol.repo
echo "Filling file ukol.repo"

# 9)
dnf install httpd
echo "Installing httpd package"

systemctl start httpd
echo "Starting httpd service"

# 10)
cd /var/www/html/ukol
echo "Entering to /var/www/html/ukol directory"

yum list # not sure
echo "Printing available repositories to check that \"ukol\" is available"

cd /root ### mby dat na zacatek taky, abych mel jistotu, kde jsem / mby to tady ani nepotrebuju
echo "Entering back to /root directory"

# 11)
umount /var/www/html/ukol # not sure
echo "Unmounting fs"

# 12)
mount -a # | grep ukol nejak to lepe dokazat
echo "Printing mounted filesystems to see that fs /dev/loop0 is mounted to /var/www/html/ukol"

# 13)
# yum list ukol
# echo ""
