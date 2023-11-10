#! /bin/bash

# 1)
echo "1) Creating ukol.img file in /var/tmp/"
dd if=/dev/zero of=/var/tmp/ukol.img bs=200MB count=1 >/dev/null 2>&1

# 2)
echo "2) Creating loop device for ukol.img"
losetup /dev/loop0 /var/tmp/ukol.img >/dev/null

# 3)
echo "3) Making filesystem on loop device"
mkfs.ext4 /dev/loop0 >/dev/null 2>&1

# 4)
echo "4) Creating record in /etc/fstab for automatic mounting fs"
echo "/var/tmp/ukol.img /var/www/html/ukol ext4 loop 0 0" >> /etc/fstab

# 5)
echo "5a) Making folder for our fs"
mkdir -p /var/www/html/ukol >/dev/null

echo "5b) Mounting fs"
mount /dev/loop0 /var/www/html/ukol >/dev/null

echo "5c) Reloading system configurations"
systemctl daemon-reload >/dev/null

# 6)
echo "6a) Entering to /var/www/html/ukol directory"
cd /var/www/html/ukol >/dev/null

echo "6b) Downloading packages from arguments"
for package in "$@"
do
	echo "...downloading package $package"
	yum download --source $package -q
done

echo "6c) Entering back to /root directory"
cd /root >/dev/null

# 7)
echo "7a) Installing createrepo package"
dnf install createrepo -y -q

echo "7b) Creating repodata via createrepo"
createrepo /var/www/html/ukol >/dev/null

echo "7c) Editing selinux context"
restorecon -Rv /var/www/html/ukol >/dev/null

# 8)
echo "8a) Creating file /etc/yum.repos.d/ukol.repo"
touch /etc/yum.repos.d/ukol.repo >/dev/null

echo "8b) Filling file ukol.repo"
echo "[localrepo]" > /etc/yum.repos.d/ukol.repo
echo "name=ukol" >> /etc/yum.repos.d/ukol.repo
echo "baseurl=http://localhost/ukol" >> /etc/yum.repos.d/ukol.repo

# 9)
echo "9a) Installing httpd package"
dnf install httpd -y -q

echo "9b) Starting httpd service"
systemctl start httpd >/dev/null

# 10)
echo "10a) Printing available repositories to check that \"ukol\" is available"
yum repolist | if grep ukol
then
	echo "10b) Repository \"ukol\" IS available"
else
	echo "10b) Repository \"ukol\" IS NOT available"
fi

# 11)
echo "11) Unmounting fs"
umount /var/www/html/ukol >/dev/null

# 12)
mount -a
echo "12a) Checking if loop device fs is mounted"

if grep -qs '/var/www/html/ukol' /proc/mounts
then
	echo "12b) The fs on loop device IS mounted"
else
	echo "12b) The fs on loop device IS NOT mounted"
fi

# 13)
echo "13) Printing information about available packages in repository \"ukol\""
yum --disablerepo="*" --enablerepo="localrepo" list available
