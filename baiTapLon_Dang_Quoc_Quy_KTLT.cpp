#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char monHoc[50];
    char thoiGian[10]; 
    char diaDiem[50];
} SuKien;

typedef struct NgayHoc {
    char ngay[11];  // "dd/mm/yyyy"
    SuKien* dsSuKien;
    int soLuong;
    int sucChua;
    struct NgayHoc* tiep;
} NgayHoc;

NgayHoc* danhSachNgay = NULL;

// ham xoa bo dem stdin
void donSachBoDem() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// kiem tra nam nhuan
int laNamNhuan(int nam) {
    return (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
}

// kiem tra ngay hop le
int kiemTraNgayHopLe(const char* ngay) {
    if (strlen(ngay) != 10) return 0;
    if (ngay[2] != '/' || ngay[5] != '/') return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(ngay[i])) return 0;
    }

    int dd = (ngay[0] - '0') * 10 + (ngay[1] - '0');
    int mm = (ngay[3] - '0') * 10 + (ngay[4] - '0');
    int yyyy = (ngay[6] - '0') * 1000 + (ngay[7] - '0') * 100 + (ngay[8] - '0') * 10 + (ngay[9] - '0');

    if (yyyy < 1900 || yyyy > 2100) return 0;
    if (mm < 1 || mm > 12) return 0;

    int soNgayTrongThang[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (laNamNhuan(yyyy)) soNgayTrongThang[1] = 29;

    if (dd < 1 || dd > soNgayTrongThang[mm - 1]) return 0;

    return 1;
}

// kiem tra thoi gian hop le 
int kiemTraThoiGianHopLe(const char* tg) {
    int len = strlen(tg);
    if (len < 4 || len > 5) return 0;  // vi du: 7:00 (4 ky tu) hoac 07:00 (5 ky tu)

    int colonPos = -1;
    for (int i = 0; i < len; i++) {
        if (tg[i] == ':') {
            colonPos = i;
            break;
        }
    }
    if (colonPos == -1) return 0; // khong co dau ':'

    // kiem tra phan gio
    for (int i = 0; i < colonPos; i++) {
        if (!isdigit(tg[i])) return 0;
    }
    // kiem tra phan phut
    for (int i = colonPos + 1; i < len; i++) {
        if (!isdigit(tg[i])) return 0;
    }

    // chuyen phan gio va phut thanh so nguyen
    int gio = 0, phut = 0;
    for (int i = 0; i < colonPos; i++) {
        gio = gio * 10 + (tg[i] - '0');
    }
    for (int i = colonPos + 1; i < len; i++) {
        phut = phut * 10 + (tg[i] - '0');
    }

    if (gio < 0 || gio > 23) return 0;
    if (phut < 0 || phut > 59) return 0;

    return 1;
}

// tim ngay trong danh sach lien ket
NgayHoc* timNgay(char* ngay) {
    NgayHoc* p = danhSachNgay;
    while (p != NULL) {
        if (strcmp(p->ngay, ngay) == 0) return p;
        p = p->tiep;
    }
    return NULL;
}

// them ngay moi (neu chua co)
NgayHoc* themNgay(char* ngay) {
    NgayHoc* moi = (NgayHoc*)malloc(sizeof(NgayHoc));
    if (moi == NULL) {
        printf("Loi cap phat bo nho!\n");
        return NULL;
    }
    strcpy(moi->ngay, ngay);
    moi->dsSuKien = (SuKien*)malloc(2 * sizeof(SuKien));  // ban dau chua 2 su kien
    if (moi->dsSuKien == NULL) {
        printf("Loi cap phat bo nho!\n");
        free(moi);
        return NULL;
    }
    moi->soLuong = 0;
    moi->sucChua = 2;
    moi->tiep = danhSachNgay;
    danhSachNgay = moi;
    return moi;
}

// kiem tra su kien da ton tai chua (cung ngay, cung mon, cung thoi gian)
int suKienTrungLap(NgayHoc* ngayHoc, const char* monHoc, const char* thoiGian) {
    for (int i = 0; i < ngayHoc->soLuong; i++) {
        if (strcmp(ngayHoc->dsSuKien[i].monHoc, monHoc) == 0 &&
            strcmp(ngayHoc->dsSuKien[i].thoiGian, thoiGian) == 0) {
            return 1; // trung lap
        }
    }
    return 0; // khong trung lap
}

void themSuKien() {
    char ngay[11];
    do {
        printf("Nhap ngay (dd/mm/yyyy): ");
        scanf("%10s", ngay);
        donSachBoDem();
        if (!kiemTraNgayHopLe(ngay)) {
            printf("Ngay khong hop le. Vui long nhap lai.\n");
        } else break;
    } while (1);

    NgayHoc* n = timNgay(ngay);
    if (n == NULL) {
        n = themNgay(ngay);
        if (n == NULL) return;
    }

    // Khai bao bien tam luu thong tin su kien
    char monHoc[50];
    char thoiGian[10];
    char diaDiem[50];

    printf("Nhap ten mon hoc: ");
    if (fgets(monHoc, sizeof(monHoc), stdin) == NULL) {
        printf("Loi nhap!\n");
        return;
    }
    monHoc[strcspn(monHoc, "\n")] = '\0';

    do {
        printf("Nhap thoi gian (h:mm hoac hh:mm): ");
        scanf("%9s", thoiGian);
        donSachBoDem();
        if (!kiemTraThoiGianHopLe(thoiGian)) {
            printf("Thoi gian khong hop le. Vui long nhap lai.\n");
        } else break;
    } while (1);

    // Kiem tra trung lap
    if (suKienTrungLap(n, monHoc, thoiGian)) {
        printf("Su kien da ton tai (trung ngay, mon hoc va thoi gian).\n");
        return;
    }

    printf("Nhap dia diem: ");
    if (fgets(diaDiem, sizeof(diaDiem), stdin) == NULL) {
        printf("Loi nhap!\n");
        return;
    }
    diaDiem[strcspn(diaDiem, "\n")] = '\0';

    // Them su kien vao danh sach
    if (n->soLuong >= n->sucChua) {
        n->sucChua *= 2;
        SuKien* tmp = (SuKien*)realloc(n->dsSuKien, n->sucChua * sizeof(SuKien));
        if (tmp == NULL) {
            printf("Khong du bo nho!\n");
            return;
        }
        n->dsSuKien = tmp;
    }

    SuKien* s = &n->dsSuKien[n->soLuong];
    strcpy(s->monHoc, monHoc);
    strcpy(s->thoiGian, thoiGian);
    strcpy(s->diaDiem, diaDiem);

    n->soLuong++;
    printf("Da them su kien thanh cong!\n");
}

// hien thi su kien cua ngay
void hienThiNgay() {
    char ngay[11];
    printf("Nhap ngay muon xem (dd/mm/yyyy): ");
    scanf("%10s", ngay);
    donSachBoDem();

    if (!kiemTraNgayHopLe(ngay)) {
        printf("Ngay khong hop le!\n");
        return;
    }

    NgayHoc* n = timNgay(ngay);
    if (n == NULL || n->soLuong == 0) {
        printf("Khong co lich hoc trong ngay nay.\n");
        return;
    }

    printf("== Lich hoc ngay %s ==\n", ngay);
    for (int i = 0; i < n->soLuong; i++) {
        printf("%d. %s - %s - %s\n", i + 1,
               n->dsSuKien[i].thoiGian,
               n->dsSuKien[i].monHoc,
               n->dsSuKien[i].diaDiem);
    }
}

// hien thi tat ca lich hoc
void hienThiTatCa() {
    NgayHoc* p = danhSachNgay;
    if (p == NULL) {
        printf("Chua co lich hoc nao.\n");
        return;
    }

    while (p != NULL) {
        printf("== Ngay %s ==\n", p->ngay);
        for (int i = 0; i < p->soLuong; i++) {
            printf(" %d. %s - %s - %s\n", i + 1,
                   p->dsSuKien[i].thoiGian,
                   p->dsSuKien[i].monHoc,
                   p->dsSuKien[i].diaDiem);
        }
        p = p->tiep;
    }
}

// tim kiem theo mon hoc
void timKiemTheoMon() {
    char mon[50];
    donSachBoDem();
    printf("Nhap ten mon hoc can tim: ");
    if (fgets(mon, sizeof(mon), stdin) == NULL) {
        printf("Loi nhap!\n");
        return;
    }
    mon[strcspn(mon, "\n")] = '\0';

    NgayHoc* p = danhSachNgay;
    int timThay = 0;
    while (p != NULL) {
        for (int i = 0; i < p->soLuong; i++) {
            if (strstr(p->dsSuKien[i].monHoc, mon) != NULL) {
                printf("Ngay %s - %s - %s - %s\n",
                       p->ngay,
                       p->dsSuKien[i].thoiGian,
                       p->dsSuKien[i].monHoc,
                       p->dsSuKien[i].diaDiem);
                timThay = 1;
            }
        }
        p = p->tiep;
    }

    if (!timThay) {
        printf("Khong tim thay lich hoc voi mon '%s'.\n", mon);
    }
}

// xoa su kien theo ngay va vi tri
void xoaSuKien() {
    char ngay[11];
    int viTri;
    printf("Nhap ngay (dd/mm/yyyy): ");
    scanf("%10s", ngay);
    donSachBoDem();

    if (!kiemTraNgayHopLe(ngay)) {
        printf("Ngay khong hop le!\n");
        return;
    }

    NgayHoc* n = timNgay(ngay);
    if (n == NULL || n->soLuong == 0) {
        printf("Khong co su kien trong ngay.\n");
        return;
    }

    printf("Nhap vi tri su kien can xoa (1-%d): ", n->soLuong);
    scanf("%d", &viTri);
    donSachBoDem();

    if (viTri < 1 || viTri > n->soLuong) {
        printf("Vi tri khong hop le!\n");
        return;
    }

    for (int i = viTri - 1; i < n->soLuong - 1; i++) {
        n->dsSuKien[i] = n->dsSuKien[i + 1];
    }
    n->soLuong--;

    printf("Da xoa su kien.\n");
}

// menu chinh
void menu() {
    int chon;
    do {
        printf("\n==== QUAN LY LICH HOC ====\n");
        printf("1. Them su kien\n");
        printf("2. Xem lich theo ngay\n");
        printf("3. Hien thi tat ca lich hoc\n");
        printf("4. Tim kiem theo mon hoc\n");
        printf("5. Xoa su kien theo ngay\n");
        printf("0. Thoat\n");
        printf("Chon: ");
        scanf("%d", &chon);
        donSachBoDem();

        switch (chon) {
            case 1: themSuKien(); break;
            case 2: hienThiNgay(); break;
            case 3: hienThiTatCa(); break;
            case 4: timKiemTheoMon(); break;
            case 5: xoaSuKien(); break;
            case 0: printf("Tam biet!\n"); break;
            default: printf("Lua chon khong hop le!\n");
        }

    } while (chon != 0);
}

int main() {
    menu();
    return 0;
}

