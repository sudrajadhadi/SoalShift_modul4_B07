# Penjelasan Soal Shift Modul 3

### Nomer 1
* Semua nama file dan folder harus terenkripsi
* Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:
   ```
   qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0
   ```
* Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
* “INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
* Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

###### Jawab
Full Code: [AFSHiaAP_B07.c](https://github.com/sudrajadhadi/SoalShift_modul4_B07/blob/master/AFSHiaAP_B07.c)
* character list yang dibuat secara khusus tersebut disimpan ke suatu variabel
   ```
   char caesar[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
   ```
* key yang digunakan adalah 17, dan index di `mod 94` sebagai antisipasi jika hasil index caesar melebihi 94 dan dapat kembali ke index awal
* nama file disimpan dalam variabel bernama `data`, yang nantinya akan diganti sesuai dengan fungsi dekripsi dan enkripsi
* alurnya adalah mencari karakter tersebut dalam character list, jika sudah ditemukan, maka akan mengembalikan karakter yang berada pada index ke-17 setelah karakter ditemukan (enkripsi)
* atau index ke-17 sebelum karakter ditemukan (dekripsi)
* untuk huruf `/`, secara otomatis akan tetap tertulis `/`
* fungsi untuk dekripsi Caesar Cipher:
   ```
   void caesar_decrypt(char *data){
	    if(!strcmp(data, ".")) return;
	    if(!strcmp(data, "..")) return;
	    for(i=0; i<strlen(data); i++){
		     for(j=0; j<95; j++){
			      if(data[i] == caesar[j]){
				       data[i] = caesar[((j-17)+94)%94];
				       break;
			      }
		     }
	    }
   }
   ```
* fungsi untuk enkripsi Caesar Cipher:
   ```
   void caesar_encrypt(char *data){
	    if(!strcmp(data, ".")) return;
	    if(!strcmp(data, "..")) return;
	    for(i=0; i<strlen(data); i++){
	       for(j=0; j<95; j++){
			      if(data[i] == caesar[j]){
				       data[i] = caesar[(j+17)%94];
				       break;
			      }
		     }
	    }
   }
   ```
* mendekripsi nama file dengan:
   ```
   char *namanya;
   namanya=de->d_name;
   caesar_decrypt(namanya);
   ```
* menenkripsi nama dari setiap path dalam semua fungsi FUSE dengan:
   ```
   char pathnya[100];
	 sprintf(pathnya, "%s", path);
	 caesar_encrypt(pathnya);
   
	 char fpath[1000];
   sprintf(fpath,"%s%s", dirpath, pathnya);
   ```
