#! /bin/bash

# 1)
dd if=/dev/zero of=/var/tmp/ukol.img bs=200MB count=1
echo "Creating ukol.img file in /var/tmp/"

# 2)
losetup /dev/loop0 /var/tmp/ukol.img # > XXX ### --find --show vezmeme nazev /dev/loop0
echo "Creating loop device for ukol.img"

# 3)
mkfs.ext4 /dev/loop0
echo "Making filesystem on loop device"

# 4)
echo "/var/tmp/ukol.img /var/www/html/ukol ext4 loop 0 0" >> /etc/fstab # /var/tmp/ukol.img misto /dev/loop0
echo "Creating record in /etc/fstab for automatic mounting fs"

# 5)
mkdir -p /var/www/html/ukol
echo "Making folder for our fs"

mount /dev/loop0 /var/www/html/ukol
echo "Mounting fs"

### systemctl daemon-reload

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
dnf install createrepo -y
echo "Installing createrepo package"

createrepo /var/www/html/ukol
echo "Creating repodata via createrepo"

restorecon -Rv /var/www/html/ukol
echo "Editing selinux context" ### ??? co to kurva je

# 8)
touch /etc/yum.repos.d/ukol.repo
echo "Creating file /etc/yum.repos.d/ukol.repo"

echo "[localrepo]" > /etc/yum.repos.d/ukol.repo
echo "name=ukol" >> /etc/yum.repos.d/ukol.repo
echo "baseurl=http://localhost/ukol" >> /etc/yum.repos.d/ukol.repo
echo "Filling file ukol.repo"

# 9)
dnf install httpd -y
echo "Installing httpd package"

systemctl start httpd
echo "Starting httpd service"

# 10)
cd /var/www/html/ukol
echo "Entering to /var/www/html/ukol directory"

yum repolist # not sure
echo "Printing available repositories to check that \"ukol\" is available"

cd /root ### mby dat na zacatek taky, abych mel jistotu, kde jsem / mby to tady ani nepotrebuju
echo "Entering back to /root directory"

# 11)
umount /var/www/html/ukol # not sure
echo "Unmounting fs"

# 12)
mount -a
echo "Checking if loop device fs is mounted"

if grep -qs '/var/www/html/ukol' /proc/mounts
then
	echo "The fs on loop device IS mounted"
else
	echo "The fs on loop device IS NOT mounted"
fi

# 13)
yum --disablerepo="*" --enablerepo="localrepo" list available
echo "printing information about available packages in repository \"ukol\""
