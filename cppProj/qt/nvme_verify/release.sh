cd ../../nvme/myNVMe
make

cd ../../qt/nvme_verify
make

mv ~/frontEndApp/nvme_verify_ap.exe ~/frontEndApp/nvme_verify_ap.exe.bak
cp nvme_verify_ap.exe ~/frontEndApp/.

mv ~/frontEndApp/admin_ds.lib ~/frontEndApp/admin_ds.lib.bak
cp  admin_ds.lib ~/frontEndApp/.

#mv ~/frontEndApp/config.dat ~/frontEndApp/config.dat.bak
#cp  config.dat ~/frontEndApp/.
