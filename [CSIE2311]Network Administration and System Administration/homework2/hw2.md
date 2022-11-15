# **NASA homework2**



## Network Administration

### Iperf3

我是透過在一台裝置當做server，先呼叫`ifconfig`，得到其IP address，再透過指令`iperf3 -s`使其成為server。而另一台當client，透過`iperf3 -c <server IP> -n 100M`即可。

- 兩台連接csie(無線網路)的裝置：
  ![Screen Shot 2022-03-20 at 4.34.55 PM](/Users/chriscyhxx/Desktop/Screen Shot 2022-03-20 at 4.34.55 PM.png)

- 一台連接csie(無線網路)與一台連接csie-5G(無線網路)的裝置：
  ![Screen Shot 2022-03-20 at 4.37.40 PM](/Users/chriscyhxx/Desktop/Screen Shot 2022-03-20 at 4.37.40 PM.png)

  <div style="page-break-after: always;"></div>

- 兩台連接csie-5G(無線網路)的裝置：
  ![Screen Shot 2022-03-20 at 4.39.27 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-20 at 4.39.27 PM.png)
  (Ref: https://blog.gtwang.org/useful-tools/iperf-network-bandwidth-testing-tool-tutorial/)

### IPv6

![Screen Shot 2022-03-17 at 10.50.01 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-17 at 10.50.01 AM.png)

- 步驟與指令：

1. 先連接到工作站：`ssh b08902149@oasis1.csie.ntu.edu.tw`
2. 透過`ifonfig`查看IPv6位址
3. 透過`telnet`與port進行connect：`telnet -6 fe80::5054:ff:fecf:12d9%net0 8888`
   (IPv6地址的telnet操作：`telnet <ipv6地址>%<接口序號> <port number>`)

(Ref: https://zhuanlan.zhihu.com/p/144171272)



### Cisco Switch

#### 1. VLAN, Access, and Trun

1. 因為對 `Gi1/0/5` 而言， vlan307 是Native vlan，所以只要沒有tag的封包它也不會幫它上tag，所以看封包 的802.1q header是沒有vlan ID的。而對` Gi1/0/4` 而言，它與另外一台之間沒有定義Native Vlan，所以會替 該封包的802.1q header寫上vlan ID為307。對` Gi1/0/3 `而言，該封包只是在switch內部傳輸，所以也無需 上tag，該封包的802.1q header也不會寫上vlan ID。
   (Ref: https://www.jannet.hk/virtual-lan-vlan-zh-hant/)

2. 因為`Gi1/0/1`是tagged port並且沒有Native VLAN的設定，因此在通過`Gi1/0/1`之前，封包的
   802.1q header會紀錄著上面的tag為VLAN 424。而封包要離開`Gi1/0/2`時，因為`Gi1/0/2`是untagged port，此時就會把封包的tag移除，因此在離開`Gi1/0/2`之後，封包的802.1q header是沒有tag的。
   (Ref: https://networkdirection.net/articles/network-theory/taggeduntaggedandnativevlans/)

3. 如果有一個封包的VLAN ID與某個port的Native VLAN吻合，該port就會把那個封包的tag移除再傳出。該指令用在有switch-to-switch的communication上面，當兩台switch需要分享connected devices的資訊時，就可以這樣設定，sending switch就不需要決定要選擇使用什麼VLAN來傳遞這個資訊。如果今天我們希望將某個VLAN ID設成Native VLAN，我們就可以在trunk link的兩邊interface透過`switchport trunk native`的指令設定相同的Native VLAN。需注意trunk link的兩邊需要設定相同的Native VLAN，不然會造成Native VLAN mismatch的狀況。
   (Ref1: https://networkdirection.net/articles/network-theory/taggeduntaggedandnativevlans/)
   (Ref2: https://www.jannet.hk/virtual-lan-vlan-zh-hant/)

   

#### 2. More on Link Aggregation

1. 不可以，因為要做Link Aggregation需要是相同速度的網路線。
   (Ref: 上課講義)

2. 錯誤點在於兩邊的`Gi1/0/1-2`都設`channel-group 1 mode passive`。因為兩邊都在等對方主動發出LACP封包來協商，所以就沒辦法建立EtherChannel。應該至少將一邊的`Gi1/0/1-2`都設成`channel-group 1 mode active`即可。
   (Ref: https://www.jannet.hk/etherchannel-pagp-lacp-zh-hant/)

   <div style="page-break-after: always;"></div>

#### 3. Network Debugging

1. 因為NewUser的privilege是2，他沒有privilege 15的使用者下放`configure`指令給他使用。
   (Ref: https://www.twblogs.net/a/5b96ef292b717750bda788dc)

2. 透過NewUser使用`show run`可以看到admin的password是經過"type 7"的cisco password，而網路上也找得到Decrypt的方法，可以知道admin的密碼是`TopSecret`。
   (CISCO Type 7 Password Decrypt : https://www.firewall.cx/cisco-technical-knowledgebase/cisco-routers/358-cisco-type7-password-crack.html)
   (Ref: https://blog.xuite.net/towns/hc/553633779)

3. 透過`show run`與`show vlan`可知要把`Gi1/0/3`和`Gi1/0/2`的shutdown解除，並且把`vlan 100`和
   `vlan 200`啟用。用以下指令即可。

   ```
   Core(config-if)#vlan 100
   Core(config-vlan)#vlan 200
   Core(config-vlan)#int Gi1/0/3
   Core(config-if)#no shutdown
   Core(config-vlan)#int Gi1/0/3
   Core(config-if)#no shutdown
   ```

   ![Screen Shot 2022-03-18 at 12.46.20 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-18 at 12.46.20 AM.png)
   (討論對象：b05504066 李旻翰)

   <div style="page-break-after: always;"></div>

## System Administration

(SA討論對象：b09505014王聖文)

#### 1. 與其他作業系統共用檔案

先以下列指令建立file system並且mount到`/mnt/usbdisk`

```shell
sudo mkfs -t ntfs /dev/sdi1
sudo mount -t ntfs3 /dev/sdi1 /mnt/usbdisk
```

再透過`ls -lh /dev/disk/by-uuid`查詢到`UUID=6737F20A06B9B0C3`後，在`/etc/fstab`裡加上：
`UUID=6737F20A06B9B0C3 /mnt/usbdisk ntfs defaults 1 2`即可在開機時自動掛載。![Screen Shot 2022-03-19 at 12.42.39 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-19 at 12.42.39 PM.png)
(Ref: https://wiki.archlinux.org/title/NTFS)



#### 2. 記憶體不足？

```shell
sudo fallocate -l 1GiB /myswap # Create a file that will be used for swap
sudo chmod 600 /myswap				 
sudo mkswap /myswap						 # Set up the file as Linux swap area
sudo swapon /myswap            # Enable the swap
```

最後在`/etc/fstab`多加上：`/myswap swap swap defaults 0 0 `即可完成。

![Screen Shot 2022-03-19 at 1.02.45 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-19 at 1.02.45 PM.png)

(Ref: https://linuxize.com/post/create-a-linux-swap-file/)

<div style="page-break-after: always;"></div>

#### 3. 空間不足

```shell
sudo lvresize -L 1GiB /dev/NasaHW2/course
sudo resize2fs /dev/mapper/NasaHW2-course
```

![Screen Shot 2022-03-19 at 1.44.44 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-19 at 1.44.44 PM.png)

(Ref: https://www.linuxtechi.com/extend-lvm-partitions/)

<div style="page-break-after: always;"></div>

#### 4. 建立加密分割區

```shell
sudo lvcreate -L 800MiB -n NasaHW2-homework /dev/NasaHW2
sudo shred -v --iterations=1 /dev/NasaHW2/NasaHW2-homework
sudo cryptsetup luksFormat /dev/NasaHW2/NasaHW2-homework
sudo cryptsetup luksAddKey /dev/NasaHW2/NasaHW2-homework /home/nasa/lvm_key
sudo cryptsetup luksDump /dev/NasaHW2/NasaHW2-homework
sudo cryptsetup luksOpen /dev/NasaHW2/NasaHW2-homework homework
sudo mkfs -t ext4 /dev/mapper/homework
sudo mount /dev/mapper/homework /home/nasa/homework/
```

在`/etc/crypttab`加上：`homework /dev/NasaHW2/NasaHW2-homework /home/nasa/lvm_key luks`，
並且在`/etc/fstab`加上：`/dev/mapper/homework /home/nasa/homework ext4 defaults 0 0 `即可。

![Screen Shot 2022-03-19 at 4.15.25 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-19 at 4.15.25 PM.png)

(Ref: https://www.cyberciti.biz/hardware/cryptsetup-add-enable-luks-disk-encryption-keyfile-linux/)



#### 5. Extend then Snapshot

```shell
sudo pvcreate /dev/sdd1
sudo vgextend NasaHW2 /dev/sdd1
sudo lvcreate -L 1GiB -s -n NasaHW2-backup /dev/NasaHW2/course
sudo mkdir /mnt/backup
sudo mount /dev/NasaHW2/NasaHW2-backup /mnt/backup
```

![Screen Shot 2022-03-19 at 5.24.39 PM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-19 at 5.24.39 PM.png)

```shell
sudo tar cvf /home/nasa/backup.tar.zst /mnt/backup
sudo umount /mnt/backup
sudo lvremove /dev/NasaHW2/NasaHW2-backup
```

(Ref: https://ithelp.ithome.com.tw/articles/10081811)



#### 6. Now, Start using ZFS

```shell
sudo zpool create b08902149 raidz sde sdf sdg sdh
sudo mkdir /mnt/zfs
sudo zfs set mountpoint=/mnt/zfs b08902149
```

![Screen Shot 2022-03-19 at 10.10.16 PM](/Users/chriscyhxx/Desktop/Screen Shot 2022-03-19 at 10.10.16 PM.png)

(Ref1: https://blog.ruanbekker.com/blog/2017/08/24/create-a-zfs-raidz1-volume-pool-on-ubuntu-16/)
(Ref2: https://askubuntu.com/questions/123126/how-do-i-mount-a-zfs-pool)

<div style="page-break-after: always;"></div>

#### 7. Create ZFS Dataset

首先執行下列指令：

```shell
sudo zfs create -o encryption=on -o keyformat=raw -o keylocation=file:///home/nasa/zfs_key b08902149/httpcat
sudo zfs set quota=500MiB b08902149/httpcat
sudo zfs set copies=2 b08902149/httpcat
sudo touch /etc/systemd/system/zfs-load-key@b08902149-httpcat.service
```

接著在`/etc/systemd/system/zfs-load-key@b08902149-httpcat.service`加上以下這段：

```
[Unit]
Description=Load %I encryption keys
Before=systemd-user-sessions.service
After=zfs-import.target
Before=zfs-mount.service

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/usr/bin/bash -c 'until (systemd-ask-password "Encrypted ZFS password for %I" --no-tty | zfs load-key %I); do echo "Try again!"; done'

[Install]
WantedBy=zfs-mount.service
```

然後繼續下列指令即可完成：

```shell
sudo systemctl enable zfs-load-key@b08902149-httpcat.service
sudo systemctl enable zfs-mount.service
sudo zfs set mountpoint=/home/nasa/httpcat b08902149/httpcat
sudo zfs mount -a
sudo cp /home/nasa/imgs/http_cat/* /home/nasa/httpcat -R
sudo reboot
```

重新啟動後執行`ls -l /home/nasa/httpcat; df -h`的結果：
![Screen Shot 2022-03-20 at 12.31.50 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-20 at 12.31.50 AM.png)
(Ref1: https://docs.freebsd.org/zh-tw/books/handbook/zfs/)
(Ref2: https://docs.oracle.com/cd/E23824_01/html/821-1448/gkkih.html)
(Ref3: https://wiki.archlinux.org/title/ZFS )



#### 8. Create ext4 on ZFS?

```shell
sudo zfs create -V 250MiB b08902149/test
sudo zfs set compression=lz4 b08902149/test
sudo fdisk /dev/b08902149/test       # create a partition
sudo mkfs -t ext4 /dev/zd0p1
sudo mkdir /home/nasa/test
sudo mount /dev/zd0p1 /home/nasa/test
```

![Screen Shot 2022-03-20 at 3.10.12 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-20 at 3.10.12 AM.png)

這個虛擬的block device是ZVOL。它可以充當virtual disk，當我們建立了一個ZVOL，這樣的操作就好像插入了一個空的disk，你可以在上面建立檔案系統。一個實際的應用是用於virtual machine，VM可以在ZVOL上面建立partition table。
(Ref1: https://pthree.org/2012/12/21/zfs-administration-part-xiv-zvols/)
(Ref2: https://pthree.org/2012/12/18/zfs-administration-part-xi-compression-and-deduplication/)
(Ref3: https://www.reddit.com/r/freenas/comments/itgwlz/what_do_you_use_a_zvol_for/)



#### 9. ZFS Snapshot

```shell
sudo zfs snapshot b08902149/httpcat@before
sudo curl https://http.cat/202.jpg -o /home/nasa/httpcat/202.jpg
sudo zfs snapshot b08902149/httpcat@after
```

![Screen Shot 2022-03-20 at 3.31.22 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-20 at 3.31.22 AM.png)

(Ref: https://docs.oracle.com/cd/E19253-01/819-5461/gbcya/index.html)

<div style="page-break-after: always;"></div>

#### 10. Rollback Snapshot

```shell
sudo zfs rollback -r b08902149/httpcat@before
```

![Screen Shot 2022-03-20 at 3.37.43 AM](/Users/chriscyhxx/Library/Application Support/typora-user-images/Screen Shot 2022-03-20 at 3.37.43 AM.png)

(Ref: https://docs.oracle.com/cd/E19253-01/819-5461/gbcxk/index.html)



#### 11. Short Answer

1. **ext**是Linux在1992創建的file system，而ext4是對於舊版本的更新。因為他是基於對 1990 年代初期開發的技術的翻新，對於管理modern data的附載能力有限，且它的journaling system也隨著所存資料量增加變得不再有用；但它可以支援不超過18TB的文件大小，對於data driven的數據公司是很適合的。**zfs**有著LVM和filesystem特性的檔案系統，可以對有檔案備份、壓縮等功能；相較於ext4，它對硬體的要求較高。
   (Ref: https://nascompares.com/2021/07/21/zfs-vs-ext4-for-nas-what-is-the-difference-in-your-file-system/)
2. **Raid0** : 透過兩個硬碟將檔案拆分（將同一筆檔案部分存在一個硬碟，另一部分存在另一個），實現讀寫速度的翻倍。缺點在於當其中一台硬碟壞掉，整個Raid0就無法運作，相較其他Raid，故障機率與資料損毀的風險較高。
   (Ref1: 上課講義)
   (Ref2: https://www.linwei.com.tw/forum-detail/13/)
   **Raid1** : 由兩台硬碟機髓組成，透過mirror的功能把資料複製成兩份，同時存在兩個硬碟中，實現自動備份的功能。且當只一台硬碟損壞，Raid1仍可正常運作。缺點是，雖然有兩台硬碟，但實際可以拿來儲存的空間卻只有一台硬碟的容量。
   (Ref1: 上課講義)
   (Ref2: https://www.linwei.com.tw/forum-detail/12/)
   **Raid5** : 每個Raid5會用至少三個硬碟，每個硬碟至多只有2/3的空間可以實際拿來存資料，剩下的部分要拿來存parity，也就是另外兩個硬碟中所存的實際資料做xor運算後的結果。好處是當其中一台硬碟壞掉，可以透過另外兩台來復原資料，儲存的方式很經濟。
   (Ref: 上課講義)
   **Raid10** : 至少需要四台硬碟。先以Raid0的方式分割資料成兩組之後，每組再將資料以Raid1的方式以兩個硬碟做鏡像複製儲存。是屬於較安全的資料儲存方式，但同時價格也會較昂貴。
   (Ref: https://zh.wikipedia.org/zh-tw/RAID)
3. FUSE是一種類UNIX電腦的作業系統之軟體介面，允許非特權用戶創建自己想要用的file system，而不需重新編譯kernel，zfs就是以FUSE實現。在user space實現file system的優點是可以大幅提升生產率，簡化了為作業系統提供新的檔案系統的工作量，缺點是在會有在kernel mode和user mode切換所帶來的開銷，對效能有一定程度的影響。
   (Ref1: https://zh.wikipedia.org/zh-tw/FUSE)
   (Ref2: https://www.twblogs.net/a/5b904bec2b7177672218c470)