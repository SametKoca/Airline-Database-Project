///SAMET KOCA 140202037

#include <stdio.h>
#include <stdlib.h>
#include "include/mysql.h"
#include <string.h>

MYSQL *mysql;
MYSQL_RES *sonuc;
MYSQL_ROW satir;
MYSQL *baglan;
int sorgu;

int main()
{
    mysql = mysql_init(NULL);

    baglan = mysql_real_connect(mysql,"127.0.0.1","root","123456789",NULL,3306,NULL,0);

    sorgu = mysql_query(baglan,"CREATE DATABASE IF NOT EXISTS havayolu");

    mysql_select_db(baglan,"havayolu");

    sorgu = mysql_query(baglan,"CREATE TABLE IF NOT EXISTS yolcu_bilgileri(TCKN varchar(11) PRIMARY KEY,adsoyad varchar(100), adres varchar(100), sehir varchar(30), email varchar(100)) ENGINE = InnoDB");

    sorgu = mysql_query(baglan,"CREATE TABLE IF NOT EXISTS ucak_bilgileri(ukn varchar(20) PRIMARY KEY,model varchar(50), kapasite int) ENGINE = InnoDB");

    sorgu = mysql_query(baglan,"CREATE TABLE IF NOT EXISTS sefer_bilgileri(ukn varchar(20),saat varchar(4),tarih varchar(8),bn int,bin int) ENGINE = InnoDB");

    sorgu = mysql_query(baglan,"CREATE TABLE IF NOT EXISTS ucus_bilgileri(tarih varchar(8), ukn varchar(20), TCKN varchar(11), bn int, bin int) ENGINE = InnoDB");

    sorgu = mysql_query(baglan,"CREATE UNIQUE INDEX ucus_index ON ucus_bilgileri(tarih,ukn,TCKN,bn,bin)");

    sorgu = mysql_query(baglan,"CREATE INDEX ukn ON ucus_bilgileri(ukn)");

    sorgu = mysql_query(baglan,"CREATE INDEX TCKN ON ucus_bilgileri(TCKN)");

    sorgu = mysql_query(baglan,"CREATE INDEX bn ON ucus_bilgileri(bn)");

    sorgu = mysql_query(baglan,"CREATE INDEX bin ON ucus_bilgileri(bin)");

    sorgu = mysql_query(baglan,"CREATE UNIQUE INDEX sefer_index ON sefer_bilgileri(ukn,tarih,saat)");

    sorgu = mysql_query(baglan,"CREATE INDEX bn ON sefer_bilgileri(bn)");

    sorgu = mysql_query(baglan,"CREATE INDEX bin ON sefer_bilgileri(bin)");

    sorgu = mysql_query(baglan,"CREATE INDEX tarih ON sefer_bilgileri(tarih)");

    sorgu = mysql_query(baglan,"CREATE INDEX saat ON sefer_bilgileri(saat)");

    sorgu = mysql_query(baglan,"ALTER TABLE ucus_bilgileri ADD CONSTRAINT tarih FOREIGN KEY(tarih) REFERENCES sefer_bilgileri(tarih) ON DELETE CASCADE ON UPDATE CASCADE");

    sorgu = mysql_query(baglan,"ALTER TABLE ucus_bilgileri ADD CONSTRAINT ukn FOREIGN KEY(ukn) REFERENCES sefer_bilgileri(ukn) ON DELETE CASCADE ON UPDATE CASCADE");

    sorgu = mysql_query(baglan,"ALTER TABLE ucus_bilgileri ADD CONSTRAINT bn FOREIGN KEY(bn) REFERENCES sefer_bilgileri(bn) ON DELETE CASCADE ON UPDATE CASCADE");

    sorgu = mysql_query(baglan,"ALTER TABLE ucus_bilgileri ADD CONSTRAINT bin FOREIGN KEY(bin) REFERENCES sefer_bilgileri(bin) ON DELETE CASCADE ON UPDATE CASCADE");

    sorgu = mysql_query(baglan,"INSERT INTO ucak_bilgileri(ukn,model,kapasite) VALUES ('001','Boeing_777','50')");

    sorgu = mysql_query(baglan,"INSERT INTO ucak_bilgileri(ukn,model,kapasite) VALUES ('002','Yolcu_Ucagi','100')");

    sorgu = mysql_query(baglan,"INSERT INTO ucak_bilgileri(ukn,model,kapasite) VALUES ('003','Jet','10')");

    sorgu = mysql_query(baglan,"INSERT INTO ucak_bilgileri(ukn,model,kapasite) VALUES ('004','Boeing_757','40')");

    char query[1024];
    char KuyrukNo[21] , Tarih[9] , Saat[5] , KalkStr[3] , InisStr[3] , SecimStr[1];
    char basStr[3] , bitisStr[3] , BiletSayisiStr[3] , DolulukStr[4];
    int bas , bitis , BiletSayisi , Doluluk , DolulukDegeri;
    int Kalk , Inis , Secim;
    int i , Say = 0 , Tutulan = 0;

    printf("\n ~~~~~ HAVAYOLU VERITABANI ~~~~~\n");

    while(1)
    {
        printf("\n");
        printf("1 - YENI SEFER TANIMLAMA \n");
        printf("2 - BILET ALIM \n");
        printf("3 - YOLCU SORGULAMAK \n");
        printf("4 - UCUS IPTALI \n");
        printf("5 - BILET IPTALI \n");
        printf("6 - SEFER SORGULAMA \n");
        printf("7 - EN DOLU UCUS \n");
        printf("CIKIS ICIN 0 A BASIN\n");
        printf("SECIMINIZ : "); gets(SecimStr);
        Secim = atoi(SecimStr);

        switch (Secim)
        {

            case 1:

            printf("KUYRUK NO: "); gets(KuyrukNo);

            sorgu = sprintf(query,"SELECT ukn FROM ucak_bilgileri WHERE ukn = '%s'",KuyrukNo);
            sorgu = mysql_query(baglan,query);
            sonuc= mysql_store_result(baglan);

            if(mysql_affected_rows(baglan)){

                printf("TARIH: "); gets(Tarih);
                printf("SAAT: "); gets(Saat);
                printf("KALKIS: "); gets(KalkStr);
                printf("INIS: "); gets(InisStr);
                Kalk = atoi(KalkStr);
                Inis = atoi(InisStr);

                sorgu = sprintf(query,"SELECT ukn FROM sefer_bilgileri WHERE ukn = '%s' and tarih = '%s' and saat = '%s' and bn = '%s' and bin = '%s'",KuyrukNo,Tarih,Saat,KalkStr,InisStr);
                sorgu = mysql_query(baglan,query);
                sonuc= mysql_store_result(baglan);

                if(!mysql_affected_rows(baglan)){

                    sorgu = sprintf(query,"INSERT INTO sefer_bilgileri(ukn,saat,tarih,bn,bin) VALUES ('%s','%s','%s','%d','%d')",KuyrukNo,Saat,Tarih,Kalk,Inis);
                    sorgu = mysql_query(baglan,query);

                    printf("\nSEFER EKLENDI\n");
                }
                else
                    printf("\nSEFER ZATEN BULUNUYOR\n");

            }
            else
                printf("\nKAYIT BASARISIZ - UCAK BULUNAMADI\n");

            break;

            case 2:

            printf("KALKIS NOKTASI: "); gets(basStr);
            printf("INIS NOKTASI: "); gets(bitisStr);
            printf("TARIH : "); gets(Tarih);
            printf("BILET SAYISI: "); gets(BiletSayisiStr);

            bas = atoi(basStr);
            bitis = atoi(bitisStr);
            BiletSayisi = atoi(BiletSayisiStr);

            printf("\nDIREKT UCUSLAR\n");

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s WHERE s.bn = '%d' and s.bin = '%d' and s.Tarih = '%s'",bas,bitis,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            printf("\n1 AKTARMALI UCUSLAR\n");
            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s1 , sefer_bilgileri s2 WHERE s1.bn = '%d' and s1.bin = s2.bn and s2.bin = '%d' and s1.tarih = '%s' and s2.tarih = '%s'",bas,bitis,Tarih,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s2 , sefer_bilgileri s1 WHERE s1.bn = '%d' and s1.bin = s2.bn and s2.bin = '%d' and s1.tarih = '%s' and s2.tarih = '%s'",bas,bitis,Tarih,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            printf("\n2 AKTARMALI UCUSLAR\n");

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s1 , sefer_bilgileri s2, sefer_bilgileri s3 WHERE s1.bn = '%d' and s1.bin = s2.bn and s2.bin = s3.bn and s3.bin = '%d' and s1.tarih = '%s' and s2.tarih = '%s' and s3.tarih = '%s'",bas,bitis,Tarih,Tarih,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s2 , sefer_bilgileri s1, sefer_bilgileri s3 WHERE s1.bn = '%d' and s1.bin = s2.bn and s2.bin = s3.bn and s3.bin = '%d' and s1.tarih = '%s' and s2.tarih = '%s' and s3.tarih = '%s'",bas,bitis,Tarih,Tarih,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri s3 , sefer_bilgileri s2, sefer_bilgileri s1 WHERE s1.bn = '%d' and s1.bin = s2.bn and s2.bin = s3.bn and s3.bin = '%d' and s1.tarih = '%s' and s2.tarih = '%s' and s3.tarih = '%s'",bas,bitis,Tarih,Tarih,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nKUYRUK NO: %s - SAAT: %s - TARIH: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            char TC[12] , AdSoyad[101] , Adress[101] , Sehir[31] , Email[101] , KuyrukNo[21];

            printf("\nBINILECEK OLAN UCAGIN KUYRUK NO: "); gets(KuyrukNo);

            sorgu = sprintf(query,"SELECT COUNT(*) FROM ucus_bilgileri WHERE ukn = '%s' and tarih = '%s'",KuyrukNo,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);
            satir = mysql_fetch_row(sonuc);

            Doluluk = atoi(satir[0]);
            DolulukDegeri=0;

            sorgu = sprintf(query,"SELECT kapasite FROM ucak_bilgileri WHERE ukn = '%s' and kapasite > %d",KuyrukNo,Doluluk+BiletSayisi);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);
            satir = mysql_fetch_row(sonuc);

            if(satir != NULL){
                DolulukDegeri=1; printf("UCAGIN KAPASITESI : %s\nUCUSDAKI YOLCU SAYISI : %d\nBOS KOLTUK SAYISI : %d\n",satir[0],Doluluk,atoi(satir[0])-Doluluk);
            }
            else{
                printf("\nKAPASITE YETERSIZ\n"); DolulukDegeri=0;
            }

            if(DolulukDegeri == 1){

                for(i=0;i<BiletSayisi;i++){

                    printf("%d. TCKN: ",i+1); gets(TC);
                    sorgu = sprintf(query,"INSERT INTO ucus_bilgileri(tarih,ukn,TCKN,bn,bin) VALUES ('%s','%s','%s','%d','%d')",Tarih,KuyrukNo,TC,bas,bitis);
                    sorgu = mysql_query(baglan,query);
                    sonuc = mysql_store_result(baglan);
                    sorgu = sprintf(query,"SELECT * FROM yolcu_bilgileri WHERE TCKN = '%s'",TC);
                    sorgu = mysql_query(baglan,query);
                    sonuc = mysql_store_result(baglan);

                    if(!mysql_affected_rows(baglan)){

                        printf("\nYOLCU KAYITTA BULUNAMADI. YENI KAYIT GIRIN  \n");
                        printf("\nAD SOYAD: ");  gets(AdSoyad);
                        printf("\nADRES: ");     gets(Adress);
                        printf("\nSEHIR: ");     gets(Sehir);
                        printf("\nEMAIL: ");     gets(Email);

                        sorgu = sprintf(query,"INSERT INTO yolcu_bilgileri(TCKN,adsoyad,adres,sehir,email) VALUES ('%s','%s','%s','%s','%s')",TC,AdSoyad,Adress,Sehir,Email);
                        sorgu = mysql_query(baglan,query);
                        sonuc = mysql_store_result(baglan);
                        printf("\nKAYIT BASARIYLA EKLENDI\n");
                    }
                }
            }

            break;

            case 3:

            printf("TC: "); gets(TC);
            sorgu = sprintf(query,"SELECT * FROM ucus_bilgileri WHERE TCKN = '%s'",TC);

            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nTARIH: %s - UKN: %s - KALKIS: %s - INIS: %s\n",satir[0],satir[1],satir[3],satir[4]);
            }

            break;

            case 4:

            printf("\nKUYRUK NO: "); gets(KuyrukNo);
            printf("\nTARIH: "); gets(Tarih);

            sorgu = sprintf(query,"SELECT * FROM sefer_bilgileri WHERE ukn = '%s' and tarih = '%s'",KuyrukNo,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                sorgu = sprintf(query,"DELETE FROM sefer_bilgileri WHERE ukn = '%s' and tarih = '%s'",KuyrukNo,Tarih);
                sorgu = mysql_query(baglan,query);
                Say++;
            }

            printf("\n%d TANE UCUS SILINDI",Say);

            break;

            case 5:

            printf("TC : ");         gets(TC);        printf("\n");
            printf("TARIH : ");      gets(Tarih);     printf("\n");
            printf("KUYRUK NO : ");  gets(KuyrukNo);  printf("\n");

            sorgu = sprintf(query,"DELETE FROM ucus_bilgileri WHERE TCKN = '%s' and tarih = '%s' and ukn = '%s'",TC,Tarih,KuyrukNo);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            if(mysql_affected_rows(baglan))
                printf("BILET BASARIYLA IPTAL EDILDI");

            else
                printf("BILET IPTALI BASARISIZ");

            break;

            case 6:

            printf("TARIH : ");      gets(Tarih);     printf("\n");
            printf("KUYRUK NO : ");  gets(KuyrukNo);  printf("\n");

            sorgu = sprintf(query,"SELECT * FROM yolcu_bilgileri WHERE TCKN IN (SELECT TCKN FROM ucus_bilgileri WHERE ukn = '%s' and tarih = '%s')",KuyrukNo,Tarih);
            sorgu = mysql_query(baglan,query);
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                printf("\nTCKN: %s \nAD SOYAD: %s \nADRESS: %s \nSEHIR: %s \nMAIL: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
            }

            break;

            case 7:

            sorgu = mysql_query(baglan,"SELECT ukn,tarih,bn,bin,COUNT(*) AS Toplam FROM ucus_bilgileri GROUP BY tarih , ukn ORDER BY Toplam DESC");
            sonuc = mysql_store_result(baglan);

            while((satir = mysql_fetch_row(sonuc))!=NULL)
            {
                if(atoi(satir[4]) >= Tutulan && atoi(satir[4])!=0)
                {
                    printf("KUYRUK NO: %s - TARIH: %s - KALKIS: %s - INIS: %s - DOLULUK: %s\n",satir[0],satir[1],satir[2],satir[3],satir[4]);
                    Tutulan = satir[4];
                }
            }

            break;

            case 0:

            goto FLAG;

            break;

            default:

            printf("YANLIS DEGER GIRILDI");

            break;
        }
    }

    FLAG:

    return 0;
}
